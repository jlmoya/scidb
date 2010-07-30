#pragma once
#include <QtSql>

class __declspec( dllexport ) FuzzyLabel
{
	int _fuzzyId;
	double _alpha, _beta, _gamma, _delta;

public:
	int fuzzyId();
	double alpha();
	double beta();
	double gamma();
	double delta();

	FuzzyLabel(QSqlQuery qry);
	FuzzyLabel(void);
	~FuzzyLabel(void);
};
