#pragma once
#include "QtIncludes.h"

class __declspec( dllexport ) FuzzyObject
{
    int _columnId;
    int _fuzzyId;
    QString _fuzzyName;
    int _fuzzyType;

public:
    int columnId();
    int fuzzyId();
    QString fuzzyName();
    int fuzzyType();

    FuzzyObject(QSqlQuery qry);
    FuzzyObject(int columnId,
                int fuzzyId,
                QString fuzzyName,
                int fuzzyType);
    FuzzyObject(QMap<QString, QVariant> *mValues);
    FuzzyObject(void);
    ~FuzzyObject(void);

    QMap<QString, QVariant>* ValuesMap();
};
