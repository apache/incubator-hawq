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

//---------------------------------------------------------------------------
//	@filename:
//		CGPOptimizer.cpp
//
//	@doc:
//		Entry point to GP optimizer
//
//	@test:
//
//
//---------------------------------------------------------------------------

#include "gpopt/CGPOptimizer.h"
#include "gpopt/utils/COptTasks.h"

// the following headers are needed to reference optimizer library initializers
#include "naucrates/init.h"
#include "gpopt/init.h"
#include "gpos/_api.h"

#include "naucrates/exception.h"

extern MemoryContext MessageContext;

//---------------------------------------------------------------------------
//	@function:
//		CGPOptimizer::TouchLibraryInitializers
//
//	@doc:
//		Touch library initializers to enforce linker to include them
//
//---------------------------------------------------------------------------
void
CGPOptimizer::TouchLibraryInitializers()
{
	void (*gpos)(gpos_init_params*) = gpos_init;
	void (*dxl)() = gpdxl_init;
	void (*opt)() = gpopt_init;
}


//---------------------------------------------------------------------------
//	@function:
//		CGPOptimizer::PlstmtOptimize
//
//	@doc:
//		Optimize given query using GP optimizer
//
//---------------------------------------------------------------------------
PlannedStmt *
CGPOptimizer::PplstmtOptimize
	(
	Query *pquery,
	bool *pfUnexpectedFailure // output : set to true if optimizer unexpectedly failed to produce plan
	)
{
	SOptContext octx;
	PlannedStmt* plStmt = NULL;
	GPOS_TRY
	{
		plStmt = COptTasks::PplstmtOptimize(pquery, &octx, pfUnexpectedFailure);
		// clean up context
		octx.Free(octx.epinQuery, octx.epinPlStmt);
	}
	GPOS_CATCH_EX(ex)
	{
		// clone the error message before context free.
		CHAR* szErrorMsg = octx.CloneErrorMsg(MessageContext);
		// clean up context
		octx.Free(octx.epinQuery, octx.epinPlStmt);
		if (GPOS_MATCH_EX(ex, gpdxl::ExmaDXL, gpdxl::ExmiOptimizerError) ||
			NULL != szErrorMsg)
		{
			Assert(NULL != szErrorMsg);
			errstart(ERROR, ex.SzFilename(), ex.UlLine(), NULL, TEXTDOMAIN);
			errfinish(errcode(ERRCODE_INTERNAL_ERROR),
					errmsg("%s", szErrorMsg));
		}
		else if (GPOS_MATCH_EX(ex, gpdxl::ExmaGPDB, gpdxl::ExmiGPDBError))
		{
			PG_RE_THROW();
		}
		else if (GPOS_MATCH_EX(ex, gpdxl::ExmaDXL, gpdxl::ExmiNoAvailableMemory))
		{
			errstart(ERROR, ex.SzFilename(), ex.UlLine(), NULL, TEXTDOMAIN);
			errfinish(errcode(ERRCODE_INTERNAL_ERROR),
					errmsg("No available memory to allocate string buffer."));
		}
		else if (GPOS_MATCH_EX(ex, gpdxl::ExmaDXL, gpdxl::ExmiInvalidComparisonTypeCode))
		{
			errstart(ERROR, ex.SzFilename(), ex.UlLine(), NULL, TEXTDOMAIN);
			errfinish(errcode(ERRCODE_INTERNAL_ERROR),
					errmsg("Invalid comparison type code. Valid values are Eq, NEq, LT, LEq, GT, GEq."));
		}
	}
	GPOS_CATCH_END;
	return plStmt;
}


//---------------------------------------------------------------------------
//	@function:
//		CGPOptimizer::SzDXL
//
//	@doc:
//		Serialize planned statement into DXL
//
//---------------------------------------------------------------------------
char *
CGPOptimizer::SzDXLPlan
	(
	Query *pquery
	)
{
	return COptTasks::SzOptimize(pquery);
}

//---------------------------------------------------------------------------
//	@function:
//		InitGPOPT()
//
//	@doc:
//		Initialize GPTOPT and dependent libraries
//
//---------------------------------------------------------------------------
void
CGPOptimizer::InitGPOPT ()
{
  // Use GPORCA's default allocators
  struct gpos_init_params params = { NULL, NULL };
  gpos_init(&params);
  gpdxl_init();
  gpopt_init();
}

//---------------------------------------------------------------------------
//	@function:
//		TerminateGPOPT()
//
//	@doc:
//		Terminate GPOPT and dependent libraries
//
//---------------------------------------------------------------------------
void
CGPOptimizer::TerminateGPOPT ()
{
  gpopt_terminate();
  gpdxl_terminate();
  gpos_terminate();
}

//---------------------------------------------------------------------------
//	@function:
//		PplstmtOptimize
//
//	@doc:
//		Expose GP optimizer API to C files
//
//---------------------------------------------------------------------------
extern "C"
{
PlannedStmt *PplstmtOptimize
	(
	Query *pquery,
	bool *pfUnexpectedFailure
	)
{
	return CGPOptimizer::PplstmtOptimize(pquery, pfUnexpectedFailure);
}
}

//---------------------------------------------------------------------------
//	@function:
//		SzDXLPlan
//
//	@doc:
//		Serialize planned statement to DXL
//
//---------------------------------------------------------------------------
extern "C"
{
char *SzDXLPlan
	(
	Query *pquery
	)
{
	return CGPOptimizer::SzDXLPlan(pquery);
}
}

//---------------------------------------------------------------------------
//	@function:
//		InitGPOPT()
//
//	@doc:
//		Initialize GPTOPT and dependent libraries
//
//---------------------------------------------------------------------------
extern "C"
{
void InitGPOPT ()
{
	return CGPOptimizer::InitGPOPT();
}
}

//---------------------------------------------------------------------------
//	@function:
//		TerminateGPOPT()
//
//	@doc:
//		Terminate GPOPT and dependent libraries
//
//---------------------------------------------------------------------------
extern "C"
{
void TerminateGPOPT ()
{
	return CGPOptimizer::TerminateGPOPT();
}
}

// EOF
