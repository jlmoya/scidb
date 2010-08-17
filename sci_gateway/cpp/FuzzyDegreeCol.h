#include "QtIncludes.h"

class FuzzyDegreeCol
{
	int _columnId1;
	int _columnId2;

public:
	int columnId1();
	int columnId2();

	FuzzyDegreeCol(QSqlQuery qry);
	FuzzyDegreeCol(int columnId1, int columnId2);
    FuzzyDegreeCol(QMap<QString, QVariant> *mValues);
	FuzzyDegreeCol(void);
	~FuzzyDegreeCol(void);
};
