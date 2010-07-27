/* ==================================================================== */
/* Igor GRICHYN */
/* Scidb toolbox */
/* This file is released into the public domain */
/* ==================================================================== */

/* ==================================================================== */
#include "sci_util.h"
#include "sci_db.h"
/* ==================================================================== */
extern "C"
{
	int sci_DbLastInsertId(char *fname)
	{
		SciErr sciErr;
		QSqlQuery *psqQuery;

		CheckRhs(1,1);
		CheckLhs(0,1);

		sciGetQSqlQueryAt(fname, 1, &psqQuery);		

		QVariant vLastInsertId = psqQuery->lastInsertId();									

		char *cLastInsertId = vLastInsertId.toString().toLatin1().data();

		sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, 1, 1, &cLastInsertId);
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