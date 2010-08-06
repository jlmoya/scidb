/* ==================================================================== */
/* Igor GRIDCHYN */
/* Scidb toolbox */
/* This file is released into the public domain */
/* ==================================================================== */

/* ==================================================================== */
#include "sci_util.h"
#include "sci_db.h"
/* ==================================================================== */
extern "C"
{
	int sci_DbLastQueryError(char *fname)
	{
		SciErr sciErr;
		QSqlQuery *query;

		CheckRhs(1,1);
		CheckLhs(0,1);

		char *cpLastError;

		sciGetQueryParam(fname, 1, &query);

		cpLastError = (char*)malloc(sizeof(char) * query->lastError().text().length());
		strcpy(cpLastError, query->lastError().text().toLatin1().data());

		sciprint("Last error: %s", query->lastError().text().toLatin1().data());

		sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, 1, 1, &cpLastError);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}	

		LhsVar(1) = Rhs + 1;

		return 0;
	}
/* ==================================================================== */	
} /* extern "C" */
/* ==================================================================== */