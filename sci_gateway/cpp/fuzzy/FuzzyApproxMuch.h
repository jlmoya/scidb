#pragma once
#include <QtSql>

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
	FuzzyApproxMuch(void);
	~FuzzyApproxMuch(void);
};
