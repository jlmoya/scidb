#pragma once
#include "QtIncludes.h"

class __declspec( dllexport ) FuzzyTableQuantifier
{
    int _tableId;
   QString _fuzzyName;
    int _fuzzyType;
    double _alpha, _beta, _gamma, _delta;

public:
    int tableId();
    QString fuzzyName();
    int fuzzyType();

    double alpha();
    double beta();
    double gamma();
    double delta();

    FuzzyTableQuantifier(QSqlQuery qry);
    FuzzyTableQuantifier(    int tableId,
                            QString fuzzyName,
                            int fuzzyType,
                            double alpha,
                            double beta,
                            double gamma,
                            double delta);
    FuzzyTableQuantifier(QMap<QString, QVariant> *mValues);
    FuzzyTableQuantifier(void);
    ~FuzzyTableQuantifier(void);

    QMap<QString, QVariant>* ValuesMap();
};
