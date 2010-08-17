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
    int sci_DbFetchAllString(char *fname)
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

        QSqlRecord rec;
        char **ppcResultStrings = NULL;

        int iRecsCount = 0;
        rec = psqQuery->record();
        int iFiledCount = rec.count();

        if(!psqQuery->isValid())
        {
            psqQuery->next();
        }

        do
        {
            iRecsCount++;
            ppcResultStrings = (char**)realloc(ppcResultStrings, sizeof(char*)*iRecsCount*iFiledCount);

            QSqlRecord rec = psqQuery-> record();

            for(int i=0; i < rec.count(); i++)
            {
                QString sVal = rec.value(i).toString();
                ppcResultStrings[ ( iRecsCount - 1 ) * iFiledCount + i ] = (char*)malloc(sizeof(char) * sVal.length());
                strcpy(ppcResultStrings[( iRecsCount - 1 ) * iFiledCount + i ], sVal.toLatin1().data());
            }
        }
        while(psqQuery->next());

        char **ppcTransposedResults = NULL;
        transposeStringMatrix(ppcResultStrings, iRecsCount, iFiledCount, &ppcTransposedResults);

        //sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, iRecsCount, iFiledCount, ppcResultStrings);
        sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, iRecsCount, iFiledCount, ppcTransposedResults);

        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        free(ppcResultStrings);
        free(ppcTransposedResults);

        LhsVar(1) = Rhs + 1;

        return 0;
    }
/* ==================================================================== */
} /* extern "C" */
/* ==================================================================== */
