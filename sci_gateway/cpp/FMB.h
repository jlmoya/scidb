#pragma once

#include "QtIncludes.h"

#include "FuzzyCol.h"
#include "FuzzyTableInfo.h"
#include "FuzzyDegreeSig.h"
#include "FuzzyDegreeCol.h"
#include "FuzzyObject.h"
#include "FuzzyLabel.h"
#include "FuzzyApproxMuch.h"
#include "FuzzyNearness.h"
#include "FuzzyCompatibleCol.h"
#include "FuzzyQualifier.h"
#include "FuzzyDegreeTable.h"
#include "FuzzyTableQuantifier.h"
#include "FuzzySystemQuantifier.h"

class __declspec( dllexport ) FMB 
{
public:
	QMap<QString, FuzzyTableInfo*> _mFuzzyTablesByName;
	QMap<int, FuzzyTableInfo*> _mFuzzyTables;
	
	QMap<int, FuzzyCol*> _mFuzzyColumns;
	QMap<int, FuzzyDegreeSig*> _mFuzzyDegreeSig;
	QMap<int, FuzzyObject*> _mFuzzyObjects;
	QMap<int, FuzzyLabel*> _mFuzzyLabels;
	QMap<int, FuzzyApproxMuch*> _mFuzzyApproxMuch;
	QMap<int, QMap<int, FuzzyNearness*>*> _mFuzzyNearness;
	QMap<int, QList<int>*> _mFuzzyCompatibleColumnAdapters;
	QMap<int, QList<int>*> _mFuzzyCompatibleColumnAdaptees;
	QMap<int, FuzzyQualifier*> _mFuzzyQualifiers;
	QMap<int, int> _mFuzzyDegreeCols;	
	QMap<int, FuzzyDegreeTable*> _mFuzzyDegreeTables;
	QMap<int, QMap<QString, FuzzyTableQuantifier*>*> _mFuzzyTableQuantifiers;
	QMap<QString, FuzzySystemQuantifier*> _mFuzzySystemQuantifiers;

	QMap<QString, FuzzyLabel*> _mFuzzyLabelsByName;
	//full name like [Table.Column] is used for keys in this map
	QMap<QString, FuzzyCol*> _mFuzzyColumnsByName;
	QMap<QString, FuzzyObject*> _mFuzzyObjectsByName;

	QMap<int, QMap<QString, FuzzyObject*>*> _mFuzzyLabelsByColumn;
			
	QSqlDatabase *_db;

	///loadsinfo from tables and fills collections
	void LoadFMB();
	void SetDb(QSqlDatabase *pDb);	

	int DeleteFuzzyTableInfo(QString name);
	int DeleteFuzzyTableInfo(int tableId);
	int DeleteFuzzyColumn(int columnId);
	int DeleteFuzzyDegreeSig(int code_sig);
	int DeleteFuzzyObject(int fuzzy_id);
	int DeleteFuzzyLabel(int fuzzy_id);
	int DeleteFuzzyApproxMuch(int columnId);
	int DeleteFuzzyNearness(int fuzzyId1, int fuzzyId2);
	int DeleteFuzzyCompatibleCol(int adapteeId, int adapterId);
	int DeleteFuzzyQualifier(int fuzzyId);
	int DeleteFuzzyDegreeCol(int columnId);
	int DeleteFuzzyDegreeTable(int tableId);
	int DeleteFuzzyTableQuantifier(int tableId, QString fuzzyName);
	int DeleteFuzzySystemQuantifier(QString fuzzy_name);
	
	int UpdateFuzzyTableInfo(FuzzyTableInfo fTInfo);
	int UpdateFuzzyColumn(FuzzyCol col);
	int UpdateFuzzyDegreeSig(FuzzyDegreeSig fDegSig);
	int UpdateFuzzyObject(FuzzyObject fObj);
	int UpdateFuzzyLabel(FuzzyLabel fLabel);
	int UpdateFuzzyApproxMuch(FuzzyApproxMuch fApprox);
	int UpdateFuzzyNearness(FuzzyNearness fNns);	
	int UpdateFuzzyQualifier(FuzzyQualifier fQualif);		
	int UpdateFuzzyTableQuantifier(FuzzyTableQuantifier fTableQuantif);
	int UpdateFuzzySystemQuantifier(FuzzySystemQuantifier fSystemQuantif);	

	//returns a string representing a select query for column id by it's name and table		
	QString ColumnIdQuery(QString table, QString columnName);
	QString TableIdQuery(QString tableName);

	void CreateTableMetaInfo(QString tableName);  
	void CreateFuzzyCol(int tableId, int f_type, int len, int code_sig, 
							  QString columnName, QString com, QString um);
	void CreateFuzzyDegreeSig(int code_sig, QString significance);
	void CreateFuzzyObject(int columnId, QString fuzzy_name, int fuzzy_type);
	void CreateFuzzyLabelDef(int fuzzy_id, 
		double alpha, double beta, double gamma, double delta);
	void CreateFuzzyApproxMuch(int columnId, double margin, double much);
	void CreateFuzzyNearness(int fuzzy_id1, int fuzzy_id2,
		double degree);
	void CreateFuzzyCompatibleCol(int col1, int col2);
	void CreateFuzzyQualifier(int fuzzy_id, double qualifier);
	void CreateFuzzyDegreeCol(int col1, int col2);
	void CreateFuzzyDegreeTable(int tableId, int columnId, QChar degree_type);
	void CreateFuzzyTableQuantifier(int tableId, QString fuzzy_name, int fuzzy_type, 
		double alpha, double beta, double gamma, double delta);
	void CreateFuzzySystemQuantifier(QString fuzzy_name, int fuzzy_type, double alpha,
		double beta, double gamma, double delta);

	FuzzyTableInfo* GetFuzzyTableInfo(QString tableName);
	FuzzyTableInfo* GetFuzzyTableInfo(int tableId)	;
	FuzzyCol* GetFuzzyCol(int columnId);	
	FuzzyCol* GetFuzzyCol(QString columnName);	
	FuzzyDegreeSig* GetFuzzyDegreeSig(int codeSig);
	FuzzyObject* GetFuzzyObject(int fuzzyId);	
	FuzzyObject* GetFuzzyObject(QString fuzzyName);	
	FuzzyLabel* GetFuzzyLabel(int fuzzyId);
	FuzzyLabel* GetFuzzyLabel(QString fuzzy_name);
	FuzzyApproxMuch* GetFuzzyApproxMuch(int columnId);
	FuzzyNearness* GetFuzzyNearness(int fuzzyId1, int fuzzyId2);
	QList<int>* GetFuzzyCompatibleCols(int adapteeId);
	FuzzyQualifier* GetFuzzyQualifier(int fuzzyId);
	int GetFuzzyDegreeCol(int columnId);
	FuzzyDegreeTable* GetFuzzyDegreeTable(int tableId);
	FuzzyTableQuantifier* GetFuzzyTableQuantifier(int tableId, QString fuzzyName);
	FuzzySystemQuantifier* GetFuzzySystemQuantifier(QString fuzzyName);

	FMB(void);
	~FMB(void);
};
