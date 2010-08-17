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
    int sci_DbFetchString(char *fname)
    {
        SciErr sciErr;
        QSqlQuery *psqQuery;

        CheckRhs(1,1);
        CheckLhs(0,1);

        sciGetQSqlQueryAt(fname, 1, &psqQuery);

        if(!psqQuery->isActive())
        {
            Scierror(999, "Given query was not successfully executed.\n");
            return 0;
        }

        if(!psqQuery->isValid() && !psqQuery->next())
        {
            Scierror(999, "No results in query.\n");
            return 0;
        }

        QSqlRecord rec = psqQuery-> record();

        char **ppcResultStrings  = (char**)malloc(sizeof(char*) * rec.count());

        for(int i=0; i < rec.count(); i++)
        {
            QString sVal = rec.value(i).toString();
            ppcResultStrings[i] = (char*)malloc(sizeof(char) * sVal.length());
            strcpy(ppcResultStrings[i], sVal.toLatin1().data());
        }

        sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, 1, rec.count(), ppcResultStrings);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        free(ppcResultStrings);

        psqQuery->next();

        LhsVar(1) = Rhs + 1;

        return 0;
    }
/* ==================================================================== */
} /* extern "C" */
/* ==================================================================== */
