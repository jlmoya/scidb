#include "FuzzyApproxMuch.h"

FuzzyApproxMuch::FuzzyApproxMuch(QSqlQuery qry)
{
	_columnId = qry.value(qry.record().indexOf("col")).toInt();
	_margin = qry.value(qry.record().indexOf("margin")).toDouble();
	_much = qry.value(qry.record().indexOf("much")).toDouble();
}

FuzzyApproxMuch::FuzzyApproxMuch(int columnId,
					double margin,
					double much)
{
	_columnId = columnId;
	_margin = margin;
	_much = much;
}


FuzzyApproxMuch::FuzzyApproxMuch(QMap<QString, QVariant> *mValues)
{
	if (mValues->contains("margin"))
		_margin = mValues->value("margin").toDouble();

	if (mValues->contains("columnId"))
		_columnId = mValues->value("columnId").toInt();

	if (mValues->contains("much"))
		_much = mValues->value("much").toDouble();
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

QMap<QString, QVariant>* FuzzyApproxMuch::ValuesMap()
{
	QMap<QString, QVariant> *pMap = new QMap<QString, QVariant>();

	pMap->insert("columnId", QVariant(_columnId));
	pMap->insert("margin", QVariant(_margin));
	pMap->insert("much", QVariant(_much));	

	return pMap;
}
