#pragma once
#include "QtIncludes.h"

class __declspec( dllexport ) FuzzyNearness
{
    int _fuzzyId1;
    int _fuzzyId2;
    double _degree;

public:
    int fuzzyId1();
    int fuzzyId2();
    double degree();

    FuzzyNearness(QSqlQuery qry);
    FuzzyNearness(    int fuzzyId1,
                    int fuzzyId2,
                    double degree);
    FuzzyNearness(QMap<QString, QVariant> *mValues);
    FuzzyNearness(void);
    ~FuzzyNearness(void);

    QMap<QString, QVariant>* ValuesMap();
};
