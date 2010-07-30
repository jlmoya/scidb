#pragma once

#include <QtCore>
#include <QtSql>

#include "FuzzyCol.h"
#include "FuzzyTableInfo.h"
#include "FuzzyDegreeSig.h"
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
	QMap<QString, FuzzyTableInfo> _mFuzzyTables;
	
	QMap<int, FuzzyCol> _mFuzzyColumns;
	QMap<int, FuzzyDegreeSig> _mFuzzyDegreeSig;
	QMap<int, FuzzyObject> _mFuzzyObjects;
	QMap<int, FuzzyLabel> _mFuzzyLabels;
	QMap<int, FuzzyApproxMuch> _mFuzzyApproxMuch;
	QMap<int, QMap<int, FuzzyNearness>*> _mFuzzyNearness;
	QMap<int, QList<int>*> _mFuzzyCompatibleColumnAdapters;
	QMap<int, QList<int>*> _mFuzzyCompatibleColumnAdaptees;
	QMap<int, FuzzyQualifier> _mFuzzyQualifiers;
	QMap<int, int> _mFuzzyDegreeCols;	
	QMap<int, FuzzyDegreeTable> _mFuzzyDegreeTables;
	QMap<int, QMap<QString, FuzzyTableQuantifier>*> _mFuzzyTableQuantifiers;
	QMap<QString, FuzzySystemQuantifier> _mFuzzySystemQuantifiers;

	QMap<QString, FuzzyLabel> _mFuzzyLabelsByName;
	QMap<QString, FuzzyCol> _mFuzzyColumnsByName;

	QSqlDatabase *_db;

public:
	///loadsinfo from tables and fills collections
	void LoadFMB();
	void SetDb(QSqlDatabase *pDb);	

	int DeleteFuzzyTableInfo(QString name);
	int DeleteFuzzyColumn(int columnId);
	int DeleteFuzzyDegreeSig(int code_sig);
	int DeleteFuzzyObject(int fuzzy_id);
	int DeleteFuzzyLabel(int fuzzy_id);
	int DeleteFuzzyApproxMuch(int columnId);
	int DeleteFuzzyNearness(int fuzzyId1, int fuzzyId2);
	int DeleteFuzzyCompatibleCol(int adapteeId, int adapterId);
	int DeleteFuzzyQualifier(int fuzzyId);
	int DeleteFuzyDegreeCol(int columnId);
	int DeleteFuzzyDegreeTable(int tableId);
	int DeleteFuzzyTableQuantifier(int tableId, QString fuzzyName);
	int DeleteFuzzySystemQuantifier(QString fuzzy_name);
	
	FuzzyTableInfo GetFuzzyTableInfo(QString tableName);
	FuzzyCol GetFuzzyCol(int columnId);
	FuzzyDegreeSig GetFuzzyDegreeSig(int codeSig);
	FuzzyObject GetFuzzyObject(int fuzzyId);
	FuzzyLabel GetFuzzyLabel(int fuzzyId);
	FuzzyLabel GetFuzzyLabel(QString fuzzy_name);
	FuzzyApproxMuch GetFuzzyApproxMuch(int columnId);
	FuzzyNearness GetFuzzyNearness(int fuzzyId1, int fuzzyId2);
	QList<int>* GetFuzzyCompatibleCols(int adapteeId);
	FuzzyQualifier GetFuzzyQualifier(int fuzzyId);
	int GetFuzzyDegreeCol(int columnId);
	FuzzyDegreeTable GetFuzzyDegreeTable(int tableId);
	FuzzyTableQuantifier GetFuzzyTableQuantifier(int tableId, QString fuzzyName);
	FuzzySystemQuantifier GetFuzzySystemQuantifier(QString fuzzyName);

	FMB(void);
	~FMB(void);
};
