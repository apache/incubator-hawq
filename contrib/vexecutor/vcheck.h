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

#ifndef VCHECK_H
#define VCHECK_H

#include "vadt.h"
typedef struct vFuncMap
{
    Oid ntype;
    vheader* (* vtbuild)(int n);
    void (* vtfree)(vheader **vh);
    size_t (* vtsize)(vheader *vh);
    size_t (*serialization)(vheader* vh, unsigned char* buf);
    Datum (*deserialization)(unsigned char* buf,size_t* len);
}vFuncMap;

extern const vFuncMap* GetVFunc(Oid vtype);
extern Plan* CheckPlanVectorzied(PlannerInfo *root, Plan *plan);
extern Plan* ReplacePlanVectorzied(PlannerInfo *root, Plan *plan);
extern Oid GetVtype(Oid ntype);

#endif
