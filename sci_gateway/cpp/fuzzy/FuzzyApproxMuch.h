#pragma once

class FuzzyApproxMuch
{
	int _columnId;
	double _margin;
	double _much;

public:
	int columnId();
	double margin();
	double much();

	FuzzyApproxMuch(void);
	~FuzzyApproxMuch(void);
};
