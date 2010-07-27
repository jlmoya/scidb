#pragma once
#include <QtCore>

class FuzzyTableQuantifier
{
	int _tableId;
	char *_fuzzyName;
	int _fuzzyType;
	double _alpha, _beta, _gamma, _delta;

public:
	int tableId();
	QString fuzzyName();
	int fuzzyType();
	double alpha(), beta(), gamma(), delta();

	FuzzyTableQuantifier(void);
	~FuzzyTableQuantifier(void);
};
