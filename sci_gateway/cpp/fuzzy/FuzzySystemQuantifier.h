#pragma once
#include <QtCore>

class FuzzySystemQuantifier
{
	char *_fuzzyName;
	int _fuzzyType;
	double _alpha, _beta, _gamma, _delta;

public:

	QString fuzzyName();
	int fuzzyType();
	double alpha(), beta(), gamma(), delta();

	FuzzySystemQuantifier(void);
	~FuzzySystemQuantifier(void);
};
