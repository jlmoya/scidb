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

extern QString sDefaultConnection;

extern QMap<QString,QList<QString>> mslsProviderConnectionOptions;

extern QList<QString> lsCommonConnectionParameters;

extern QList<QString> lsProviders;

extern "C"
{
    int sci_DbFuzzyQuery(char *fname)
    {
        FuzzySQL *fsql;
        QSqlQuery *qry;
        SciErr sciErr;
        char *pcQueryString;

        CheckRhs(2,2);
        CheckLhs(0,1);

        //using default connection

        getFSQLParam(fname, 1, &fsql);
        qry = new QSqlQuery(*fsql->DataBase());

        sciGetStringAt(fname, 2, &pcQueryString);

        QString sTranslatorError("");
        QString sTranslatedQuery = fsql->FSQL2SQL(pcQueryString, &sTranslatorError);
        if (sTranslatorError != "")
        {
            Scierror(999, sTranslatorError.toLatin1().data());
            return 0;
        }

        QString sQry = QString(sTranslatedQuery);

        if(!fsql->DataBase()->isOpen())
        {
            Scierror(999, "The connection is not opened!");
            return 0;
        }

        if(!fsql->DataBase()->isValid())
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
