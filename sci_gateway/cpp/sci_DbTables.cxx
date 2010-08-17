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
    int sci_DbTables(char *fname)
    {
        SciErr sciErr;
        QSqlDatabase *db;
        QStringList slTables;

        CheckRhs(0,1);
        CheckLhs(0,1);

        if(Rhs == 0)
        {
            db = &(QSqlDatabase::database(sDefaultConnection));
        }
        else
        {
            getDatabaseParam(fname, 1, &db);
        }

        slTables = db->tables();

        char **psTablesList = (char**)malloc(sizeof(char*)*slTables.count());

        for(int i=0; i<slTables.count();i++)
        {
            psTablesList[i] = (char*)malloc(sizeof(char)*slTables.at(i).length());
            strcpy(psTablesList[i], slTables.at(i).toLatin1().data());
        }

        sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, 1, slTables.count(), psTablesList);
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
