//#ifndef FUZZYSQL_H
//#define FUZZYSQL_H

#include "TreeTokenNode.h"
#include "FuzzyConstant.h"

#include "QtIncludes.h"

#include "FMB.h" 

class __declspec( dllexport ) FuzzySQL {
private:
    QSqlDatabase *_dataBase;
	FMB _fmb;
public:
	QSqlDatabase * DataBase();
	FMB *FuzzyMetaBase();

    FuzzySQL();
	FuzzySQL(QSqlDatabase *db);	

    QString FSQL2SQL(QString queryString, QString *error);

    void CreateFuzzyMetaBaseOnDataBase(QSqlDatabase *db);
	void CreateFuzzyMetaBaseOnDataBase();


	






	//============================================================//
	int FuzzySQL::OperandType(QString operand);
	QList<QString> FuzzySQL::CallParameters(QString sArgument, int iArgType, QString marginAttribute);
	QString FuzzySQL::GetFullColumnName(QString operand, QString query, int iOperandStartIndex);
};

//#endif // FUZZYSQL_H
