/* ==================================================================== */
/* Igor GRIDCHYN */
/* Database toolbox */
/* This file is released into the public domain */
/* ==================================================================== */

#include <string>
#include <QtCore\QString.h>
#include <QtSql\qsql.h>
#include <QtSql\qsqldatabase.h>
#include <QtSql\qsqldriver.h>
#include <QtSql\qsqlquery.h>
#include <QtSql\qsqlerror.h>
#include <QtSql\qsqlfield.h>
#include <QtSql\qsqlindex.h>

#include "stack-c.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "MALLOC.h"
#include <cstdlib>
#include "sciprint.h"

int SciStructStringFields(int *piMList, QMap<QString, QString> *map, char *fname);