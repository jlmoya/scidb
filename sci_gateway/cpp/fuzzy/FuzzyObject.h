#include <QtCore>
#include <QtSql>

class __declspec( dllexport ) FuzzyObject
{
	int _columnId;
	int _fuzzyId;
	QString _fuzzyName;
	int _fuzzyType;

public:
	int columnId();
	int fuzzyId();
	QString fuzzyName();
	int fuzzyType();

	FuzzyObject(QSqlQuery qry);
	FuzzyObject(void);
	~FuzzyObject(void);
};
