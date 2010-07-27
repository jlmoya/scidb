#include "FuzzyCol.h"

FuzzyCol::FuzzyCol(void)
{
}

FuzzyCol::~FuzzyCol(void)
{
}

int FuzzyCol::tableId()
{
	return _tableId;
}
int FuzzyCol::columnId(){ return _columnId; }
//1-8 (1-4 - attributes, 5-7 - degrees
int FuzzyCol::fType(){ return _fType; }
//for types 3-4
int FuzzyCol::len(){ return _len; }
//code of meaning of degree{}
int FuzzyCol::codeSig(){ return _codeSig; }
QString FuzzyCol::columnName(){ return _columnName; }
QString FuzzyCol::com(){ return _com; }
//unit of measurement
QString FuzzyCol::um(){ return _um; }