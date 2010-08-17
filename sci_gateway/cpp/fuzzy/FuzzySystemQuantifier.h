#include <QtCore>
#include <QtSql>

class __declspec( dllexport ) FuzzySystemQuantifier
{
	QString _fuzzyName;
	int _fuzzyType;
	double _alpha, _beta, _gamma, _delta;

public:

	QString fuzzyName();
	int fuzzyType();

	double alpha();
	double beta();
	double gamma();
	double delta();

	FuzzySystemQuantifier(QSqlQuery qry);
	FuzzySystemQuantifier(void);
	~FuzzySystemQuantifier(void);
};
