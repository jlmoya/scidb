#pragma once
#include "QtIncludes.h"

class __declspec( dllexport ) FuzzySystemQuantifier
{
	QString _fuzzyName;
	int _fuzzyType;
	double _alpha, _beta, _gamma, _delta;

public:

	QString fuzzyName();
	int fuzzyType();

	double alpha();
	double beta();
	double gamma();
	double delta();

	FuzzySystemQuantifier(QSqlQuery qry);
	FuzzySystemQuantifier(	QString fuzzyName,
							int fuzzyType,
							double alpha, 
							double beta, 
							double gamma, 
							double delta);
	FuzzySystemQuantifier(QMap<QString, QVariant> *mValues);
	FuzzySystemQuantifier(void);
	~FuzzySystemQuantifier(void);

	QMap<QString, QVariant>* ValuesMap();	
};
