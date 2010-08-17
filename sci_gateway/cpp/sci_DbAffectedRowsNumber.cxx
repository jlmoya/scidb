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
    int sci_DbAffectedRowsNumber(char *fname)
    {
        SciErr sciErr;
        QSqlQuery *query;

        CheckRhs(1,1);
        CheckLhs(0,1);

        sciGetQueryParam(fname, 1, &query);

        int iRowsAffected = query->numRowsAffected();

        sciErr = createMatrixOfInteger32(pvApiCtx, Rhs + 1, 1, 1, &iRowsAffected);
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
