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
