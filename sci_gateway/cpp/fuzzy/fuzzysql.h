//#ifndef FUZZYSQL_H
//#define FUZZYSQL_H

//#include "FuzzySQL_global.h"
#include <Qt/qglobal.h>
#include <QtSql>

#include "FMB.h" 

class __declspec( dllexport ) FuzzySQL {
private:
    QSqlDatabase *_dataBase;
	FMB _fmb;
public:
    FuzzySQL();
	FuzzySQL(QSqlDatabase *db);	

    QString FSQL2SQL(QString queryString);

    void CreateFuzzyMetaBaseOnDataBase(QSqlDatabase *db);
    void CreateTableMetaInfo(QSqlDatabase *db, QString tableName);
    void CreateColumnMetaInfo(QSqlDatabase *db, QString tableName, QString columnName);
	
	//returns a string representing a select query for column id by it's name and table		
	QString FuzzySQL::ColumnIdQuery(QString table, QString columnName);

	QString FuzzySQL::TableIdQuery(QString tableName);

	void FuzzySQL::CreateFuzzyCol(QString column, int f_type, int len, int code_sig, 
							  QString columnName, QString com, QString um);
	void FuzzySQL::CreateFuzzyDegreeSig(int code_sig, int significance);
	void FuzzySQL::CreateFuzzyObject(QString table, QString column, QString fuzzy_name, int fuzzy_type);
	void FuzzySQL::CreateFuzzyLabelDef(QString table, QString column, int fuzzy_id, 
		double alpha, double beta, double gamma, double delta);
	void FuzzySQL::CreateFuzzyApproxMuch(QString table, QString column, double margin, double much);
	void FuzzySQL::CreateFuzzyNearness(QString table, QString column, int fuzzy_id1, int fuzzy_id2,
		double degree);
	void FuzzySQL::CreateFuzzyCompatibleCol(QString table1, QString col1, QString table2, QString col2);
	void FuzzySQL::CreateFuzzyQualifier(QString table, QString column, int fuzzy_id, double qualifier);
	void FuzzySQL::CreateFuzzyDegreeCol(QString table1, QString col1, QString table2, QString col2);
	void FuzzySQL::CreateFuzzyDegreeTable(QString table, QString col, char degree_type);
	void FuzzySQL::CreateFuzzyTableQuantifier(QString table, QString fuzzy_name, int fuzzy_type, 
		double alpha, double beta, double gamma, double delta);
	void FuzzySQL::CreateFuzzySystemQuantifier(QString fuzzy_name, int fuzzy_type, double alpha,
		double beta, double gamma, double delta);

	//============================================================//
	int FuzzySQL::OperandType(QString operand);
};

//#endif // FUZZYSQL_H
