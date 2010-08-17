#include "FuzzyLabel.h"

FuzzyLabel::FuzzyLabel(QSqlQuery qry)
{
	_fuzzyId = qry.value(qry.record().indexOf("fuzzy_id")).toInt();
	_alpha = qry.value(qry.record().indexOf("alpha")).toDouble();
	_beta= qry.value(qry.record().indexOf("beta")).toDouble();
	_gamma = qry.value(qry.record().indexOf("gamma")).toDouble();
	_delta = qry.value(qry.record().indexOf("delta")).toDouble();
}

FuzzyLabel::FuzzyLabel(void)
{
}

FuzzyLabel::~FuzzyLabel(void)
{
}

int FuzzyLabel::fuzzyId(){return _fuzzyId; }
double FuzzyLabel::alpha(){ return _alpha; }
double FuzzyLabel::beta() { return _beta; }
double FuzzyLabel::gamma() { return _gamma; }
double FuzzyLabel::delta() { return _delta; }
