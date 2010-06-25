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
