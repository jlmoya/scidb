#include "FuzzyQualifier.h"

FuzzyQualifier::FuzzyQualifier(QSqlQuery qry)
{
	_fuzzyId = qry.value(qry.record().indexOf("fuzzy_id")).toInt();
	_qualifier = qry.value(qry.record().indexOf("qualifier")).toDouble();
}

FuzzyQualifier::FuzzyQualifier(	int fuzzyId,
					double qualifier)
{
	_fuzzyId = fuzzyId;
	_qualifier = qualifier;
}


FuzzyQualifier::FuzzyQualifier(QMap<QString, QVariant> *mValues)
{	
	if (mValues->contains("fuzzyId"))
		_fuzzyId = mValues->value("fuzzyId").toInt();

	if (mValues->contains("qualifier"))
		_qualifier = mValues->value("qualifier").toDouble();
}

FuzzyQualifier::FuzzyQualifier(void)
{
}

FuzzyQualifier::~FuzzyQualifier(void)
{
}

int FuzzyQualifier::fuzzyId() { return _fuzzyId; }
double FuzzyQualifier::qualifier() { return _qualifier; }

QMap<QString, QVariant>* FuzzyQualifier::ValuesMap()
{
	QMap<QString, QVariant> *pMap = new QMap<QString, QVariant>();

	pMap->insert("fuzzyId", QVariant(_fuzzyId));
	pMap->insert("qualifier", QVariant(_qualifier));	

	return pMap;
}