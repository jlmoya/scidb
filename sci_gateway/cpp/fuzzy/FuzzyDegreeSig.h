#pragma once
#include <QtCore>

class FuzzyDegreeSig
{
	int _codeSig;
	char *_significance;

public:	
	int codeSig();
	QString significance();

	FuzzyDegreeSig(void);
	~FuzzyDegreeSig(void);
};
