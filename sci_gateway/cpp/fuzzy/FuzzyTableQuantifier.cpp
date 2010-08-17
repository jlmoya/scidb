#include "FuzzyTableQuantifier.h"

FuzzyTableQuantifier::FuzzyTableQuantifier(QSqlQuery qry)
{
	_tableId = qry.value(qry.record().indexOf("obj")).toInt();
	_fuzzyName = qry.value(qry.record().indexOf("fuzzy_name")).toString();
	_fuzzyType = qry.value(qry.record().indexOf("fuzzy_type")).toInt();
	_alpha = qry.value(qry.record().indexOf("alpha")).toDouble();
	_beta= qry.value(qry.record().indexOf("beta")).toDouble();
	_gamma = qry.value(qry.record().indexOf("gamma")).toDouble();
	_delta = qry.value(qry.record().indexOf("delta")).toDouble();
}

FuzzyTableQuantifier::FuzzyTableQuantifier(void)
{
}

FuzzyTableQuantifier::~FuzzyTableQuantifier(void)
{
}

int FuzzyTableQuantifier::tableId() { return _tableId; }
QString FuzzyTableQuantifier::fuzzyName() { return _fuzzyName; }
int FuzzyTableQuantifier::fuzzyType() { return _fuzzyType; }
double FuzzyTableQuantifier::alpha() { return _alpha; }
double FuzzyTableQuantifier::beta() { return _beta; }
double FuzzyTableQuantifier::gamma() { return _gamma; }
double FuzzyTableQuantifier::delta() { return _delta; }