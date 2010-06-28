#include "sci_db.h"

int sciGetQueryParam(char *fname, int iPos, QSqlQuery **query);
int getDatabaseParam(char *fname, int iPos, QSqlDatabase **db);
int sciWriteVarIntoList(int *piList, int position, QVariant vValue);
int sciStructStringFields(int *piMList, QMap<QString, QString> *map, char *fname);
int sciGetStringAt(char *fname, int iPos, char **ppcResult);
int sciGetQSqlQueryAt(char *fname, int iPos, QSqlQuery **ppSqlQuery);
int transposeDoubleMatrix(double *pdMatr, int iRows, int iCols, double **ppdRes);
int transposeStringMatrix(char **pdMatr, int iRows, int iCols, char ***ppdRes);
