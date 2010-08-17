#pragma once
#include "QtIncludes.h"

class __declspec( dllexport ) FuzzyQualifier
{
	int _fuzzyId;
	double _qualifier;

public:
	int fuzzyId();
	double qualifier();

	FuzzyQualifier(QSqlQuery qry);
	FuzzyQualifier(	int fuzzyId,
					double qualifier);
	FuzzyQualifier(QMap<QString, QVariant> *mValues);
	FuzzyQualifier(void);
	~FuzzyQualifier(void);

	QMap<QString, QVariant>* ValuesMap();	
};
