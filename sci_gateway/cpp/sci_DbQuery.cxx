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
    int sci_DbQuery(char * fname)
    {
        QSqlDatabase *db;
        QSqlQuery *qry;
        SciErr sciErr;
        char *pcQueryString;

        CheckRhs(1,2);
        CheckLhs(0,1);

        //using default connection
        if (Rhs == 1)
        {
            db = &QSqlDatabase::database(sDefaultConnection);
            qry = new QSqlQuery(*db);

            sciGetStringAt(fname, 1, &pcQueryString);
        }
        else
        {
            getDatabaseParam(fname, 1, &db);
            qry = new QSqlQuery(*db);
            sciGetStringAt(fname, 2, &pcQueryString);
        }

        QString sQry = QString(pcQueryString);

        if(!db->isOpen())
        {
            Scierror(999, "The connection is not opened!");
            return 0;
        }

        if(!db->isValid())
        {
            Scierror(999, "The connection is not valid!");
            return 0;
        }

        qry->exec(sQry);

        sciErr = createPointer(pvApiCtx, Rhs + 1, (void*)qry);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
        }

        LhsVar(1) = Rhs + 1;

        return 0;
    }
/* ==================================================================== */
} /* extern "C" */
/* ==================================================================== */
