#pragma once
#include "QtIncludes.h"

class __declspec( dllexport ) FuzzyApproxMuch
{
	int _columnId;
	double _margin;
	double _much;

public:
	int columnId();
	double margin();
	double much();

	FuzzyApproxMuch(QSqlQuery qry);

	FuzzyApproxMuch(int columnId,
					double margin,
					double much);


	FuzzyApproxMuch(QMap<QString, QVariant> *mValues);


	FuzzyApproxMuch(void);
	~FuzzyApproxMuch(void);
	
	QMap<QString, QVariant>* ValuesMap();	
};
