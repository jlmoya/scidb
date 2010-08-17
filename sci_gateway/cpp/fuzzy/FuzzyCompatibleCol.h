#include <QtSql>

class __declspec( dllexport ) FuzzyCompatibleCol
{
	int _columnId1, _columnId2;

public:
	int columnId1();
	int columnId2();

	FuzzyCompatibleCol(QSqlQuery qry);
	FuzzyCompatibleCol(void);
	~FuzzyCompatibleCol(void);
};
