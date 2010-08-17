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
    int sci_DbFetchAllStruct(char *fname)
    {
        QSqlQuery *psqQuery;

        CheckRhs(1,1);
        CheckLhs(0,1);

        if(sciGetQSqlQueryAt(fname, 1, &psqQuery) > 0)
        {
            return 0;
        }

        if(!psqQuery->isActive())
        {
            Scierror(999, "Given query was not successfully executed.\n");
            return 0;
        }

        char **pstLabels;
        double pdblDims[]  = {1,1};

        QSqlRecord rec;
        int iFieldsNumber;

        if(psqQuery->isValid() || psqQuery->next())
        {
            rec = psqQuery-> record();

            iFieldsNumber = rec.count();

            pstLabels = (char**)malloc(sizeof(char*)*(iFieldsNumber + 2));
            pstLabels[0] = "st";
            pstLabels[1] = "dims";

            for(int j=0; j<iFieldsNumber; j++)
            {
                QString sFieldName = rec.field(j).name();

                pstLabels[j+2] = (char*)malloc(sizeof(char)*sFieldName.length());
                strcpy(pstLabels[j+2], sFieldName.toLatin1().data());
            }
        }
        else
        {
            Scierror(999, "No results in query.\n");
            return 0;
        }

        QList< QList<QVariant> > llvRecords = QList< QList<QVariant> >();

        do
        {
            rec = psqQuery-> record();

            QList<QVariant> lvRecord = QList<QVariant>();

            for(int i=0; i<rec.count(); i++)
            {
                lvRecord.append(rec.value(i));
            }

            llvRecords.append(lvRecord);
        }
        while(psqQuery->next());

        int *piList, *piStruct;

        createList(pvApiCtx, Rhs + 1, llvRecords.size(), &piList);

        for (int i = 0; i < llvRecords.size(); ++i)
        {
            SciErr err = createMListInList(pvApiCtx, Rhs + 1, piList, i + 1, iFieldsNumber + 2, &piStruct);
            createMatrixOfStringInList(pvApiCtx, Rhs + 1, piStruct, 1, 1, iFieldsNumber + 2, pstLabels);
            createMatrixOfDoubleInList(pvApiCtx, Rhs + 1, piStruct, 2, 1, 2, pdblDims);

            for (int j = 0; j < iFieldsNumber; j++)
            {
                sciWriteVarIntoList(piStruct, j+3, llvRecords.at(i).at(j));
            }
        }

        LhsVar(1) = Rhs + 1;

        return 0;
    }
/* ==================================================================== */
} /* extern "C" */
/* ==================================================================== */
