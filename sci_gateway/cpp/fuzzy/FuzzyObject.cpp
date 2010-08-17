#include "FuzzyObject.h"

FuzzyObject::FuzzyObject(QSqlQuery qry)
{
	_columnId = qry.value(qry.record().indexOf("col")).toInt();
	_fuzzyId = qry.value(qry.record().indexOf("fuzzy_id")).toInt();
	_fuzzyName  = qry.value(qry.record().indexOf("fuzzy_name")).toString();
	_fuzzyType = qry.value(qry.record().indexOf("fuzzy_type")).toInt();
}

FuzzyObject::FuzzyObject(void)
{
}

FuzzyObject::~FuzzyObject(void)
{
}

int FuzzyObject::columnId() {return _columnId; }
int FuzzyObject::fuzzyId() { return _fuzzyId; }
QString FuzzyObject::fuzzyName() { return _fuzzyName; }
int FuzzyObject::fuzzyType() { return _fuzzyType; }
