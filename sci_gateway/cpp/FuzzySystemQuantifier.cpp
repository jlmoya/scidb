#include "FuzzySystemQuantifier.h"

FuzzySystemQuantifier::FuzzySystemQuantifier(QSqlQuery qry)
{
    _fuzzyName = qry.value(qry.record().indexOf("fuzzy_name")).toString();
    _fuzzyType = qry.value(qry.record().indexOf("fuzzy_type")).toInt();
    _alpha = qry.value(qry.record().indexOf("alpha")).toDouble();
    _beta= qry.value(qry.record().indexOf("beta")).toDouble();
    _gamma = qry.value(qry.record().indexOf("gamma")).toDouble();
    _delta = qry.value(qry.record().indexOf("delta")).toDouble();
}

FuzzySystemQuantifier::FuzzySystemQuantifier(    QString fuzzyName,
                            int fuzzyType,
                            double alpha,
                            double beta,
                            double gamma,
                            double delta)
{
    _fuzzyName = fuzzyName;
    _fuzzyType = fuzzyType;
    _alpha = alpha;
    _beta = beta;
    _gamma = gamma;
    _delta = delta;
}
FuzzySystemQuantifier::FuzzySystemQuantifier(QMap<QString, QVariant> *mValues)
{
    if (mValues->contains("fuzzyName"))
        _fuzzyName = mValues->value("fuzzyName").toString();
    if (mValues->contains("fuzzyType"))
        _fuzzyType = mValues->value("fuzzyType").toInt();
    if (mValues->contains("alpha"))
        _alpha = mValues->value("alpha").toDouble();
    if (mValues->contains("beta"))
        _beta = mValues->value("beta").toDouble();
    if (mValues->contains("gamma"))
        _gamma = mValues->value("gamma").toDouble();
    if (mValues->contains("delta"))
        _delta = mValues->value("delta").toDouble();
}
FuzzySystemQuantifier::FuzzySystemQuantifier(void)
{
}

FuzzySystemQuantifier::~FuzzySystemQuantifier(void)
{
}

QString FuzzySystemQuantifier::fuzzyName() { return _fuzzyName; }
int FuzzySystemQuantifier::fuzzyType() { return _fuzzyType; }
double FuzzySystemQuantifier::alpha() { return _alpha; }
double FuzzySystemQuantifier::beta() { return _beta; }
double FuzzySystemQuantifier::gamma() { return _gamma; }
double FuzzySystemQuantifier::delta() { return _delta; }

QMap<QString, QVariant>* FuzzySystemQuantifier::ValuesMap()
{
    QMap<QString, QVariant> *pMap = new QMap<QString, QVariant>();

    pMap->insert("fuzzyName", QVariant(_fuzzyName));
    pMap->insert("fuzzyType", QVariant(_fuzzyType));
    pMap->insert("alpha", QVariant(_alpha));
    pMap->insert("beta", QVariant(_beta));
    pMap->insert("gamma", QVariant(_gamma));
    pMap->insert("delta", QVariant(_delta));

    return pMap;
}
