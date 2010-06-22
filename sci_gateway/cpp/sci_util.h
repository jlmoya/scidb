#include <string>
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

int sciWriteVarIntoList(int *piList, int position, QVariant vValue);
int sciStructStringFields(int *piMList, QMap<QString, QString> *map, char *fname);
int sciGetStringAt(char *fname, int iPos, char **ppcResult);
int sciGetQSqlQueryAt(char *fname, int iPos, QSqlQuery **ppSqlQuery);
int transposeDoubleMatrix(double *pdMatr, int iRows, int iCols, double **ppdRes);
int transposeStringMatrix(char **pdMatr, int iRows, int iCols, char ***ppdRes);