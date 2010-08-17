#include <QtSql>

class FuzzyDegreeCol
{
	int _columnId1;
	int _columnId2;

public:
	int _columnId1();
	int _columnId2();

	FuzzyDegreeCol(QSqlQuery qry);
	FuzzyDegreeCol(void);
	~FuzzyDegreeCol(void);
};
