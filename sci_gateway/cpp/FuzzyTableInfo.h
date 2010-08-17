#pragma once
#include "QtIncludes.h"

class __declspec( dllexport ) FuzzyTableInfo
{
    int _tableId;
    QString _name;

public:
    int tableId();
    QString name();

    FuzzyTableInfo(void);
    FuzzyTableInfo(int id, QString name);
    FuzzyTableInfo(QMap<QString, QVariant> *mValues);

    ~FuzzyTableInfo(void);

    QMap<QString, QVariant>* ValuesMap();
};
