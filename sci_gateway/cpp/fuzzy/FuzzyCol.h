#pragma once
#include <QtCore>

class FuzzyCol
{
	int _tableId;
	int _columnId;
	//1-8 (1-4 - attributes, 5-7 - degrees
	int _fType;	
	//for types 3-4
	int _len;
	//code of meaning of degree;
	int _codeSig;
	char *_columnName;
	char *_com;
	//unit of measurement
	char *_um;

public:
	int tableId();
	int columnId();
	//1-8 (1-4 - attributes, 5-7 - degrees
	int fType();
	//for types 3-4
	int len();
	//code of meaning of degree;
	int codeSig();
	QString columnName();
	QString com();
	//unit of measurement
	QString um();

	FuzzyCol(void);
	~FuzzyCol(void);
};
