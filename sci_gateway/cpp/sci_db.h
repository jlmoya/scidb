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

#include <QtCore/qstring.h>
#include <QtSql/qsql.h>
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqldriver.h>
#include <QtSql/qsqlquery.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlfield.h>
#include <QtSql/qsqlindex.h>

#include "stack-c.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "MALLOC.h"
#include <cstdlib>
#include "sciprint.h"

extern QString sDefaultConnection;

extern QMap< QString,QList<QString> > mslsProviderConnectionOptions;

extern QList<QString> lsCommonConnectionParameters;

extern QList<QString> lsProviders;