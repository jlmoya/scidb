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
    int sci_DbFetchStruct(char *fname)
    {
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

        char **pstLabels = (char**)malloc(sizeof(char*)*(rec.count() + 2));
        pstLabels[0] = "st";
        pstLabels[1] = "dims";
        double pdblDims[]  = {1,1};

        for(int j=0; j<rec.count();j++)
        {
            QString sFieldName = rec.field(j).name();

            pstLabels[j+2] = (char*)malloc(sizeof(char)*sFieldName.length());
            strcpy(pstLabels[j+2], sFieldName.toLatin1().data());
        }

        int *piStructAddress;

        createMList(pvApiCtx, Rhs + 1, rec.count() + 2, &piStructAddress);
        createMatrixOfStringInList(pvApiCtx, Rhs + 1, piStructAddress, 1, 1, rec.count() + 2, pstLabels);
        createMatrixOfDoubleInList(pvApiCtx, Rhs + 1, piStructAddress, 2, 1, 2, pdblDims);

        for(int i=0; i < rec.count(); i++)
        {
            sciWriteVarIntoList(piStructAddress, i+3, rec.value(i));
        }

        psqQuery->next();

        LhsVar(1) = Rhs + 1;

        return 0;
    }
/* ==================================================================== */
} /* extern "C" */
/* ==================================================================== */
