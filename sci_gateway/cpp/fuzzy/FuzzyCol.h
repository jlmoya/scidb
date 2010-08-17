#include <QtCore>
#include <QtSql>

class __declspec( dllexport ) FuzzyCol
{
	int _tableId;
	int _columnId;
	QString _name;
	//1-8 (1-4 - attributes, 5-7 - degrees
	int _fType;	
	//for types 3-4
	int _len;
	//code of meaning of degree;
	int _codeSig;
	QString _columnName;
	QString _com;
	//unit of measurement
	QString _um;

public:
	int tableId();
	int columnId();
	QString name();
	int fType();	
	int len();	
	int codeSig();
	QString columnName();
	QString com();	
	QString um();

	FuzzyCol(QSqlQuery qry);
	FuzzyCol(void);
	~FuzzyCol(void);
};
