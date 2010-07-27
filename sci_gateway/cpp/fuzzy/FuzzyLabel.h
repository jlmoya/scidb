#pragma once

class FuzzyLabel
{
	int _fuzzyId;
	double _alpha, _beta, _gamma, _delta;

public:
	int fuzzyId();
	double alpha(), beta(), gamma(), delta();

	FuzzyLabel(void);
	~FuzzyLabel(void);
};
