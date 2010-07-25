#ifndef FUZZYSQL_H
#define FUZZYSQL_H

#include "FuzzySQL_global.h"
#include <QtSql>

class FuzzySQL {
private:
    QSqlDatabase dataBase;
public:
    FuzzySQL();
    QString FSQL2SQL(QString queryString);
    void InsertLabelTrapecic(QString name, int a, int b, int c, int d, QSqlDatabase db);
    QList<QString> GetLinguisticLabels(QSqlDatabase db);
    void CreateFuzzyMetaBaseOnDataBase(QSqlDatabase db);
    void InsertTableMetaInfo(QSqlDatabase db, QString tableName);
    void InsertColumnMetaInfo(QSqlDatabase db, QString tableName, QString columnName);
};

#endif // FUZZYSQL_H
