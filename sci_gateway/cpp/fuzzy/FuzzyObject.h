#pragma once
#include <QtCore>

class FuzzyObject
{
	int _columnId;
	int _fuzzyId;
	char *_fuzzyName;
	int _fuzzyType;

public:
	int columnId();
	int fuzzyId();
	QString fuzzyName();
	int fuzzyType();

	FuzzyObject(void);
	~FuzzyObject(void);
};
