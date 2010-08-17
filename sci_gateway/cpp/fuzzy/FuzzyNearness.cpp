#include "FuzzyNearness.h"

FuzzyNearness::FuzzyNearness(QSqlQuery qry)
{
	_fuzzyId1 = qry.value(qry.record().indexOf("fuzzy_id1")).toInt();
	_fuzzyId2 = qry.value(qry.record().indexOf("fuzzy_id2")).toInt();
	_degree = qry.value(qry.record().indexOf("degree")).toDouble();
}


FuzzyNearness::FuzzyNearness(void)
{
}

FuzzyNearness::~FuzzyNearness(void)
{
}

int FuzzyNearness::fuzzyId1() { return _fuzzyId1; }
int FuzzyNearness::fuzzyId2() { return _fuzzyId2; }
double FuzzyNearness::degree() { return _degree; }