#include "FuzzyApproxMuch.h"

FuzzyApproxMuch::FuzzyApproxMuch(QSqlQuery qry)
{
	_columnId = qry.value(qry.record().indexOf("col")).toInt();
	_margin = qry.value(qry.record().indexOf("margin")).toDouble();
	_much = qry.value(qry.record().indexOf("much")).toDouble();
}

FuzzyApproxMuch::FuzzyApproxMuch(void)
{
}

FuzzyApproxMuch::~FuzzyApproxMuch(void)
{
}

int FuzzyApproxMuch::columnId()
{
	return _columnId;
}

double FuzzyApproxMuch::margin()
{
	return _margin;
}

double FuzzyApproxMuch::much()
{
	return _much;
}
