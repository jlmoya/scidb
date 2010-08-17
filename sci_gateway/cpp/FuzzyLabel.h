#pragma once
#include "QtIncludes.h"

class __declspec( dllexport ) FuzzyLabel
{
    int _fuzzyId;
    double _alpha, _beta, _gamma, _delta;

public:
    int fuzzyId();
    double alpha();
    double beta();
    double gamma();
    double delta();

    FuzzyLabel(QSqlQuery qry);
    FuzzyLabel(    int fuzzyId,
                double alpha, double beta, double gamma, double delta);
    FuzzyLabel(QMap<QString, QVariant> *mValues);
    FuzzyLabel(void);
    ~FuzzyLabel(void);

    void SetValues(double alpha, double beta, double gamma, double delta);

    QMap<QString, QVariant>* ValuesMap();
};
