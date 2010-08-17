#include "FuzzyCol.h"

FuzzyCol::FuzzyCol(QSqlQuery qry)
{
	_tableId = qry.value(qry.record().indexOf("obj")).toInt();
	_columnId = qry.value(qry.record().indexOf("col")).toInt();
	_name = qry.value(qry.record().indexOf("name")).toString();
	_fType = qry.value(qry.record().indexOf("f_type")).toInt();
	 _len = qry.value(qry.record().indexOf("len")).toInt();
	_codeSig  = qry.value(qry.record().indexOf("code_sig")).toInt();
	_columnName = qry.value(qry.record().indexOf("column_name")).toString();
	_com = qry.value(qry.record().indexOf("com")).toString();	
	_um = qry.value(qry.record().indexOf("um")).toString();
}

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
QString FuzzyCol::name(){ return _name; }
int FuzzyCol::fType(){ return _fType; }
int FuzzyCol::len(){ return _len; }
int FuzzyCol::codeSig(){ return _codeSig; }
QString FuzzyCol::columnName(){ return _columnName; }
QString FuzzyCol::com(){ return _com; }
QString FuzzyCol::um(){ return _um; }