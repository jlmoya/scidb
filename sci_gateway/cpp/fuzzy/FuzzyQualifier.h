#pragma once

class FuzzyQualifier
{
	int _fuzzyId;
	double _qualifier;

public:
	int fuzzyId();
	double qualifier();

	FuzzyQualifier(void);
	~FuzzyQualifier(void);
};
