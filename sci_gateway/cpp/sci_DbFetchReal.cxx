/*
 * Copyright (C) 2010 - Igor GRIDCHYN
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "sci_util.h"
#include "sci_db.h"

extern "C"
{
    int sci_DbFetchReal(char *fname)
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

        double *pdResults  = (double*)malloc(sizeof(double) * rec.count());

        bool *pbConvertOk = (bool*)malloc(sizeof(bool));

        for(int i=0; i < rec.count(); i++)
        {
            pdResults[i] = rec.value(i).toDouble(pbConvertOk);
            if(!(*pbConvertOk))
            {
                Scierror(999, "Cannot convert %d-th value (%s) to double.\n", i, rec.value(i).toString().toLatin1().data());

                free(pdResults);
                free(pbConvertOk);

                return 0;
            }
        }

        sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, rec.count(), pdResults);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        free(pdResults);
        free(pbConvertOk);

        psqQuery->next();

        LhsVar(1) = Rhs + 1;

        return 0;
    }
/* ==================================================================== */
} /* extern "C" */
/* ==================================================================== */
