#include "FuzzyCompatibleCol.h"

FuzzyCompatibleCol::FuzzyCompatibleCol(QSqlQuery qry)
{
		_columnId1 = qry.value(qry.record().indexOf("col1")).toInt();
		_columnId2 = qry.value(qry.record().indexOf("col2")).toInt();
}

FuzzyCompatibleCol::FuzzyCompatibleCol(void)
{
}

FuzzyCompatibleCol::~FuzzyCompatibleCol(void)
{
}
int FuzzyCompatibleCol::columnId1(){ return _columnId1; }
int FuzzyCompatibleCol::columnId2(){ return _columnId2; }

