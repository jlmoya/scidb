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

#include <string>
#include <QtCore\QString.h>
#include <QtSql\qsql.h>
#include <QtSql\qsqldatabase.h>
#include <QtSql\qsqldriver.h>
#include <QtSql\qsqlquery.h>
#include <QtSql\qsqlerror.h>
#include <QtSql\qsqlfield.h>
#include <QtSql\qsqlindex.h>

#include "sci_util.h"
#include "QtIncludes.h"

extern "C"
{
    int sci_DbTest(char *fname)
    {

        //
        //WRITE STRUCT
        //
        //int *p_out_address, *piList;
        //char  *pstLabels[]   = {"st","dims","a1","a2","a3"};
        //double pdblDims[]  = {1,1};
        //double pdblA1[] = {1};
        //double pdblA2[] = {1};
        //double pdblA3[] = {1,2,3,4};

        //createList(pvApiCtx, Rhs + 1, 1, &piList);

        //SciErr err = createMListInList(pvApiCtx, Rhs + 1, piList, 1, 5, &p_out_address);

        //// Add the labels at position 1 (in the first field).
        //// We can get the labels using getfield(1,mymlist) in Scilab
        //createMatrixOfStringInList(pvApiCtx, Rhs + 1, p_out_address, 1, 1, 5, pstLabels);

        //// Add a vector to the 'dims' field
        //createMatrixOfDoubleInList(pvApiCtx, Rhs + 1, p_out_address, 2, 1, 2, pdblDims);

        //// Add the 'Ai' fields
        //createMatrixOfDoubleInList(pvApiCtx, Rhs + 1, p_out_address, 3, 1, 1, pdblA1);
        //createMatrixOfDoubleInList(pvApiCtx, Rhs + 1, p_out_address, 4, 1, 1, pdblA2);
        //createMatrixOfDoubleInList(pvApiCtx, Rhs + 1, p_out_address, 5, 1, 4, pdblA3);

        // Return the created mlist
        //LhsVar(1) = Rhs + 1;

        int *piAddress;
        QMap<QString, QVariant> mValues;

        getVarAddressFromPosition(pvApiCtx, 1, &piAddress);
        sciGetValuesMapFromStruct(fname, piAddress, &mValues);

        sciprint("Number of items: %d\n", mValues.count());
        for (int i = 0; i < mValues.count(); i++)
        {
            sciprint("Item %s: %s\n", mValues.keys().at(i).toLatin1().data(), mValues.value(mValues.keys().at(i)).toString().toLatin1().data());
        }

        return 0;
    }
}
