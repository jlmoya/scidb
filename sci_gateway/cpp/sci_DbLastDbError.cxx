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
    int sci_DbLastDbError(char *fname)
    {
        SciErr sciErr;
        QSqlDatabase *db;

        CheckRhs(0,1);
        CheckLhs(0,1);

        char *cpLastError;

        if(Rhs == 0)
        {
            db = &(QSqlDatabase::database(sDefaultConnection));
        }
        else
        {
            getDatabaseParam(fname, 1, &db);
        }

        cpLastError = (char*)malloc(sizeof(char) * db->lastError().text().length());
        strcpy(cpLastError, db->lastError().text().toLatin1().data());

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
