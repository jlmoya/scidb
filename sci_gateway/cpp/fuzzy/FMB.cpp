#include "FMB.h"

void FMB::LoadFMB()
{
	QSqlQuery qry = QSqlQuery("SELECT * FROM fuzzy_meta_tables", *_db);	
	while(qry.next())
	{
		FuzzyTableInfo tableInfo = FuzzyTableInfo( qry.value(0).toInt(),
			qry.value(1).toString());				

		_mFuzzyTables.insert(qry.value(1).toString(), tableInfo);

		printf("%s", qry.value(1).toString().toLatin1().data());
	}

	qry = QSqlQuery("SELECT * FROM fuzzy_col_list", *_db);	
	while(qry.next())
	{
		FuzzyCol fuzzyColumn(qry);		

		_mFuzzyColumns.insert(fuzzyColumn.columnId(), fuzzyColumn);
		_mFuzzyColumnsByName.insert(fuzzyColumn.name(), fuzzyColumn);
	}

	qry = QSqlQuery("SELECT * FROM fuzzy_degree_sig", *_db);
	while(qry.next())
	{
		FuzzyDegreeSig fDegrSig(qry);

		_mFuzzyDegreeSig.insert(fDegrSig.codeSig(), fDegrSig);
	}

	qry.exec("SELECT * from fuzzy_object_list");
	while(qry.next())
	{
		FuzzyObject fObj(qry);

		_mFuzzyObjects.insert(fObj.fuzzyId(), fObj);
	}

	qry.exec("SELECT * FROM fuzzy_label_def");
	while(qry.next())
	{
		FuzzyLabel fLbl(qry);

		_mFuzzyLabels.insert(fLbl.fuzzyId(), fLbl);
		FuzzyObject fLabelObj = _mFuzzyObjects.value(fLbl.fuzzyId());
		_mFuzzyLabelsByName.insert(fLabelObj.fuzzyName(), fLbl);
	}

	qry.exec("SELECT * FROM fuzzy_approx_much");
	while(qry.next())
	{
		FuzzyApproxMuch fApprox(qry);

		_mFuzzyApproxMuch.insert(fApprox.columnId(), fApprox);
	}

	qry.exec("SELECT * FROM fuzzy_nearness_def");
	while(qry.next())
	{
		FuzzyNearness fNns(qry);

		if(!_mFuzzyNearness.contains(fNns.fuzzyId1()))				
		{
			QMap<int, FuzzyNearness> nMap;

			_mFuzzyNearness.insert(fNns.fuzzyId1(), &nMap);
		}

		QMap<int, FuzzyNearness> *fnList = _mFuzzyNearness.value(fNns.fuzzyId1());
		fnList->insert(fNns.fuzzyId2(), fNns);		
	}

	qry.exec("SELECT * FROM fuzzy_compatible_col");
	while(qry.next())
	{
		FuzzyCompatibleCol fCompat(qry);

		if (!_mFuzzyCompatibleColumnAdapters.contains(fCompat.columnId1()))
		{
			QList<int> fCompList;

			_mFuzzyCompatibleColumnAdapters.insert(fCompat.columnId1(), &fCompList);
		}

		QList<int> *fCompList = _mFuzzyCompatibleColumnAdapters.value(fCompat.columnId1());

		fCompList->append(fCompat.columnId2());
	}

	qry.exec("SELECT * FROM fuzzy_qualifiers_def");
	while(qry.next())
	{
		FuzzyQualifier fQualif(qry);

		_mFuzzyQualifiers.insert(fQualif.fuzzyId(), fQualif);
	}

	qry.exec("SELECT * FROM fuzzy_degree_cols");
	while(qry.next())
	{
		_mFuzzyDegreeCols.insert(qry.value(qry.record().indexOf("col1")).toInt(), 
			qry.value(qry.record().indexOf("col2")).toInt());		
	}

	qry.exec("SELECT * FROM fuzzy_degree_table");
	while(qry.next())
	{
		FuzzyDegreeTable fDegTable(qry);						

		_mFuzzyDegreeTables.insert(fDegTable.tableId(), fDegTable);
	}

	qry.exec("SELECT * FROM fuzzy_table_quantifiers");
	while(qry.next())
	{
		FuzzyTableQuantifier fTabQ(qry);

		if(! _mFuzzyTableQuantifiers.contains(fTabQ.tableId()))
		{
			QMap<QString, FuzzyTableQuantifier> mTabQ;
			_mFuzzyTableQuantifiers.insert(fTabQ.tableId(), &mTabQ);
		}

		_mFuzzyTableQuantifiers.value(fTabQ.tableId())->insert(fTabQ.fuzzyName(), fTabQ);
	}

	qry.exec("SELECT * FROM fuzzy_system_quantifiers");
	while(qry.next())
	{
		FuzzySystemQuantifier fSystemQuantifier(qry);

		_mFuzzySystemQuantifiers.insert(fSystemQuantifier.fuzzyName(), fSystemQuantifier);
	}
}

void FMB::SetDb(QSqlDatabase *pDb)
{
	_db = pDb;
}

int FMB::DeleteFuzzyTableInfo(QString name)
{
	if(!_mFuzzyTables.contains(name))
	{
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_meta_tables WHERE name = :name");
	qry.bindValue(":name", name);

	if(qry.exec())
	{
		_mFuzzyTables.remove(name);
		return 0;
	}
	else
	{
		return 2;
	}		
}

int FMB::DeleteFuzzyColumn(int columnId)
{
	if(! _mFuzzyColumns.contains(columnId))
	{
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_col_list WHERE col = :columnId");
	qry.bindValue(":columnId", columnId);

	if(qry.exec())
	{
		_mFuzzyColumns.remove(columnId);
		return 0;
	}
	else
	{
		return 2;
	}		
}

int FMB::DeleteFuzzyDegreeSig(int code_sig)
{
	if(! _mFuzzyDegreeSig.contains(code_sig))
	{
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_degree_sig WHERE code_sig = :code_sig");
	qry.bindValue(":code_sig", code_sig);

	if(qry.exec())
	{
		_mFuzzyDegreeSig.remove(code_sig);
		return 0;
	}
	else
	{
		return 2;
	}	
}

int FMB::DeleteFuzzyObject(int fuzzy_id)
{
	if(! _mFuzzyObjects.contains(fuzzy_id))
	{
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_object_list WHERE fuzzy_id = :fuzzy_id");
	qry.bindValue(":fuzzy_id", fuzzy_id);

	if(qry.exec())
	{
		_mFuzzyObjects.remove(fuzzy_id);
		return 0;
	}
	else
	{
		return 2;
	}	
}

int FMB::DeleteFuzzyLabel(int fuzzy_id)
{
	if (! _mFuzzyLabels.contains(fuzzy_id))
	{
		return 1;
	}

	
	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_label_def WHERE fuzzy_id = :fuzzy_id");
	qry.bindValue(":fuzzy_id", fuzzy_id);

	if(qry.exec())
	{
		_mFuzzyLabels.remove(fuzzy_id);
		return 0;
	}
	else
	{
		return 2;
	}
}

int FMB::DeleteFuzzyApproxMuch(int columnId)
{
	if (! _mFuzzyApproxMuch.contains(columnId) )
	{
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_approx_much WHERE col = :columnId");
	qry.bindValue(":columnId", columnId);

	if(qry.exec())
	{
		_mFuzzyApproxMuch.remove(columnId);
		return 0;
	}
	else
	{
		return 2;
	}		
}

int FMB::DeleteFuzzyNearness(int fuzzyId1, int fuzzyId2)
{
	if (! _mFuzzyNearness.contains(fuzzyId1))
	{
		return 1;
	}
		
	QMap<int, FuzzyNearness> *nMap = _mFuzzyNearness.value(fuzzyId1);

	if(! nMap->contains(fuzzyId2))
	{
		return 2;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_nearness_def WHERE fuzzy_id1 = :fuzzy_id1 AND fuzzy_id2 = :fuzzy_id2");
	qry.bindValue(":fuzzy_id1", fuzzyId1);		
	qry.bindValue(":fuzzy_id2", fuzzyId2);		

	if(qry.exec())
	{
		nMap->remove(fuzzyId2);
		return 0;
	}
	else
	{
		return 3;
	}	
}

int FMB::DeleteFuzzyCompatibleCol(int adapteeId, int adapterId)
{
	if(! _mFuzzyCompatibleColumnAdapters.contains(adapteeId))
	{
		return 1;
	}

	if(!_mFuzzyCompatibleColumnAdaptees.value(adapteeId)->contains(adapterId))
	{
		return 2;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_compatible_col WHERE col1 = :adapteeId AND col2 = :adapterId");
	qry.bindValue(":adapteeId", adapteeId);		
	qry.bindValue(":adapterId", adapterId);		

	if(qry.exec())
	{
		_mFuzzyCompatibleColumnAdaptees.value(adapteeId)->removeAll(adapterId);
		return 0;
	}
	else
	{
		return 3;
	}	
}

int FMB::DeleteFuzzyQualifier(int fuzzyId)
{
	if(! _mFuzzyQualifiers.contains(fuzzyId))
	{
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_qualifiers_def WHERE fuzz_id = :fuzzyId ");
	qry.bindValue(":fuzzyId", fuzzyId);					

	if(qry.exec())
	{
		_mFuzzyQualifiers.remove(fuzzyId);
		return 0;
	}
	else
	{
		return 2;
	}	
}

int FMB::DeleteFuzyDegreeCol(int columnId)
{
	if(! _mFuzzyDegreeCols.contains(columnId))
	{
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_degree_cols WHERE col1 = :columnId");
	qry.bindValue(":columnId", columnId);

	if(qry.exec())
	{
		_mFuzzyDegreeCols.remove(columnId);
		return 0;
	}
	else
	{
		return 2;
	}	
}

int FMB::DeleteFuzzyDegreeTable(int tableId)
{
	if(! _mFuzzyDegreeTables.contains(tableId))
	{
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_degree_table WHERE obj = :tableId");
	qry.bindValue(":tableId", tableId);

	if(qry.exec())
	{
		_mFuzzyDegreeCols.remove(tableId);
		return 0;
	}
	else
	{
		return 2;
	}	
}

int FMB::DeleteFuzzyTableQuantifier(int tableId, QString fuzzyName)
{
	if(! _mFuzzyTableQuantifiers.contains(tableId))
	{
		return 1;
	}

	if(! _mFuzzyTableQuantifiers.value(tableId)->contains(fuzzyName))
	{
		return 2;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_table_quantifiers WHERE obj = :tableId AND fuzzy_name = :fuzzyName" );
	qry.bindValue(":tableId", tableId);
	qry.bindValue(":fuzzyName", fuzzyName);

	if(qry.exec())
	{
		_mFuzzyTableQuantifiers.value(tableId)->remove(fuzzyName);
		return 0;
	}
	else
	{
		return 3;
	}	
}

int FMB::DeleteFuzzySystemQuantifier(QString fuzzy_name)
{
	if(! _mFuzzySystemQuantifiers.contains(fuzzy_name))
	{
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_system_quantifiers WHERE fyzzy_name = :fuzzy_name");
	qry.bindValue(":fuzzy_name", fuzzy_name);

	if(qry.exec())
	{
		_mFuzzySystemQuantifiers.remove(fuzzy_name);
		return 0;
	}
	else
	{
		return 2;
	}
}

FuzzyTableInfo FMB::GetFuzzyTableInfo(QString tableName)
{
	return _mFuzzyTables.value(tableName);
}
FuzzyCol FMB::GetFuzzyCol(int columnId)
{
	return _mFuzzyColumns.value(columnId);
}
FuzzyDegreeSig FMB::GetFuzzyDegreeSig(int codeSig)
{
	return _mFuzzyDegreeSig.value(codeSig);
}
FuzzyObject FMB::GetFuzzyObject(int fuzzyId)
{
	return _mFuzzyObjects.value(fuzzyId);
}
FuzzyLabel FMB::GetFuzzyLabel(int fuzzyId)
{
	return _mFuzzyLabels.value(fuzzyId);
}
FuzzyApproxMuch FMB::GetFuzzyApproxMuch(int columnId)
{
	return _mFuzzyApproxMuch.value(columnId);
}
FuzzyNearness FMB::GetFuzzyNearness(int fuzzyId1, int fuzzyId2)
{
	return _mFuzzyNearness.value(fuzzyId1)->value(fuzzyId2);
}
QList<int>* FMB::GetFuzzyCompatibleCols(int adapteeId)
{
	return _mFuzzyCompatibleColumnAdapters.value(adapteeId);
}
FuzzyQualifier FMB::GetFuzzyQualifier(int fuzzyId)
{
	return _mFuzzyQualifiers.value(fuzzyId);
}
int FMB::GetFuzzyDegreeCol(int columnId)
{
	return _mFuzzyDegreeCols.value(columnId);
}
FuzzyDegreeTable FMB::GetFuzzyDegreeTable(int tableId)
{
	return _mFuzzyDegreeTables.value(tableId);
}
FuzzyTableQuantifier FMB::GetFuzzyTableQuantifier(int tableId, QString fuzzyName)
{
	return _mFuzzyTableQuantifiers.value(tableId)->value(fuzzyName);
}
FuzzySystemQuantifier FMB::GetFuzzySystemQuantifier(QString fuzzyName)
{
	return _mFuzzySystemQuantifiers.value(fuzzyName);
}

FMB::FMB(void)
{
}

FMB::~FMB(void)
{
}