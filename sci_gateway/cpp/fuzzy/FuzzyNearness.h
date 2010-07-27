#pragma once

class FuzzyNearness
{
	int _fuzzyId1;
	int _fuzzyId2;
	double _degree;

public:
	int fuzzyId1();
	int fuzzyId2();
	double degree();

	FuzzyNearness(void);
	~FuzzyNearness(void);
};
