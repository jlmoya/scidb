#include "FuzzyQualifier.h"

FuzzyQualifier::FuzzyQualifier(QSqlQuery qry)
{
	_fuzzyId = qry.value(qry.record().indexOf("fuzzy_id")).toInt();
	_qualifier = qry.value(qry.record().indexOf("qualifier")).toInt();
}

FuzzyQualifier::FuzzyQualifier(void)
{
}

FuzzyQualifier::~FuzzyQualifier(void)
{
}

int FuzzyQualifier::fuzzyId() { return _fuzzyId; }
double FuzzyQualifier::qualifier() { return _qualifier; }
