#include "sci_db.h"
#include "fuzzysql.h"

int sciGetQueryParam(char *fname, int iPos, QSqlQuery **query);
int getDatabaseParam(char *fname, int iPos, QSqlDatabase **db);
int getFSQLParam(char *fname, int iPos, FuzzySQL **fsql);
int sciWriteVarIntoList(int *piList, int position, QVariant vValue);
int sciWriteMapIntoList(QMap<QString, QVariant> *mValues);
int sciStructStringFields(int *piMList, QMap<QString, QString> *map, char *fname);
int sciGetStringAt(char *fname, int iPos, char **ppcResult);
int sciGetStringInList(char *fname, int *piListAddress, int iPos, char **psResult);
int sciGetIntInList(char *fname, int *piListAddress, int iPos, int *piResult);
int sciGetIntAt(char *fname, int iPos, int *piResult);
int sciGetQSqlQueryAt(char *fname, int iPos, QSqlQuery **ppSqlQuery);
int transposeDoubleMatrix(double *pdMatr, int iRows, int iCols, double **ppdRes);
int transposeStringMatrix(char **pdMatr, int iRows, int iCols, char ***ppdRes);
int getConnectionStringMembers(QString sConnectionString, QMap<QString, QString> **mConnectionStringMembers);
int sciGetValuesMapFromStruct(char *fname, int *piListPos, QMap<QString, QVariant> *mValues);