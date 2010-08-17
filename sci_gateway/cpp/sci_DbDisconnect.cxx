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
    int sci_DbDisconnect(char *fname)
    {
        QSqlDatabase *db;

        CheckRhs(0,1);
        CheckLhs(0,1);

        if(Rhs==1)
        {
            getDatabaseParam(fname, 1, &db);
        }
        else
        {
            db = &QSqlDatabase::database(sDefaultConnection);
        }

        db->close();

        return 0;
    }
/* ==================================================================== */
} /* extern "C" */
/* ==================================================================== */
