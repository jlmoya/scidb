#include "FuzzyTableInfo.h"

FuzzyTableInfo::FuzzyTableInfo(void)
{
}

FuzzyTableInfo::FuzzyTableInfo(int id, QString name)
{
		_tableId = id;
		_name = name;
}


FuzzyTableInfo::~FuzzyTableInfo(void)
{
}

int FuzzyTableInfo::tableId() { return _tableId; }

QString FuzzyTableInfo::name() { return _name; }
