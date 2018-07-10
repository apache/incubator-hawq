package org.apache.hawq.pxf.service;

/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

import java.io.IOException;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.DelayQueue;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.security.UserGroupInformation;

public class UGICache {

    private static final Log LOG = LogFactory.getLog(UGICache.class);
    private Map<SessionId, UGICacheEntry> cache = new ConcurrentHashMap<>();
    @SuppressWarnings("unchecked")
    // There is a separate DelayQueue for each segment (also being used for locking)
    private DelayQueue<UGICacheEntry>[] delayQueues = (DelayQueue<UGICacheEntry>[])new DelayQueue[64];
    private final UGIProvider ugiProvider;

    public UGICache(UGIProvider provider) {
        this.ugiProvider = provider;
        for (int i = 0; i < delayQueues.length; i++) {
            delayQueues[i] = new DelayQueue<>();
        }
    }

    public UGICache() {
        this(new UGIProvider());
    }

    // Create new proxy UGI if not found in cache and increment reference count
    public UGICacheEntry getTimedProxyUGI(SessionId session)
            throws IOException {

        Integer segmentId = session.getSegmentId();
        String user = session.getUser();
        synchronized (delayQueues[segmentId]) {
            // Use the opportunity to cleanup any expired entries
            cleanup(segmentId);
            UGICacheEntry timedProxyUGI = cache.get(session);
            if (timedProxyUGI == null) {
                LOG.info(session.toString() + " Creating proxy user = " + user);
                UserGroupInformation proxyUGI = ugiProvider.createProxyUGI(user);
                timedProxyUGI = new UGICacheEntry(proxyUGI, session);
                delayQueues[segmentId].offer(timedProxyUGI);
                cache.put(session, timedProxyUGI);
            }
            timedProxyUGI.incrementCounter();
            return timedProxyUGI;
        }
    }

    // Poll segment expiration queue for all expired entries
    // and clean them if possible
    private void cleanup(Integer segmentId) {

        UGICacheEntry ugi = null;
        while ((ugi = delayQueues[segmentId].poll()) != null) {
            // Place it back in the queue if still in use and was not closed
            if (!closeUGI(ugi)) {
                delayQueues[segmentId].offer(ugi);
            }
            LOG.debug("Delay Queue Size for segment " +
                    segmentId + " = " + delayQueues[segmentId].size());
        }
    }

    // Decrement reference count and optional cleanup
    public void release(UGICacheEntry timedProxyUGI, boolean forceClean) {

        Integer segmentId = timedProxyUGI.getSession().getSegmentId();
        timedProxyUGI.resetTime();
        timedProxyUGI.decrementCounter();
        if (forceClean) {
            synchronized (delayQueues[segmentId]) {
                timedProxyUGI.setExpired();
                closeUGI(timedProxyUGI);
            }
        }
    }

    // There is no need to synchronize this method because it should be called
    // from within a synchronized block
    private boolean closeUGI(UGICacheEntry expiredProxyUGI) {

        SessionId session = expiredProxyUGI.getSession();
        String fsMsg = "FileSystem for proxy user = " + expiredProxyUGI.getUGI().getUserName();
        try {
            // The UGI object is still being used by another thread
            if (expiredProxyUGI.getCounter() != 0) {
                LOG.info(session.toString() + " Skipping close of " + fsMsg);
                // Reset time so that it doesn't expire until release
                // updates the time again
                expiredProxyUGI.resetTime();
                return false;
            }

            // Expired UGI object can be cleaned since it is not used
            // Determine if it can be removed from cache also
            UGICacheEntry cachedUGI = cache.get(session);
            if (expiredProxyUGI == cachedUGI) {
                // Remove it from cache, as cache now has an
                // expired entry which is not in progress
                cache.remove(session);
            }

            // Optimization to call close only if it has not been
            // called for that UGI
            if (!expiredProxyUGI.isCleaned()) {
                LOG.info(session.toString() + " Closing " + fsMsg +
                        " (Cache Size = " + cache.size() + ")");
                ugiProvider.destroy(expiredProxyUGI.getUGI());
                expiredProxyUGI.setCleaned();
            }

        } catch (Throwable t) {
            LOG.warn(session.toString() + " Error closing " + fsMsg);
        }
        return true;
    }

}
