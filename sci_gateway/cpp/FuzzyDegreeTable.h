#pragma once
#include "QtIncludes.h"

class __declspec( dllexport ) FuzzyDegreeTable
{
    int _tableId;
    int _columnId;
    QChar _degreeType;

public:
    int tableId();
    int columnId();
    QChar degreeType();

    FuzzyDegreeTable(QSqlQuery qry);
    FuzzyDegreeTable(    int tableId,
                        int columnId,
                        QChar degreeType);
    FuzzyDegreeTable(QMap<QString, QVariant> *mValues);
    FuzzyDegreeTable(void);
    ~FuzzyDegreeTable(void);

    QMap<QString, QVariant>* ValuesMap();
};
