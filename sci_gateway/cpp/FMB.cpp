#include "FMB.h"

void FMB::LoadFMB()
{
	QSqlQuery qry = QSqlQuery("SELECT * FROM fuzzy_meta_tables", *_db);	
	while(qry.next())
	{
		FuzzyTableInfo tableInfo = FuzzyTableInfo( qry.value(0).toInt(),
			qry.value(1).toString());				

		_mFuzzyTables.insert(tableInfo.tableId(), &tableInfo);
		_mFuzzyTablesByName.insert(tableInfo.name(), &tableInfo);

		//printf("%s", qry.value(1).toString().toLatin1().data());
	}

	qry = QSqlQuery("SELECT * FROM fuzzy_col_list", *_db);	
	while(qry.next())
	{
		FuzzyCol fuzzyColumn(qry);		

		_mFuzzyColumns.insert(fuzzyColumn.columnId(), &fuzzyColumn);
		_mFuzzyColumnsByName.insert(fuzzyColumn.columnName(), &fuzzyColumn);
	}

	qry = QSqlQuery("SELECT * FROM fuzzy_degree_sig", *_db);
	while(qry.next())
	{
		FuzzyDegreeSig fDegrSig(qry);

		_mFuzzyDegreeSig.insert(fDegrSig.codeSig(), &fDegrSig);
	}

	qry.exec("SELECT * from fuzzy_object_list");
	while(qry.next())
	{
		FuzzyObject fObj(qry);

		_mFuzzyObjects.insert(fObj.fuzzyId(), &fObj);
		_mFuzzyObjectsByName.insert(fObj.fuzzyName(), &fObj);

		if (fObj.fuzzyType() < 3 || fObj.fuzzyType() == 4)
		{
			if (!_mFuzzyLabelsByColumn.contains(fObj.columnId()))
			{
				QMap<QString, FuzzyObject*> mnObjsByCol;
				_mFuzzyLabelsByColumn.insert(fObj.columnId(), &mnObjsByCol);
			}

			QMap<QString, FuzzyObject*> *mObjsByCol = _mFuzzyLabelsByColumn.value(fObj.columnId());

			mObjsByCol->insert(fObj.fuzzyName(), &fObj);
		}
	}

	qry.exec("SELECT * FROM fuzzy_label_def");
	while(qry.next())
	{
		FuzzyLabel fLbl(qry);

		_mFuzzyLabels.insert(fLbl.fuzzyId(), &fLbl);
		FuzzyObject *fLabelObj = _mFuzzyObjects.value(fLbl.fuzzyId());
		_mFuzzyLabelsByName.insert(fLabelObj->fuzzyName(), &fLbl);
	}

	qry.exec("SELECT * FROM fuzzy_approx_much");
	while(qry.next())
	{
		FuzzyApproxMuch fApprox(qry);

		_mFuzzyApproxMuch.insert(fApprox.columnId(), &fApprox);
	}

	qry.exec("SELECT * FROM fuzzy_nearness_def");
	while(qry.next())
	{
		FuzzyNearness fNns(qry);

		if(!_mFuzzyNearness.contains(fNns.fuzzyId1()))				
		{
			QMap<int, FuzzyNearness*> nMap;

			_mFuzzyNearness.insert(fNns.fuzzyId1(), &nMap);
		}

		QMap<int, FuzzyNearness*> *fnList = _mFuzzyNearness.value(fNns.fuzzyId1());
		fnList->insert(fNns.fuzzyId2(), &fNns);		
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

		_mFuzzyQualifiers.insert(fQualif.fuzzyId(), &fQualif);
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

		_mFuzzyDegreeTables.insert(fDegTable.tableId(), &fDegTable);
	}

	qry.exec("SELECT * FROM fuzzy_table_quantifiers");
	while(qry.next())
	{
		FuzzyTableQuantifier fTabQ(qry);

		if(! _mFuzzyTableQuantifiers.contains(fTabQ.tableId()))
		{
			QMap<QString, FuzzyTableQuantifier*> mTabQ;
			_mFuzzyTableQuantifiers.insert(fTabQ.tableId(), &mTabQ);
		}

		_mFuzzyTableQuantifiers.value(fTabQ.tableId())->insert(fTabQ.fuzzyName(), &fTabQ);
	}

	qry.exec("SELECT * FROM fuzzy_system_quantifiers");
	while(qry.next())
	{
		FuzzySystemQuantifier fSystemQuantifier(qry);

		_mFuzzySystemQuantifiers.insert(fSystemQuantifier.fuzzyName(), &fSystemQuantifier);
	}
}

void FMB::SetDb(QSqlDatabase *pDb)
{
	_db = pDb;
}

int FMB::DeleteFuzzyTableInfo(int tableId)
{
	if(!_mFuzzyTables.contains(tableId))
	{
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_meta_tables WHERE obj = :tableId");
	qry.bindValue(":tableId", tableId);

	if(qry.exec())
	{
		_mFuzzyTables.remove(tableId);
		return 0;
	}
	else
	{
		return 2;
	}		
}

int FMB::DeleteFuzzyTableInfo(QString name)
{
	if(!_mFuzzyTablesByName.contains(name))
	{
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_meta_tables WHERE name = :name");
	qry.bindValue(":name", name);

	if(qry.exec())
	{
		_mFuzzyTablesByName.remove(name);
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
		
	QMap<int, FuzzyNearness*> *nMap = _mFuzzyNearness.value(fuzzyId1);

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

int FMB::DeleteFuzzyDegreeCol(int columnId)
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

int FMB::UpdateFuzzyTableInfo(FuzzyTableInfo fTInfo)
{
	if(!_mFuzzyTables.contains(fTInfo.tableId()))
	{
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("UPDATE fuzzy_meta_tables SET name=:name  WHERE tableId = :tableId");
	qry.bindValue(":tableId", fTInfo.tableId());
	qry.bindValue(":name", fTInfo.name());

	if(qry.exec())
	{
		FuzzyTableInfo *ftiOld = _mFuzzyTables.value(fTInfo.tableId());
	
		_mFuzzyTablesByName.remove(ftiOld->name());

		_mFuzzyTablesByName.insert(fTInfo.name(), &fTInfo);
		_mFuzzyTables.insert(fTInfo.tableId(), &fTInfo);
		return 0;
	}
	else
	{
		return 2;
	}		
}

int FMB::UpdateFuzzyColumn(FuzzyCol col)
{
	if(! _mFuzzyColumns.contains(col.columnId()))
	{
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("UPDATE fuzzy_col_list SET obj=:tableId, f_type=:fType,\
				len=:len, name=:columnName, code_sig=:codeSig, com=:com,\
				um=:um WHERE col = :columnId");
	qry.bindValue(":columnId", col.columnId());
	qry.bindValue(":tableId", col.tableId());
	qry.bindValue(":fType", col.fType());
	qry.bindValue(":len", col.len());
	qry.bindValue(":columnName", col.columnName());
	qry.bindValue(":codeSig", col.codeSig());
	qry.bindValue(":um", col.um());

	if(qry.exec())
	{
		//TO-DO: just change object by reference (they must be the same in all collections)
		FuzzyCol *fcOld = _mFuzzyColumns.value(col.columnId());
		
		_mFuzzyColumnsByName.remove(fcOld->columnName());
		_mFuzzyColumnsByName.insert(col.columnName(), &col);		

		_mFuzzyColumns.insert(col.columnId(), &col);

		return 0;
	}
	else
	{
		return 2;
	}		
}
int FMB::UpdateFuzzyDegreeSig(FuzzyDegreeSig fDegSig)
{
	if(! _mFuzzyDegreeSig.contains(fDegSig.codeSig()))
	{
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("UPDATE fuzzy_degree_sig SET significance=:significance WHERE code_sig = :code_sig");
	qry.bindValue(":code_sig", fDegSig.codeSig());
	qry.bindValue(":significance", fDegSig.significance());

	if(qry.exec())
	{
		_mFuzzyDegreeSig.insert(fDegSig.codeSig(), &fDegSig);
		return 0;
	}
	else
	{
		return 2;
	}	
}
int FMB::UpdateFuzzyObject(FuzzyObject fObj)
{
	if(! _mFuzzyObjects.contains(fObj.fuzzyId()))
	{
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("UPDATE fuzzy_object_list SET fuzzy_name=:fuzzyName,\
				fuzzy_type=:fuzzyType, col=:columnId  WHERE fuzzy_id = :fuzzyId");
	qry.bindValue(":fuzzyId", fObj.fuzzyId());
	qry.bindValue(":fuzzyName", fObj.fuzzyName());
	qry.bindValue(":fuzzyType", fObj.fuzzyType());	
	qry.bindValue(":columnId", fObj.columnId());	

	if(qry.exec())
	{
		FuzzyObject *foOlf = _mFuzzyObjects.value(fObj.fuzzyId());
		_mFuzzyObjectsByName.remove(fObj.fuzzyName());
		_mFuzzyObjectsByName.insert(fObj.fuzzyName(), &fObj);

		_mFuzzyObjects.insert(fObj.fuzzyId(), &fObj);
		
		return 0;
	}
	else
	{
		return 2;
	}	
}
int FMB::UpdateFuzzyLabel(FuzzyLabel fLabel)
{
	if (! _mFuzzyLabels.contains(fLabel.fuzzyId()))
	{
		return 1;
	}
	
	QSqlQuery qry(*_db);
	qry.prepare("UPDATE fuzzy_label_def SET alpha=:alpha,\
				beta=:beta, gamma=:gamma, delta=:delta WHERE fuzzy_id = :fuzzyId");
	qry.bindValue(":fuzzyId", fLabel.fuzzyId());
	qry.bindValue(":alpha", fLabel.alpha());
	qry.bindValue(":beta", fLabel.beta());
	qry.bindValue(":gamma", fLabel.gamma());
	qry.bindValue(":delta", fLabel.delta());

	if(qry.exec())
	{
		FuzzyLabel *lAltered = _mFuzzyLabels.value(fLabel.fuzzyId());

		lAltered->SetValues(fLabel.alpha(), fLabel.beta(), fLabel.gamma(), fLabel.delta());

		return 0;
	}
	else
	{
		return 2;
	}
}
int FMB::UpdateFuzzyApproxMuch(FuzzyApproxMuch fApprox)
{
	if (! _mFuzzyApproxMuch.contains(fApprox.columnId()) )
	{
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("UPDATE fuzzy_approx_much SET margin=:margin, much=:much WHERE col = :columnId");
	qry.bindValue(":columnId", fApprox.columnId());
	qry.bindValue(":margin", fApprox.margin());
	qry.bindValue(":much", fApprox.much());

	if(qry.exec())
	{
		_mFuzzyApproxMuch.insert(fApprox.columnId(), &fApprox);
		return 0;
	}
	else
	{
		return 2;
	}		
}
int FMB::UpdateFuzzyNearness(FuzzyNearness fNns)
{
	if (! _mFuzzyNearness.contains(fNns.fuzzyId1()))
	{
		return 1;
	}
		
	QMap<int, FuzzyNearness*> *nMap = _mFuzzyNearness.value(fNns.fuzzyId1());

	if(! nMap->contains(fNns.fuzzyId2()))
	{
		return 2;
	}

	QSqlQuery qry(*_db);
	qry.prepare("UPDATE fuzzy_nearness_def SET degree=:degree WHERE fuzzy_id1 = :fuzzy_id1 AND fuzzy_id2 = :fuzzy_id2");
	qry.bindValue(":fuzzy_id1", fNns.fuzzyId1());		
	qry.bindValue(":fuzzy_id2", fNns.fuzzyId2());		
	qry.bindValue(":degree", fNns.degree());		

	if(qry.exec())
	{
		nMap->insert(fNns.fuzzyId2(), &fNns);
		return 0;
	}
	else
	{
		return 3;
	}	
}

int FMB::UpdateFuzzyQualifier(FuzzyQualifier fQualif)
{
	if(! _mFuzzyQualifiers.contains(fQualif.fuzzyId()))
	{
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("UPDATE fuzzy_qualifiers_def SET qualifier=:qualifier WHERE fuzz_id = :fuzzyId ");
	qry.bindValue(":fuzzyId", fQualif.fuzzyId());					
	qry.bindValue(":qualifier", fQualif.qualifier());

	if(qry.exec())
	{
		_mFuzzyQualifiers.insert(fQualif.fuzzyId(), &fQualif);
		return 0;
	}
	else
	{
		return 2;
	}	
}

int FMB::UpdateFuzzyTableQuantifier(FuzzyTableQuantifier fTableQuantif)
{
	if(! _mFuzzyTableQuantifiers.contains(fTableQuantif.tableId()))
	{
		return 1;
	}

	if(! _mFuzzyTableQuantifiers.value(fTableQuantif.tableId())->contains(fTableQuantif.fuzzyName()))
	{
		return 2;
	}

	QSqlQuery qry(*_db);
	qry.prepare("UPDATE fuzzy_table_quantifiers SET\
				fuzzy_type=:fuzzyType, alpha=:alpha, beta=:beta, gamma=:gamma,\
				delta=:delta WHERE obj = :tableId AND fuzzy_name = :fuzzyName" );
	qry.bindValue(":tableId", fTableQuantif.tableId());
	qry.bindValue(":fuzzyType", fTableQuantif.fuzzyType());
	qry.bindValue(":alpha", fTableQuantif.alpha());
	qry.bindValue(":beta", fTableQuantif.beta());
	qry.bindValue(":gamma", fTableQuantif.gamma());
	qry.bindValue(":delta", fTableQuantif.delta());

	if(qry.exec())
	{
		_mFuzzyTableQuantifiers.value(fTableQuantif.tableId())->insert(fTableQuantif.fuzzyName(), &fTableQuantif);
		return 0;
	}
	else
	{
		return 3;
	}	
}
int FMB::UpdateFuzzySystemQuantifier(FuzzySystemQuantifier fSystemQuantif)
{
	if(! _mFuzzySystemQuantifiers.contains(fSystemQuantif.fuzzyName()))
	{
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("UPDATE fuzzy_system_quantifiers SET fuzzy_type=:fuzzyType,\
				alpha=:alpha, beta=:beta, gamma=:gamma,\
				delta=:delta WHERE fyzzy_name = :fuzzyName");
	qry.bindValue(":fuzzyName", fSystemQuantif.fuzzyName());
	qry.bindValue(":fuzzyType", fSystemQuantif.fuzzyType());
	qry.bindValue(":alpha", fSystemQuantif.alpha());
	qry.bindValue(":beta", fSystemQuantif.beta());
	qry.bindValue(":gamma", fSystemQuantif.gamma());
	qry.bindValue(":delta", fSystemQuantif.delta());

	if(qry.exec())
	{
		_mFuzzySystemQuantifiers.remove(fSystemQuantif.fuzzyName());
		return 0;
	}
	else
	{
		return 2;
	}
}	
FuzzyTableInfo* FMB::GetFuzzyTableInfo(QString tableName)
{
	return _mFuzzyTablesByName.value(tableName);
}
FuzzyTableInfo* FMB::GetFuzzyTableInfo(int tableId)
{
	return _mFuzzyTables.value(tableId);
}
FuzzyCol* FMB::GetFuzzyCol(int columnId)
{
	return _mFuzzyColumns.value(columnId);
}
FuzzyCol* FMB::GetFuzzyCol(QString columnName)
{
	return _mFuzzyColumnsByName.value(columnName);
}
FuzzyDegreeSig* FMB::GetFuzzyDegreeSig(int codeSig)
{
	return _mFuzzyDegreeSig.value(codeSig);
}
FuzzyObject* FMB::GetFuzzyObject(int fuzzyId)
{
	return _mFuzzyObjects.value(fuzzyId);
}
FuzzyObject* FMB::GetFuzzyObject(QString fuzzyName)
{
	return _mFuzzyObjectsByName.value(fuzzyName);
}
FuzzyLabel* FMB::GetFuzzyLabel(int fuzzyId)
{
	return _mFuzzyLabels.value(fuzzyId);
}
FuzzyLabel* FMB::GetFuzzyLabel(QString fuzzy_name)
{
	return _mFuzzyLabelsByName.value(fuzzy_name);
}
FuzzyApproxMuch* FMB::GetFuzzyApproxMuch(int columnId)
{
	return _mFuzzyApproxMuch.value(columnId);
}
FuzzyNearness* FMB::GetFuzzyNearness(int fuzzyId1, int fuzzyId2)
{
	return _mFuzzyNearness.value(fuzzyId1)->value(fuzzyId2);
}
QList<int>* FMB::GetFuzzyCompatibleCols(int adapteeId)
{
	QList<int> *lAllColmatibleCols = new QList<int>();
	lAllColmatibleCols->append(*_mFuzzyCompatibleColumnAdapters.value(adapteeId));
	lAllColmatibleCols->append(*_mFuzzyCompatibleColumnAdaptees.value(adapteeId));

	return lAllColmatibleCols;
}
FuzzyQualifier* FMB::GetFuzzyQualifier(int fuzzyId)
{
	return _mFuzzyQualifiers.value(fuzzyId);
}
int FMB::GetFuzzyDegreeCol(int columnId)
{
	return _mFuzzyDegreeCols.value(columnId);
}
FuzzyDegreeTable* FMB::GetFuzzyDegreeTable(int tableId)
{
	return _mFuzzyDegreeTables.value(tableId);
}
FuzzyTableQuantifier* FMB::GetFuzzyTableQuantifier(int tableId, QString fuzzyName)
{
	return _mFuzzyTableQuantifiers.value(tableId)->value(fuzzyName);
}
FuzzySystemQuantifier* FMB::GetFuzzySystemQuantifier(QString fuzzyName)
{
	return _mFuzzySystemQuantifiers.value(fuzzyName);
}


QString FMB::TableIdQuery(QString tableName)
{
	return "SELECT table_id FROM FUZZY_META_TABLES WHERE name = " + tableName;
}

QString FMB::ColumnIdQuery(QString table, QString columnName)
{
	return "SELECT col_id FROM FUZZY_META_COLUMNS WHERE name = " + columnName + 
		"AND table_id = " + "(SELECT table_id from FUZZY_META_TABLES WHERE name = "+table+")";
}

void FMB::CreateTableMetaInfo(QString tableName)
{
    QSqlQuery query(*_db);

    if(query.exec("INSERT INTO FUZZY_META_TABLES (name) VALUES (" + tableName + ") RETURNING table_id"))
	{
		query.next();
		_mFuzzyTables.insert(query.value(0).toInt(), &FuzzyTableInfo(query.value(0).toInt(), tableName));
	}
}

void FMB::CreateFuzzyCol(int tableId, int f_type, int len, int code_sig, 
							  QString columnName, QString com, QString um)
{
	 QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_COL_LIST (obj, f_type, len, code_sig, column_name, com, um)"
		 " VALUES (:tableId, :f_type, :len, :code_sig, :columnName, :com, :um) RETURNING col");     
     query.bindValue(":f_type", f_type);
     query.bindValue(":len", len);
	 query.bindValue(":code_sig", code_sig);
	 query.bindValue(":columnName", columnName);
	 query.bindValue(":com", com);
	 query.bindValue(":um", um);
     if(query.exec())
	 {
		 query.next();
		 _mFuzzyColumns.insert(query.value(0).toInt(), &FuzzyCol(tableId, query.value(0).toInt(), f_type, len, code_sig, columnName, com, um ));
	 }
}

void FMB::CreateFuzzyDegreeSig(int code_sig, QString significance)
{
	 QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_DEGREE_SIG"
		 "VALUES (:code_sig, :significance)");
     query.bindValue(":code_sig", code_sig);
     query.bindValue(":significance", significance);
     if(query.exec())
	 {
		 _mFuzzyDegreeSig.insert(code_sig, &FuzzyDegreeSig(code_sig, significance));
	 }
}

void FMB::CreateFuzzyObject(int columnId, QString fuzzy_name, int fuzzy_type)
{
	QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_OBJECT_LIST"
		 "VALUES (:column, NULL, :fuzzy_name, :fuzzy_type) RETURNING fuzzy_id");
     query.bindValue(":column", columnId);
     query.bindValue(":fuzzy_name", fuzzy_name);
     query.bindValue(":fuzzy_type", fuzzy_type);
     if(query.exec())
	 {
		 query.next();
		 _mFuzzyObjects.insert(query.value(0).toInt(), &FuzzyObject(columnId, query.value(0).toInt(), fuzzy_name, fuzzy_type));
	 }
}

void FMB::CreateFuzzyLabelDef(int fuzzy_id, 
		double alpha, double beta, double gamma, double delta)
{
	QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_LABEL_DEF"
		 "VALUES (:fuzzy_id, :alpha, :beta, :gamma, :delta)");     
     query.bindValue(":fuzzy_id", fuzzy_id);
     query.bindValue(":alpha", alpha);
	 query.bindValue(":beta", beta);
	 query.bindValue(":gamma", gamma);
	 query.bindValue(":delta", delta);
	 if(query.exec())
	 {
		  FuzzyLabel fLab(fuzzy_id, alpha, beta, gamma, delta);
		  FuzzyObject *fLabObj = _mFuzzyObjects.value(fuzzy_id);

		 _mFuzzyLabels.insert(fuzzy_id, &fLab);
		 _mFuzzyLabelsByName.insert(fLabObj->fuzzyName(), &fLab);		 
	 }
}

void FMB::CreateFuzzyApproxMuch(int columnId, double margin, double much)
{
	QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_APPROX_MUCH"
		 "VALUES (:column, :margin, :much)");
     query.bindValue(":column", columnId);
     query.bindValue(":margin", margin);
     query.bindValue(":much", much);
     if(query.exec())
	 {
		_mFuzzyApproxMuch.insert(columnId, &FuzzyApproxMuch(columnId, margin, much));
	 }
}

void FMB::CreateFuzzyNearness( int fuzzy_id1, int fuzzy_id2,
		double degree)
{
	QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_NEARNESS_DEF"
		 "VALUES (:fuzzy_id1, :fuzzy_id2, :degree)");
     query.bindValue(":fuzzy_id1", fuzzy_id1);
     query.bindValue(":fuzzy_id2", fuzzy_id2);
	 query.bindValue(":degree", degree);
	 if (query.exec())
	 {
		 if (!_mFuzzyNearness.contains(fuzzy_id1))
		 {
			 _mFuzzyNearness.insert(fuzzy_id1, &QMap<int, FuzzyNearness*>());
		 }

		 _mFuzzyNearness.value(fuzzy_id1)->insert(fuzzy_id2, &FuzzyNearness(fuzzy_id1, fuzzy_id2, degree));
	 }
}


void FMB::CreateFuzzyCompatibleCol(int col1, int col2)
{
	 QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_COMPATIBLE_COL"
		 "VALUES (:column1, :column2)");
     query.bindValue(":col1", col1);
	 query.bindValue(":col2", col2);     

     if(query.exec())
	 {
		 if (!_mFuzzyCompatibleColumnAdaptees.contains(col2))
		 {
			 _mFuzzyCompatibleColumnAdaptees.insert(col2, &QList<int>());
		 }

		 _mFuzzyCompatibleColumnAdaptees.value(col2)->append(col1);

		 if (!_mFuzzyCompatibleColumnAdapters.contains(col1))
		 {
			 _mFuzzyCompatibleColumnAdapters.insert(col1, &QList<int>());
		 }

		 _mFuzzyCompatibleColumnAdapters.value(col1)->append(col2);
	 }
}

void FMB::CreateFuzzyQualifier(int fuzzy_id, double qualifier)
{
	 QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_QUALIFIERS_DEF"
		 "VALUES (:fuzzy_id, :qualifier)");     
	 query.bindValue(":fuzzy_id", fuzzy_id);     
	 query.bindValue(":qualifier", qualifier);     

     if(query.exec())
	 {
		 _mFuzzyQualifiers.insert(fuzzy_id, &FuzzyQualifier(fuzzy_id, qualifier));
	 }
}

void FMB::CreateFuzzyDegreeCol(int col1, int col2)
{
		 QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_DEGREE_COLS"
		 "VALUES (:column1, :column2)");
     query.bindValue(":column1", col1);
	 query.bindValue(":column2", col2);     

     if(query.exec())
	 {
		 _mFuzzyDegreeCols.insert(col1, col2);
	 }
}

void FMB::CreateFuzzyDegreeTable(int tableId, int columnId, QChar degree_type)
{
		 QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_DEGREE_TABLE"
		 "VALUES (:table_id, :column_id, :deree_type)");
	 query.bindValue(":table_id",  tableId);
     query.bindValue(":column_id",  columnId);
	 query.bindValue(":degree_type", degree_type);     
     if(query.exec())
	 {
		 _mFuzzyDegreeTables.insert(tableId, &FuzzyDegreeTable(tableId, columnId, degree_type));
	 }
}

void FMB::CreateFuzzyTableQuantifier(int tableId, QString fuzzy_name, int fuzzy_type, 
		double alpha, double beta, double gamma, double delta)
{
	 QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_TABLE_QUANTIFIERS"
		 "VALUES (:table_id, :fuzzy_name, :fuzzy_type, :alpha, :beta, :gamma, :delta)");
     query.bindValue(":table_id", tableId);
	 query.bindValue(":fuzzy_name", fuzzy_name);     
	 query.bindValue(":fuzzy_type", fuzzy_type);  
	 query.bindValue(":alpha", alpha);
	 query.bindValue(":beta", beta);
	 query.bindValue(":gamma", gamma);
	 query.bindValue(":delta", delta);
     if(query.exec())
	 {
		 if (!_mFuzzyTableQuantifiers.contains(tableId))
		 {
			 _mFuzzyTableQuantifiers.insert(tableId, &QMap<QString, FuzzyTableQuantifier*>());
		 }
		 _mFuzzyTableQuantifiers.value(tableId)->insert(fuzzy_name, &FuzzyTableQuantifier(tableId, fuzzy_name, fuzzy_type, alpha, beta, gamma, delta));
	 }
}

void FMB::CreateFuzzySystemQuantifier(QString fuzzy_name, int fuzzy_type, double alpha,
		double beta, double gamma, double delta)
{
	 QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_SYSTEM_QUANTIFIERS"
		 "VALUES (:fuzzy_name, :fuzzy_type, :alpha, :beta, :gamma, :delta)");     
	 query.bindValue(":fuzzy_name", fuzzy_name);     
	 query.bindValue(":fuzzy_type", fuzzy_type);  
	 query.bindValue(":alpha", alpha);
	 query.bindValue(":beta", beta);
	 query.bindValue(":gamma", gamma);
	 query.bindValue(":delta", delta);
     if(query.exec())
	 {
		 _mFuzzySystemQuantifiers.insert(fuzzy_name, &FuzzySystemQuantifier(fuzzy_name, fuzzy_type, alpha, beta, gamma, delta));
	 }
}


FMB::FMB(void)
{
}

FMB::~FMB(void)
{
}