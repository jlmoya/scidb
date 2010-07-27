#pragma once

class FuzzyDegreeTable
{
	int _columnId;
	char _degreeType;

public:
	int columnId();
	char degreeType();

	FuzzyDegreeTable(void);
	~FuzzyDegreeTable(void);
};
