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
