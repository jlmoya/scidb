#pragma once
#include "QtIncludes.h"

class __declspec( dllexport ) FuzzyDegreeSig
{
	int _codeSig;
	QString _significance;

public:	
	int codeSig();
	QString significance();

	FuzzyDegreeSig(QSqlQuery qry);
	FuzzyDegreeSig(	int codeSig,
					QString significance);
    FuzzyDegreeSig(QMap<QString, QVariant> *mValues);
	FuzzyDegreeSig(void);
	~FuzzyDegreeSig(void);

	QMap<QString, QVariant>* ValuesMap();	
};
