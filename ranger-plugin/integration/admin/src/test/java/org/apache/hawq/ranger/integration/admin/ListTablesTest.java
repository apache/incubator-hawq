/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific schema governing permissions and limitations
 * under the License.
 */

package org.apache.hawq.ranger.integration.admin;

import com.google.common.collect.Sets;
import org.junit.Before;
import org.junit.Test;

import java.util.List;
import java.util.Map;
import java.util.HashMap;
import java.util.Arrays;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class ListTablesTest extends LookupTestBase {

    private Map<String, List<String>> resources;

    @Before
    public void setUp() {
        resources = new HashMap<>();
    }

    @Test
    public void testListTables_NoSchemaDb_AllSchemas_AllFilter() throws Exception {
        resources.put("database", Arrays.asList("noschema_db"));
        resources.put("schema", Arrays.asList("*"));
        List<String> result = service.lookupResource(getContext("table", "*", resources));
        assertTrue(result.isEmpty());
    }

    @Test
    public void testListTables_SingleDb_SingleSchema_AllFilter_NoTables() throws Exception {
        resources.put("database", Arrays.asList("west"));
        resources.put("schema", Arrays.asList("jamaica"));
        List<String> result = service.lookupResource(getContext("table", "*", resources));
        assertTrue(result.isEmpty());
    }

    @Test
    public void testListTables_SingleDb_SingleSchema_AllFilter() throws Exception {
        resources.put("database", Arrays.asList("east"));
        resources.put("schema", Arrays.asList("japan"));
        List<String> result = service.lookupResource(getContext("table", "*", resources));
        assertEquals(2, result.size());
        assertTrue(Sets.newHashSet(result).equals(Sets.newHashSet("rice", "sushi")));
    }

    @Test
    public void testListTables_SingleDb_TwoSchemas_AllFilter() throws Exception {
        resources.put("database", Arrays.asList("east"));
        resources.put("schema", Arrays.asList("common", "japan"));
        List<String> result = service.lookupResource(getContext("table", "*", resources));
        assertEquals(3, result.size());
        assertTrue(Sets.newHashSet(result).equals(Sets.newHashSet("rice", "soup", "sushi")));
    }

    @Test
    public void testListTables_SingleDb_AllSchemas_AllFilter() throws Exception {
        resources.put("database", Arrays.asList("east"));
        resources.put("schema", Arrays.asList("*"));
        List<String> result = service.lookupResource(getContext("table", "*", resources));
        assertEquals(3, result.size());
        assertTrue(Sets.newHashSet(result).equals(Sets.newHashSet("rice", "soup", "sushi")));
    }

    @Test
    public void testListTables_TwoDb_CommonSchema_AllFilter() throws Exception {
        resources.put("database", Arrays.asList("east", "west"));
        resources.put("schema", Arrays.asList("common"));
        List<String> result = service.lookupResource(getContext("table", "*", resources));
        assertEquals(2, result.size());
        assertTrue(Sets.newHashSet(result).equals(Sets.newHashSet("rice", "soup")));
    }

    @Test
    public void testListTables_TwoDb_SingleSchema_AllFilter() throws Exception {
        resources.put("database", Arrays.asList("east", "west"));
        resources.put("schema", Arrays.asList("japan"));
        List<String> result = service.lookupResource(getContext("table", "*", resources));
        assertEquals(2, result.size());
        assertTrue(Sets.newHashSet(result).equals(Sets.newHashSet("rice", "sushi")));
    }

    @Test
    public void testListTables_TwoDb_AllSchemas_AllFilter() throws Exception {
        resources.put("database", Arrays.asList("east", "west"));
        resources.put("schema", Arrays.asList("*"));
        List<String> result = service.lookupResource(getContext("table", "*", resources));
        assertEquals(4, result.size());
        assertTrue(Sets.newHashSet(result).equals(Sets.newHashSet("rice", "soup", "sushi", "stew")));
    }

    @Test
    public void testListTables_AllDb_AllSchemas_AllFilter() throws Exception {
        resources.put("database", Arrays.asList("*"));
        resources.put("schema", Arrays.asList("*"));
        List<String> result = service.lookupResource(getContext("table", "*", resources));
        assertEquals(4, result.size());
        assertTrue(Sets.newHashSet(result).equals(Sets.newHashSet("rice", "soup", "sushi", "stew")));
    }

    @Test
    public void testListTables_SingleDb_SingleSchema_FilteredAbsent() throws Exception {
        resources.put("database", Arrays.asList("east"));
        resources.put("schema", Arrays.asList("japan"));
        List<String> result = service.lookupResource(getContext("table", "z", resources));
        assertTrue(result.isEmpty());
    }

    @Test
    public void testListTables_SingleDb_TwoSchemas_FilteredAbsent() throws Exception {
        resources.put("database", Arrays.asList("east"));
        resources.put("schema", Arrays.asList("common", "japan"));
        List<String> result = service.lookupResource(getContext("table", "z", resources));
        assertTrue(result.isEmpty());
    }

    @Test
    public void testListTables_SingleDb_AllSchemas_FilteredAbsent() throws Exception {
        resources.put("database", Arrays.asList("east"));
        resources.put("schema", Arrays.asList("*"));
        List<String> result = service.lookupResource(getContext("table", "z", resources));
        assertTrue(result.isEmpty());
    }

    @Test
    public void testListTables_TwoDbs_CommonSchema_FilteredAbsent() throws Exception {
        resources.put("database", Arrays.asList("east", "west"));
        resources.put("schema", Arrays.asList("common"));
        List<String> result = service.lookupResource(getContext("table", "z", resources));
        assertTrue(result.isEmpty());
    }

    @Test
    public void testListTables_TwoDbs_SingleSchema_FilteredAbsent() throws Exception {
        resources.put("database", Arrays.asList("east", "west"));
        resources.put("schema", Arrays.asList("japan"));
        List<String> result = service.lookupResource(getContext("table", "z", resources));
        assertTrue(result.isEmpty());
    }

    @Test
    public void testListTables_TwoDbs_AllSchemas_FilteredAbsent() throws Exception {
        resources.put("database", Arrays.asList("east", "west"));
        resources.put("schema", Arrays.asList("*"));
        List<String> result = service.lookupResource(getContext("table", "z", resources));
        assertTrue(result.isEmpty());
    }

    @Test
    public void testListTables_AllDbs_AllSchemas_FilteredAbsent() throws Exception {
        resources.put("database", Arrays.asList("*"));
        resources.put("schema", Arrays.asList("*"));
        List<String> result = service.lookupResource(getContext("table", "z", resources));
        assertTrue(result.isEmpty());
    }

    @Test
    public void testListTables_SingleDb_SingleSchema_FilteredPresent() throws Exception {
        resources.put("database", Arrays.asList("east"));
        resources.put("schema", Arrays.asList("japan"));
        List<String> result = service.lookupResource(getContext("table", "s", resources));
        assertEquals(1, result.size());
        assertTrue(Sets.newHashSet(result).equals(Sets.newHashSet("sushi")));
    }

    @Test
    public void testListTables_SingleDb_TwoSchemas_FilteredPresent() throws Exception {
        resources.put("database", Arrays.asList("east"));
        resources.put("schema", Arrays.asList("common", "japan"));
        List<String> result = service.lookupResource(getContext("table", "s", resources));
        assertEquals(2, result.size());
        assertTrue(Sets.newHashSet(result).equals(Sets.newHashSet("soup", "sushi")));
    }

    @Test
    public void testListTables_SingleDb_AllSchemas_FilteredPresent() throws Exception {
        resources.put("database", Arrays.asList("east"));
        resources.put("schema", Arrays.asList("*"));
        List<String> result = service.lookupResource(getContext("table", "s", resources));
        assertEquals(2, result.size());
        assertTrue(Sets.newHashSet(result).equals(Sets.newHashSet("soup", "sushi")));
    }

    @Test
    public void testListTables_SingleDb_AllSchemas_FilteredPresent2() throws Exception {
        resources.put("database", Arrays.asList("east"));
        resources.put("schema", Arrays.asList("*"));
        List<String> result = service.lookupResource(getContext("table", "r", resources));
        assertEquals(1, result.size());
        assertTrue(Sets.newHashSet(result).equals(Sets.newHashSet("rice")));
    }

    @Test
    public void testListTables_TwoDbs_CommonSchema_FilteredPresent() throws Exception {
        resources.put("database", Arrays.asList("east", "west"));
        resources.put("schema", Arrays.asList("common"));
        List<String> result = service.lookupResource(getContext("table", "r", resources));
        assertEquals(1, result.size());
        assertTrue(Sets.newHashSet(result).equals(Sets.newHashSet("rice")));
    }

    @Test
    public void testListTables_TwoDbs_SingleSchema_FilteredPresent() throws Exception {
        resources.put("database", Arrays.asList("east", "west"));
        resources.put("schema", Arrays.asList("japan"));
        List<String> result = service.lookupResource(getContext("table", "s", resources));
        assertEquals(1, result.size());
        assertTrue(Sets.newHashSet(result).equals(Sets.newHashSet("sushi")));
    }

    @Test
    public void testListTables_TwoDbs_AllSchemas_FilteredPresent() throws Exception {
        resources.put("database", Arrays.asList("east", "west"));
        resources.put("schema", Arrays.asList("*"));
        List<String> result = service.lookupResource(getContext("table", "s", resources));
        assertEquals(3, result.size());
        assertTrue(Sets.newHashSet(result).equals(Sets.newHashSet("soup", "sushi", "stew")));
    }

    @Test
    public void testListTables_AllDbs_AllSchemas_FilteredPresent() throws Exception {
        resources.put("database", Arrays.asList("*"));
        resources.put("schema", Arrays.asList("*"));
        List<String> result = service.lookupResource(getContext("table", "s", resources));
        assertEquals(3, result.size());
        assertTrue(Sets.newHashSet(result).equals(Sets.newHashSet("soup", "sushi", "stew")));
    }

}