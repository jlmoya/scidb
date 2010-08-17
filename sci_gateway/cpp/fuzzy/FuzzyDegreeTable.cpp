#include "FuzzyDegreeTable.h"

FuzzyDegreeTable::FuzzyDegreeTable(QSqlQuery qry)
{
	_tableId =  qry.value(qry.record().indexOf("obj")).toInt();
	_columnId =  qry.value(qry.record().indexOf("col")).toInt();
	_degreeType =  qry.value(qry.record().indexOf("degree_type")).toChar();
}

FuzzyDegreeTable::FuzzyDegreeTable(void)
{
}

FuzzyDegreeTable::~FuzzyDegreeTable(void)
{
}

int FuzzyDegreeTable::tableId(){ return _tableId; }
int FuzzyDegreeTable::columnId(){ return _columnId; }
QChar FuzzyDegreeTable::degreeType(){ return _degreeType; }