#pragma once
#include "QtIncludes.h"

class __declspec( dllexport ) FuzzyCompatibleCol
{
    int _columnId1, _columnId2;

public:
    int columnId1();
    int columnId2();

    FuzzyCompatibleCol(QSqlQuery qry);
    FuzzyCompatibleCol(int columnId1, int columnId2);
    FuzzyCompatibleCol(QMap<QString, QVariant> *mValues);
    FuzzyCompatibleCol(void);
    ~FuzzyCompatibleCol(void);

    QMap<QString, QVariant>* ValuesMap();
};
