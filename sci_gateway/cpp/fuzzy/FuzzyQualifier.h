#include <QtSql>

class __declspec( dllexport ) FuzzyQualifier
{
	int _fuzzyId;
	double _qualifier;

public:
	int fuzzyId();
	double qualifier();

	FuzzyQualifier(QSqlQuery qry);
	FuzzyQualifier(void);
	~FuzzyQualifier(void);
};
