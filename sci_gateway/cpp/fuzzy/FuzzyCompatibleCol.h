#pragma once

class FuzzyCompatibleCol
{
	int _columnId1, _columnId2;

public:
	int columnId1(), columnId2();

	FuzzyCompatibleCol(void);
	~FuzzyCompatibleCol(void);
};
