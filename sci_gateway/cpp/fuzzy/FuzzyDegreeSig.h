#pragma once
#include <QtCore>
#include <QtSql>

class __declspec( dllexport ) FuzzyDegreeSig
{
	int _codeSig;
	QString _significance;

public:	
	int codeSig();
	QString significance();

	FuzzyDegreeSig(QSqlQuery qry);
	FuzzyDegreeSig(void);
	~FuzzyDegreeSig(void);
};
