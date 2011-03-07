/*
 * Copyright (C) 2010 - Igor GRIDCHYN
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "FMB.h"

QString FMB::LastError()
{
	return _lastError;	
}

void FMB::LoadFMB()
{
	QSqlQuery qry = QSqlQuery("SELECT * FROM fuzzy_meta_tables", *_db);	
	while(qry.next())
	{
		FuzzyTableInfo *tableInfo = new FuzzyTableInfo( qry.value(0).toInt(),
			qry.value(1).toString());				

		_mFuzzyTables.insert(tableInfo->tableId(), tableInfo);
		_mFuzzyTablesByName.insert(tableInfo->name(), tableInfo);

		//printf("%s", qry.value(1).toString().toLatin1().data());
	}

	qry = QSqlQuery("SELECT * FROM fuzzy_col_list", *_db);	
	while(qry.next())
	{
		FuzzyCol *fuzzyColumn = new FuzzyCol(qry);						

		_mFuzzyColumns.insert(fuzzyColumn->columnId(), fuzzyColumn);
		_mFuzzyColumnsByName.insert(fuzzyColumn->columnName(), fuzzyColumn);
	}

	qry = QSqlQuery("SELECT * FROM fuzzy_degree_sig", *_db);
	while(qry.next())
	{
		FuzzyDegreeSig *fDegrSig = new FuzzyDegreeSig(qry);

		_mFuzzyDegreeSig.insert(fDegrSig->codeSig(), fDegrSig);
		_mFuzzyDegreeSigByName.insert(fDegrSig->significance(), fDegrSig);
	}

	qry.exec("SELECT * from fuzzy_object_list");
	while(qry.next())
	{
		FuzzyObject *fObj =new FuzzyObject(qry);

		_mFuzzyObjects.insert(fObj->fuzzyId(), fObj);
		_mFuzzyObjectsByName.insert(fObj->fuzzyName(), fObj);

		if (fObj->fuzzyType() < 3 || fObj->fuzzyType() == 4)
		{
			if (!_mFuzzyLabelsByColumn.contains(fObj->columnId()))
			{
				QMap<QString, FuzzyObject*> *mnObjsByCol = new QMap<QString, FuzzyObject*>();
				_mFuzzyLabelsByColumn.insert(fObj->columnId(), mnObjsByCol);
			}

			QMap<QString, FuzzyObject*> *mObjsByCol = _mFuzzyLabelsByColumn.value(fObj->columnId());

			mObjsByCol->insert(fObj->fuzzyName(), fObj);
		}
	}

	qry.exec("SELECT * FROM fuzzy_label_def");
	while(qry.next())
	{
		FuzzyLabel *fLbl = new FuzzyLabel(qry);

		_mFuzzyLabels.insert(fLbl->fuzzyId(), fLbl);
		FuzzyObject *fLabelObj = _mFuzzyObjects.value(fLbl->fuzzyId());
		_mFuzzyLabelsByName.insert(fLabelObj->fuzzyName(), fLbl);
	}

	qry.exec("SELECT * FROM fuzzy_approx_much");
	while(qry.next())
	{
		FuzzyApproxMuch *fApprox = new FuzzyApproxMuch(qry);

		_mFuzzyApproxMuch.insert(fApprox->columnId(), fApprox);
	}

	qry.exec("SELECT * FROM fuzzy_nearness_def");
	while(qry.next())
	{
		FuzzyNearness *fNns = new FuzzyNearness(qry);

		if(!_mFuzzyNearness.contains(fNns->fuzzyId1()))				
		{
			QMap<int, FuzzyNearness*> *nMap = new QMap<int, FuzzyNearness*>();

			_mFuzzyNearness.insert(fNns->fuzzyId1(), nMap);
		}

		QMap<int, FuzzyNearness*> *fnList = _mFuzzyNearness.value(fNns->fuzzyId1());
		fnList->insert(fNns->fuzzyId2(), fNns);		
	}

	qry.exec("SELECT * FROM fuzzy_compatible_col");
	while(qry.next())
	{
		FuzzyCompatibleCol *fCompat = new FuzzyCompatibleCol(qry);

		if (!_mFuzzyCompatibleColumnAdapters.contains(fCompat->columnId1()))
		{
			QList<int> *fCompList = new QList<int>();

			_mFuzzyCompatibleColumnAdapters.insert(fCompat->columnId1(), fCompList);
		}

		QList<int> *fCompList = _mFuzzyCompatibleColumnAdapters.value(fCompat->columnId1());

		fCompList->append(fCompat->columnId2());
	}

	qry.exec("SELECT * FROM fuzzy_qualifiers_def");
	while(qry.next())
	{
		FuzzyQualifier *fQualif = new FuzzyQualifier(qry);

		_mFuzzyQualifiers.insert(fQualif->fuzzyId(), fQualif);
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
		FuzzyDegreeTable *fDegTable = new FuzzyDegreeTable(qry);						

		_mFuzzyDegreeTables.insert(fDegTable->tableId(), fDegTable);
	}

	qry.exec("SELECT * FROM fuzzy_table_quantifiers");
	while(qry.next())
	{
		FuzzyTableQuantifier *fTabQ = new FuzzyTableQuantifier(qry);

		if(! _mFuzzyTableQuantifiers.contains(fTabQ->tableId()))
		{
			QMap<QString, FuzzyTableQuantifier*> *mTabQ = new QMap<QString, FuzzyTableQuantifier*>();
			_mFuzzyTableQuantifiers.insert(fTabQ->tableId(), mTabQ);
		}

		_mFuzzyTableQuantifiers.value(fTabQ->tableId())->insert(fTabQ->fuzzyName(), fTabQ);
	}

	qry.exec("SELECT * FROM fuzzy_system_quantifiers");
	while(qry.next())
	{
		FuzzySystemQuantifier *fSystemQuantifier = new FuzzySystemQuantifier(qry);

		_mFuzzySystemQuantifiers.insert(fSystemQuantifier->fuzzyName(), fSystemQuantifier);
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
		_lastError = "Table with given id is not present in the FMB";
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_meta_tables WHERE table_id = :tableId");
	qry.bindValue(":tableId", tableId);

	if(qry.exec())
	{
		_mFuzzyTables.remove(tableId);
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not remove table info from database: ", qry.lastError().text().toLatin1().data());
		return 2;
	}		
}

int FMB::DeleteFuzzyTableInfo(QString name)
{
	if(!_mFuzzyTablesByName.contains(name))
	{
		_lastError = "Fuzzy table with given name is not present in the FMB";
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_meta_tables WHERE \"name\" = :name");
	qry.bindValue(":name", name);

	if(qry.exec())
	{
		_mFuzzyTablesByName.remove(name);
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not remove table info from database: ", qry.lastError().text().toLatin1().data());
		return 2;
	}		
}

int FMB::DeleteFuzzyColumn(int columnId)
{
	if(! _mFuzzyColumns.contains(columnId))
	{
		_lastError = "Fuzzy column with given id is not present in the FMB";
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_col_list WHERE col = :columnId");
	qry.bindValue(":columnId", columnId);

	if(qry.exec())
	{
		_mFuzzyColumns.remove(columnId);
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not remove fuzzy column from database: ", qry.lastError().text().toLatin1().data());
		return 2;
	}		
}

int FMB::DeleteFuzzyDegreeSig(int code_sig)
{
	if(! _mFuzzyDegreeSig.contains(code_sig))
	{
		_lastError = "Fuzzy degree signature with given id is not present in the FMB";
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_degree_sig WHERE code_sig = :code_sig");
	qry.bindValue(":code_sig", code_sig);

	if(qry.exec())
	{
		_mFuzzyDegreeSigByName.remove(_mFuzzyDegreeSig.value(code_sig)->significance());
		_mFuzzyDegreeSig.remove(code_sig);
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not remove fuzzy degree signature from database: ", qry.lastError().text().toLatin1().data());
		return 2;
	}	
}

int FMB::DeleteFuzzyObject(int fuzzy_id)
{
	if(! _mFuzzyObjects.contains(fuzzy_id))
	{
		_lastError = "Fuzzy object with given id is not present in the FMB";
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_object_list WHERE fuzzy_id = :fuzzy_id");
	qry.bindValue(":fuzzy_id", fuzzy_id);

	if(qry.exec())
	{
		_mFuzzyObjects.remove(fuzzy_id);
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not remove fuzzy object from database: ", qry.lastError().text().toLatin1().data());
		return 2;
	}	
}



int FMB::DeleteFuzzyLabel(int fuzzy_id)
{
	if (! _mFuzzyLabels.contains(fuzzy_id))
	{
		_lastError = "Fuzzy label with given fuzzy id is not present in the FMB";
		return 1;
	}

	
	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_label_def WHERE fuzzy_id = :fuzzy_id");
	qry.bindValue(":fuzzy_id", fuzzy_id);

	if(qry.exec())
	{
		_mFuzzyLabels.remove(fuzzy_id);
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not remove fuzzy label from database: ", qry.lastError().text().toLatin1().data());
		return 2;
	}
}

int FMB::DeleteFuzzyApproxMuch(int columnId)
{
	if (! _mFuzzyApproxMuch.contains(columnId) )
	{
		_lastError = "Fuzzy approximate and much info with given fuzzy id is not present in the FMB";
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_approx_much WHERE col = :columnId");
	qry.bindValue(":columnId", columnId);

	if(qry.exec())
	{
		_mFuzzyApproxMuch.remove(columnId);
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not remove fuzzy approximate and much info from database: ", qry.lastError().text().toLatin1().data());
		return 2;
	}		
}

int FMB::DeleteFuzzyNearness(int fuzzyId1, int fuzzyId2)
{
	if (! _mFuzzyNearness.contains(fuzzyId1))
	{
		_lastError = "Fuzzy nearness definition with given fuzzy ids is not present in the FMB";
		return 1;
	}
		
	QMap<int, FuzzyNearness*> *nMap = _mFuzzyNearness.value(fuzzyId1);

	if(! nMap->contains(fuzzyId2))
	{
		_lastError = "Fuzzy nearness definition with given fuzzy ids is not present in the FMB";
		return 2;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_nearness_def WHERE fuzzy_id1 = :fuzzy_id1 AND fuzzy_id2 = :fuzzy_id2");
	qry.bindValue(":fuzzy_id1", fuzzyId1);		
	qry.bindValue(":fuzzy_id2", fuzzyId2);		

	if(qry.exec())
	{
		nMap->remove(fuzzyId2);
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not remove fuzzy nearness definition from database: ", qry.lastError().text().toLatin1().data());
		return 3;
	}	
}

int FMB::DeleteFuzzyCompatibleCol(int adapteeId, int adapterId)
{
	if(! _mFuzzyCompatibleColumnAdapters.contains(adapteeId))
	{
		_lastError = "Fuzzy compatible columns info with given column ids is not present in the FMB";
		return 1;
	}

	if(!_mFuzzyCompatibleColumnAdapters.value(adapteeId)->contains(adapterId))
	{
		_lastError = "Fuzzy compatible columns info with given column ids is not present in the FMB";
		return 2;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_compatible_col WHERE col1 = :adapteeId AND col2 = :adapterId");
	qry.bindValue(":adapteeId", adapteeId);		
	qry.bindValue(":adapterId", adapterId);		

	if(qry.exec())
	{
		_mFuzzyCompatibleColumnAdapters.value(adapteeId)->removeAll(adapterId);
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not remove fuzzy compatible columns info from database: ", qry.lastError().text().toLatin1().data());
		return 3;
	}	
}

int FMB::DeleteFuzzyQualifier(int fuzzyId)
{
	if(! _mFuzzyQualifiers.contains(fuzzyId))
	{
		_lastError = "Fuzzy qualifier with given fuzzy id is not present in the FMB";
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_qualifiers_def WHERE fuzzy_id = :fuzzyId ");
	qry.bindValue(":fuzzyId", fuzzyId);					

	if(qry.exec())
	{
		_mFuzzyQualifiers.remove(fuzzyId);
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not remove fuzzy qualifier from database: ", qry.lastError().text().toLatin1().data());
		return 2;
	}	
}

int FMB::DeleteFuzzyDegreeCol(int columnId)
{
	if(! _mFuzzyDegreeCols.contains(columnId))
	{
		_lastError = "Fuzzy degree col info with given column id is not present in the FMB";
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_degree_cols WHERE col1 = :columnId");
	qry.bindValue(":columnId", columnId);

	if(qry.exec())
	{
		_mFuzzyDegreeCols.remove(columnId);
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not remove fuzzy degree column info from database: ", qry.lastError().text().toLatin1().data());
		return 2;
	}	
}

int FMB::DeleteFuzzyDegreeTable(int tableId)
{
	if(! _mFuzzyDegreeTables.contains(tableId))
	{
		_lastError = "Fuzzy degree table info with given column id is not present in the FMB";
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_degree_table WHERE obj = :tableId");
	qry.bindValue(":tableId", tableId);

	if(qry.exec())
	{
		_mFuzzyDegreeCols.remove(tableId);
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not remove fuzzy degree table info from database: ", qry.lastError().text().toLatin1().data());
		return 2;
	}	
}

int FMB::DeleteFuzzyTableQuantifier(int tableId, QString fuzzyName)
{
	if(! _mFuzzyTableQuantifiers.contains(tableId))
	{
		_lastError = "Fuzzy table quantifers with given table id are not present in the FMB";
		return 1;
	}

	if(! _mFuzzyTableQuantifiers.value(tableId)->contains(fuzzyName))
	{
		_lastError = "Fuzzy table quantifers with given name is not present in the quantifers collection for table with given table id";
		return 2;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_table_quantifiers WHERE obj = :tableId AND fuzzy_name = :fuzzyName" );
	qry.bindValue(":tableId", tableId);
	qry.bindValue(":fuzzyName", fuzzyName);

	if(qry.exec())
	{
		_mFuzzyTableQuantifiers.value(tableId)->remove(fuzzyName);
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not remove fuzzy table quantifier from database: ", qry.lastError().text().toLatin1().data());
		return 3;
	}	
}

int FMB::DeleteFuzzySystemQuantifier(QString fuzzy_name)
{
	if(! _mFuzzySystemQuantifiers.contains(fuzzy_name))
	{
		_lastError = "Fuzzy system quantifers with given name is not present in the FMB";
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("DELETE FROM fuzzy_system_quantifiers WHERE fuzzy_name = :fuzzy_name");
	qry.bindValue(":fuzzy_name", fuzzy_name);

	if(qry.exec())
	{
		_mFuzzySystemQuantifiers.remove(fuzzy_name);
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not remove fuzzy system quantifier from database: ", qry.lastError().text().toLatin1().data());
		return 2;
	}
}

int FMB::UpdateFuzzyTableInfo(FuzzyTableInfo fTInfo)
{
	if(!_mFuzzyTables.contains(fTInfo.tableId()))
	{
		_lastError = "Fuzzy table with given name is not present in the FMB";
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("UPDATE fuzzy_meta_tables SET \"name\"=:name  WHERE table_id = :tableId");
	qry.bindValue(":tableId", fTInfo.tableId());
	qry.bindValue(":name", fTInfo.name());

	if(qry.exec())
	{
		FuzzyTableInfo *ftiOld = _mFuzzyTables.value(fTInfo.tableId());
	
		_mFuzzyTablesByName.remove(ftiOld->name());

		_mFuzzyTablesByName.insert(fTInfo.name(), &fTInfo);
		_mFuzzyTables.insert(fTInfo.tableId(), &fTInfo);
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not update table info in database: ", qry.lastError().text().toLatin1().data());		
		return 2;
	}		
}

int FMB::UpdateFuzzyColumn(FuzzyCol col)
{
	if(! _mFuzzyColumns.contains(col.columnId()))
	{
		_lastError = "Fuzzy column with given id is not present in the FMB";
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("UPDATE fuzzy_col_list SET obj=:tableId, f_type=:fType,\
				len=:len, column_name=:columnName, code_sig=:codeSig, com=:com,\
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

		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not update fuzzy column in database: ", qry.lastError().text().toLatin1().data());
		return 2;
	}		
}
int FMB::UpdateFuzzyDegreeSig(FuzzyDegreeSig fDegSig)
{
	if(! _mFuzzyDegreeSig.contains(fDegSig.codeSig()))
	{
		_lastError = "Fuzzy degree signature with given id is not present in the FMB";
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("UPDATE fuzzy_degree_sig SET significance=:significance WHERE code_sig = :code_sig");
	qry.bindValue(":code_sig", fDegSig.codeSig());
	qry.bindValue(":significance", fDegSig.significance());

	if(qry.exec())
	{
		_mFuzzyDegreeSig.insert(fDegSig.codeSig(), &fDegSig);
		_mFuzzyDegreeSigByName.insert(fDegSig.significance(), &fDegSig);

		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not update fuzzy degree signature in database: ", qry.lastError().text().toLatin1().data());
		return 2;
	}	
}
int FMB::UpdateFuzzyObject(FuzzyObject fObj)
{
	if(! _mFuzzyObjects.contains(fObj.fuzzyId()))
	{
		_lastError = "Fuzzy object with given id is not present in the FMB";
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
		
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not update fuzzy object in database: ", qry.lastError().text().toLatin1().data());
		return 2;
	}	
}
int FMB::UpdateFuzzyLabel(FuzzyLabel fLabel)
{
	if (! _mFuzzyLabels.contains(fLabel.fuzzyId()))
	{
		_lastError = "Fuzzy lable with given fuzzy id is not present in the FMB";
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

		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not update fuzzy label in database: ", qry.lastError().text().toLatin1().data());
		return 2;
	}
}
int FMB::UpdateFuzzyApproxMuch(FuzzyApproxMuch fApprox)
{
	if (! _mFuzzyApproxMuch.contains(fApprox.columnId()) )
	{
		_lastError = "Fuzzy approximate and much into with given column id is not present in the FMB";
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
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not update fuzzy approximate and much info in database: ", qry.lastError().text().toLatin1().data());
		return 2;
	}		
}
int FMB::UpdateFuzzyNearness(FuzzyNearness fNns)
{
	if (! _mFuzzyNearness.contains(fNns.fuzzyId1()))
	{
		_lastError = "Fuzzy nearness info for given column ids is not present in the FMB";
		return 1;
	}
		
	QMap<int, FuzzyNearness*> *nMap = _mFuzzyNearness.value(fNns.fuzzyId1());

	if(! nMap->contains(fNns.fuzzyId2()))
	{
		_lastError = "Fuzzy nearness info for given column ids is not present in the FMB";
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
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not update fuzzy nearness definition in database: ", qry.lastError().text().toLatin1().data());
		return 3;
	}	
}

int FMB::UpdateFuzzyQualifier(FuzzyQualifier fQualif)
{
	if(! _mFuzzyQualifiers.contains(fQualif.fuzzyId()))
	{
		_lastError = "Fuzzy qualifier with given fuzzy id is not present in the FMB";
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("UPDATE fuzzy_qualifiers_def SET qualifier=:qualifier WHERE fuzzy_id = :fuzzyId ");
	qry.bindValue(":fuzzyId", fQualif.fuzzyId());					
	qry.bindValue(":qualifier", fQualif.qualifier());

	if(qry.exec())
	{
		_mFuzzyQualifiers.insert(fQualif.fuzzyId(), &fQualif);
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not update fuzzy qualifier in database: ", qry.lastError().text().toLatin1().data());
		return 2;
	}	
}

int FMB::UpdateFuzzyTableQuantifier(FuzzyTableQuantifier fTableQuantif)
{
	if(! _mFuzzyTableQuantifiers.contains(fTableQuantif.tableId()))
	{
		_lastError = "Fuzzy table quantifer with given fuzzy id is not present in the FMB";
		return 1;
	}

	if(! _mFuzzyTableQuantifiers.value(fTableQuantif.tableId())->contains(fTableQuantif.fuzzyName()))
	{
		_lastError = "Fuzzy table quantifer with given name is not present in the collection of fuzzy quantifers of table with given table id";
		return 2;
	}

	QSqlQuery qry(*_db);
	qry.prepare("UPDATE fuzzy_table_quantifiers SET \
				fuzzy_type=:fuzzyType, alpha=:alpha, beta=:beta, gamma=:gamma,\
				delta=:delta WHERE obj = :tableId AND fuzzy_name = :fuzzyName" );
	qry.bindValue(":tableId", fTableQuantif.tableId());
	qry.bindValue(":fuzzyType", fTableQuantif.fuzzyType());
	qry.bindValue(":fuzzyName", fTableQuantif.fuzzyName());
	qry.bindValue(":alpha", fTableQuantif.alpha());
	qry.bindValue(":beta", fTableQuantif.beta());
	qry.bindValue(":gamma", fTableQuantif.gamma());
	qry.bindValue(":delta", fTableQuantif.delta());

	if(qry.exec())
	{
		_mFuzzyTableQuantifiers.value(fTableQuantif.tableId())->insert(fTableQuantif.fuzzyName(), &fTableQuantif);
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not update fuzzy table quantifer in database: ", qry.lastError().text().toLatin1().data());
		return 3;
	}	
}
int FMB::UpdateFuzzySystemQuantifier(FuzzySystemQuantifier fSystemQuantif)
{
	if(! _mFuzzySystemQuantifiers.contains(fSystemQuantif.fuzzyName()))
	{
		_lastError = "Fuzzy system quantifier with given fuzzy id is not present in the FMB";
		return 1;
	}

	QSqlQuery qry(*_db);
	qry.prepare("UPDATE fuzzy_system_quantifiers SET fuzzy_type=:fuzzyType,\
				alpha=:alpha, beta=:beta, gamma=:gamma,\
				delta=:delta WHERE fuzzy_name = :fuzzyName");
	qry.bindValue(":fuzzyName", fSystemQuantif.fuzzyName());
	qry.bindValue(":fuzzyType", fSystemQuantif.fuzzyType());
	qry.bindValue(":alpha", fSystemQuantif.alpha());
	qry.bindValue(":beta", fSystemQuantif.beta());
	qry.bindValue(":gamma", fSystemQuantif.gamma());
	qry.bindValue(":delta", fSystemQuantif.delta());

	if(qry.exec())
	{
		_mFuzzySystemQuantifiers.remove(fSystemQuantif.fuzzyName());
		_lastError = "";
		return 0;
	}
	else
	{
		_lastError = strcat("Could not update fuzzy system quantifer in database: ", qry.lastError().text().toLatin1().data());
		return 2;
	}
}	
FuzzyTableInfo* FMB::GetFuzzyTableInfo(QString tableName)
{
	if (_mFuzzyTablesByName.contains(tableName))
		return _mFuzzyTablesByName.value(tableName);
	else
	{
		_lastError = "Fuzzy table with given name is not present in the FMB";
		return NULL;
	}
}
FuzzyTableInfo* FMB::GetFuzzyTableInfo(int tableId)
{
	if (_mFuzzyTables.contains(tableId))
		return _mFuzzyTables.value(tableId);
	else
	{
		_lastError = "Fuzzy table with given table id is not present in the FMB";
		return NULL;
	}
}
FuzzyCol* FMB::GetFuzzyCol(int columnId)
{
	if (_mFuzzyColumns.contains(columnId))
		return _mFuzzyColumns.value(columnId);
	else
	{
		_lastError = "Fuzzy column with given column id is not present in the FMB";
		return NULL;
	}
}
FuzzyCol* FMB::GetFuzzyCol(QString columnName)
{
	if (_mFuzzyColumnsByName.contains(columnName))
		return _mFuzzyColumnsByName.value(columnName);
	else
	{
		_lastError = "Fuzzy column with given column name is not present in the FMB";
		return NULL;
	}
}
FuzzyDegreeSig* FMB::GetFuzzyDegreeSig(QString significance)
{
	if (_mFuzzyDegreeSigByName.contains(significance))
		return _mFuzzyDegreeSigByName.value(significance);
	else
	{
		_lastError = "Fuzzy degree significance with given significance code is not present in the FMB";
		return NULL;
	}
}
FuzzyDegreeSig* FMB::GetFuzzyDegreeSig(int codeSig)
{
	if (_mFuzzyDegreeSig.contains(codeSig))
		return _mFuzzyDegreeSig.value(codeSig);
	else
	{
		_lastError = "Fuzzy degree significance with given significance code is not present in the FMB";
		return NULL;
	}
}
FuzzyObject* FMB::GetFuzzyObject(int fuzzyId)
{
	if (_mFuzzyObjects.contains(fuzzyId))
		return _mFuzzyObjects.value(fuzzyId);
	else
	{
		_lastError = "Fuzzy object with given fuzzy id is not present in the FMB";
		return NULL;
	}
}
FuzzyObject* FMB::GetFuzzyObject(QString fuzzyName)
{
	if (_mFuzzyObjectsByName.contains(fuzzyName))
		return _mFuzzyObjectsByName.value(fuzzyName);
	else
	{
		_lastError = "Fuzzy object with given name is not present in the FMB";
		return NULL;
	}
}
FuzzyLabel* FMB::GetFuzzyLabel(int fuzzyId)
{
	if (_mFuzzyLabels.contains(fuzzyId))
		return _mFuzzyLabels.value(fuzzyId);
	else
	{
		_lastError = "Fuzzy label with given fuzzy id is not present in the FMB";
		return NULL;
	}
}
FuzzyLabel* FMB::GetFuzzyLabel(QString fuzzy_name)
{
	if (_mFuzzyLabelsByName.contains(fuzzy_name))
		return _mFuzzyLabelsByName.value(fuzzy_name);
	else
	{
		_lastError = "Fuzzy label with given name is not present in the FMB";
		return NULL;
	}
}
FuzzyApproxMuch* FMB::GetFuzzyApproxMuch(int columnId)
{
	if (_mFuzzyApproxMuch.contains(columnId))
		return _mFuzzyApproxMuch.value(columnId);
	else
	{
		_lastError = "Fuzzy approximate and much info with given column id is not present in the FMB";
		return NULL;
	}
}
FuzzyNearness* FMB::GetFuzzyNearness(int fuzzyId1, int fuzzyId2)
{
	if (_mFuzzyNearness.contains(fuzzyId1) && 
		_mFuzzyNearness.value(fuzzyId1)->contains(fuzzyId2))
	return _mFuzzyNearness.value(fuzzyId1)->value(fuzzyId2);
	else
	{
		_lastError = "Fuzzy nearness for given fuzzy ids is not present in the FMB";
		return NULL;
	}
}
QList<int>* FMB::GetFuzzyCompatibleCols(int adapteeId)
{
	QList<int> *lAllColmatibleCols = new QList<int>();
	if (_mFuzzyCompatibleColumnAdaptees.contains(adapteeId))
		lAllColmatibleCols->append(*_mFuzzyCompatibleColumnAdapters.value(adapteeId));
	if (_mFuzzyCompatibleColumnAdaptees.contains(adapteeId))
		lAllColmatibleCols->append(*_mFuzzyCompatibleColumnAdaptees.value(adapteeId));

	return lAllColmatibleCols;
}
FuzzyQualifier* FMB::GetFuzzyQualifier(int fuzzyId)
{
	if (_mFuzzyQualifiers.contains(fuzzyId))
		return _mFuzzyQualifiers.value(fuzzyId);
	else
	{
		_lastError = "Fuzzy qualifier with given fuzzy ids is not present in the FMB";
		return NULL;
	}
}
int FMB::GetFuzzyDegreeCol(int columnId)
{
	if (_mFuzzyDegreeCols.contains(columnId))
		return _mFuzzyDegreeCols.value(columnId);
	else
	{
		_lastError = "Fuzzy degree column with given column id is not present in the FMB";
		return NULL;
	}
}
FuzzyDegreeTable* FMB::GetFuzzyDegreeTable(int tableId)
{
	if (_mFuzzyDegreeTables.contains(tableId))
		return _mFuzzyDegreeTables.value(tableId);
	else
	{
		_lastError = "Fuzzy degree column for table with given table id is not present in the FMB";
		return NULL;
	}
}
FuzzyTableQuantifier* FMB::GetFuzzyTableQuantifier(int tableId, QString fuzzyName)
{
	if (_mFuzzyTableQuantifiers.contains(tableId) && _mFuzzyTableQuantifiers.value(tableId)->contains(fuzzyName))
		return _mFuzzyTableQuantifiers.value(tableId)->value(fuzzyName);
	else
	{
		_lastError = "Fuzzy table quantifer for given name and table id is not present in the FMB";
		return NULL;
	}
	
}
FuzzySystemQuantifier* FMB::GetFuzzySystemQuantifier(QString fuzzyName)
{
	if (_mFuzzySystemQuantifiers.contains(fuzzyName))
		return _mFuzzySystemQuantifiers.value(fuzzyName);
	else
	{
		_lastError = "Fuzzy system quantifer with given name is not present in the FMB";
		return NULL;
	}
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

	if(query.exec("INSERT INTO FUZZY_META_TABLES(\"name\") VALUES(\'" + tableName + "\') RETURNING table_id"))
	{
		query.next();
		FuzzyTableInfo *nel = new FuzzyTableInfo(query.value(0).toInt(), tableName);
		_mFuzzyTables.insert(query.value(0).toInt(), nel);
		_mFuzzyTablesByName.insert(tableName, nel);
	}
	else
	{
		_lastError = strcat("Could not create fuzzy table info in database: ", query.lastError().text().toLatin1().data());			
	}
}

void FMB::CreateFuzzyCol(int tableId, int f_type, int len, int code_sig, 
							  QString columnName, QString com, QString um)
{
	 QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_COL_LIST (obj, f_type, len, code_sig, column_name, com, um) "
		 " VALUES (:tableId, :f_type, :len, :code_sig, :columnName, :com, :um) RETURNING col");     
	 query.bindValue(":tableId", tableId);
     query.bindValue(":f_type", f_type);
     query.bindValue(":len", len);
	 query.bindValue(":code_sig", code_sig);
	 query.bindValue(":columnName", columnName);
	 query.bindValue(":com", com);
	 query.bindValue(":um", um);
     if(query.exec())
	 {
		 query.next();
		 FuzzyCol* nel = new FuzzyCol(tableId, query.value(0).toInt(), f_type, len, code_sig, columnName, com, um );
		 _mFuzzyColumns.insert(query.value(0).toInt(), nel);
		 _mFuzzyColumnsByName.insert(columnName, nel);
	 }
	 else
	{
		_lastError = strcat("Could not create fuzzy column info in database: ", query.lastError().text().toLatin1().data());			
	}
}

void FMB::CreateFuzzyDegreeSig(QString significance)
{
	 QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_DEGREE_SIG(significance) "
		 "VALUES (:significance) RETURNING code_sig");     
     query.bindValue(":significance", significance);
     if(query.exec())
	 {
		 query.next();
		 FuzzyDegreeSig *nel = new FuzzyDegreeSig(query.value(0).toInt(), significance);
		 _mFuzzyDegreeSig.insert(query.value(0).toInt(), nel);
		_mFuzzyDegreeSigByName.insert(significance, nel);
	 }
	 	else
	{
		_lastError = strcat("Could not create fuzzy table info in database: ", query.lastError().text().toLatin1().data());			
	}
}

void FMB::CreateFuzzyObject(int columnId, QString fuzzy_name, int fuzzy_type)
{
	QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_OBJECT_LIST(col, fuzzy_name, fuzzy_type) "
		 "VALUES (:column, :fuzzy_name, :fuzzy_type) RETURNING fuzzy_id");
     query.bindValue(":column", columnId);
     query.bindValue(":fuzzy_name", fuzzy_name);
     query.bindValue(":fuzzy_type", fuzzy_type);
     if(query.exec())
	 {
		 query.next();
		 FuzzyObject *nel = new FuzzyObject(columnId, query.value(0).toInt(), fuzzy_name, fuzzy_type);
		 _mFuzzyObjects.insert(query.value(0).toInt(), nel);
		 _mFuzzyObjectsByName.insert(fuzzy_name, nel);
	 }
	 else
	{
		_lastError = strcat("Could not create fuzzy object info in database: ", query.lastError().text().toLatin1().data());			
	}
}

void FMB::CreateFuzzyLabelDef(int fuzzy_id, 
		double alpha, double beta, double gamma, double delta)
{
	if (!_mFuzzyObjects.contains(fuzzy_id))
	{
		return;
	}

	QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_LABEL_DEF "
		 "VALUES (:fuzzy_id, :alpha, :beta, :gamma, :delta)");     
     query.bindValue(":fuzzy_id", fuzzy_id);
     query.bindValue(":alpha", alpha);
	 query.bindValue(":beta", beta);
	 query.bindValue(":gamma", gamma);
	 query.bindValue(":delta", delta);
	 if(query.exec())
	 {
		  FuzzyLabel* fLab = new FuzzyLabel(fuzzy_id, alpha, beta, gamma, delta);
		  FuzzyObject *fLabObj = _mFuzzyObjects.value(fuzzy_id);

		 _mFuzzyLabels.insert(fuzzy_id, fLab);
		 _mFuzzyLabelsByName.insert(fLabObj->fuzzyName(), fLab);		 
	 }
	 else
	 {
		 _lastError = strcat("Could not create fuzzy label in database: ", query.lastError().text().toLatin1().data());			
	 }
}

void FMB::CreateFuzzyApproxMuch(int columnId, double margin, double much)
{
	QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_APPROX_MUCH "
		 "VALUES (:column, :margin, :much)");
     query.bindValue(":column", columnId);
     query.bindValue(":margin", margin);
     query.bindValue(":much", much);
     if(query.exec())
	 {
		 FuzzyApproxMuch *nel = new FuzzyApproxMuch(columnId, margin, much);
		_mFuzzyApproxMuch.insert(columnId, nel);
	 }
	 	 else
	 {
		 _lastError = strcat("Could not create fuzzy approx much info in database: ", query.lastError().text().toLatin1().data());			
	 }
}

void FMB::CreateFuzzyNearness( int fuzzy_id1, int fuzzy_id2,
		double degree)
{
	QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_NEARNESS_DEF "
		 "VALUES (:fuzzy_id1, :fuzzy_id2, :degree)");
     query.bindValue(":fuzzy_id1", fuzzy_id1);
     query.bindValue(":fuzzy_id2", fuzzy_id2);
	 query.bindValue(":degree", degree);
	 if (query.exec())
	 {
		 if (!_mFuzzyNearness.contains(fuzzy_id1))
		 {
			 QMap<int, FuzzyNearness*> *nel = new QMap<int, FuzzyNearness*>();
			 _mFuzzyNearness.insert(fuzzy_id1, nel);
		 }

		 FuzzyNearness *nel = new FuzzyNearness(fuzzy_id1, fuzzy_id2, degree);
		 _mFuzzyNearness.value(fuzzy_id1)->insert(fuzzy_id2, nel);
	 }
	 else
	 {
		 _lastError = strcat("Could not create fuzzy nearness in database: ", query.lastError().text().toLatin1().data());			
	 }
}


void FMB::CreateFuzzyCompatibleCol(int col1, int col2)
{
	 QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_COMPATIBLE_COL "
		 "VALUES (:column1, :column2)");
     query.bindValue(":col1", col1);
	 query.bindValue(":col2", col2);     

     if(query.exec())
	 {
		 if (!_mFuzzyCompatibleColumnAdaptees.contains(col2))
		 {
			 QList<int> *nel = new QList<int>();
			 _mFuzzyCompatibleColumnAdaptees.insert(col2, nel);
		 }

		 _mFuzzyCompatibleColumnAdaptees.value(col2)->append(col1);

		 if (!_mFuzzyCompatibleColumnAdapters.contains(col1))
		 {
			 QList<int> *nel = new QList<int>();
			 _mFuzzyCompatibleColumnAdapters.insert(col1, nel);
		 }

		 _mFuzzyCompatibleColumnAdapters.value(col1)->append(col2);
	 }
	  else
	 {
		 _lastError = strcat("Could not create fuzzy compatible columns in database: ", query.lastError().text().toLatin1().data());			
	 }
}

void FMB::CreateFuzzyQualifier(int fuzzy_id, double qualifier)
{
	 QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_QUALIFIERS_DEF "
		 "VALUES (:fuzzy_id, :qualifier)");     
	 query.bindValue(":fuzzy_id", fuzzy_id);     
	 query.bindValue(":qualifier", qualifier);     

     if(query.exec())
	 {
		 FuzzyQualifier *nel = new FuzzyQualifier(fuzzy_id, qualifier);
		 _mFuzzyQualifiers.insert(fuzzy_id, nel);
	 }
	 else
	 {
		 _lastError = strcat("Could not create fuzzy qualifier in database: ", query.lastError().text().toLatin1().data());			
	 }
}

void FMB::CreateFuzzyDegreeCol(int col1, int col2)
{
		 QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_DEGREE_COLS "
		 "VALUES (:column1, :column2)");
     query.bindValue(":column1", col1);
	 query.bindValue(":column2", col2);     

     if(query.exec())
	 {
		 _mFuzzyDegreeCols.insert(col1, col2);
	 }
	 else
	 {
		 _lastError = strcat("Could not create fuzzy degree column in database: ", query.lastError().text().toLatin1().data());			
	 }
}

void FMB::CreateFuzzyDegreeTable(int tableId, int columnId, QChar degree_type)
{
		 QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_DEGREE_TABLE "
		 "VALUES (:table_id, :column_id, :deree_type)");
	 query.bindValue(":table_id",  tableId);
     query.bindValue(":column_id",  columnId);
	 query.bindValue(":degree_type", degree_type);     
     if(query.exec())
	 {
		 FuzzyDegreeTable *nel = new FuzzyDegreeTable(tableId, columnId, degree_type);
		 _mFuzzyDegreeTables.insert(tableId, nel);
	 }
	 else
	 {
		 _lastError = strcat("Could not create fuzzy degree for table in database: ", query.lastError().text().toLatin1().data());			
	 }
}

void FMB::CreateFuzzyTableQuantifier(int tableId, QString fuzzy_name, int fuzzy_type, 
		double alpha, double beta, double gamma, double delta)
{
	 QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_TABLE_QUANTIFIERS "
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
			 QMap<QString, FuzzyTableQuantifier*> *nel = new QMap<QString, FuzzyTableQuantifier*>();
			 _mFuzzyTableQuantifiers.insert(tableId, nel);
		 }
		 FuzzyTableQuantifier *nel = new FuzzyTableQuantifier(tableId, fuzzy_name, fuzzy_type, alpha, beta, gamma, delta);
		 _mFuzzyTableQuantifiers.value(tableId)->insert(fuzzy_name, nel);
	 }
	 else
	 {
		 _lastError = strcat("Could not create fuzzy table quantifier in database: ", query.lastError().text().toLatin1().data());			
	 }
}

void FMB::CreateFuzzySystemQuantifier(QString fuzzy_name, int fuzzy_type, double alpha,
		double beta, double gamma, double delta)
{
	 QSqlQuery query(*_db);
     query.prepare("INSERT INTO FUZZY_SYSTEM_QUANTIFIERS "
		 "VALUES (:fuzzy_name, :fuzzy_type, :alpha, :beta, :gamma, :delta)");     
	 query.bindValue(":fuzzy_name", fuzzy_name);     
	 query.bindValue(":fuzzy_type", fuzzy_type);  
	 query.bindValue(":alpha", alpha);
	 query.bindValue(":beta", beta);
	 query.bindValue(":gamma", gamma);
	 query.bindValue(":delta", delta);
     if(query.exec())
	 {
		 FuzzySystemQuantifier *nel = new FuzzySystemQuantifier(fuzzy_name, fuzzy_type, alpha, beta, gamma, delta);
		 _mFuzzySystemQuantifiers.insert(fuzzy_name, nel);
	 }
	 else
	 {
		 _lastError = strcat("Could not create fuzzy system quantifier in database: ", query.lastError().text().toLatin1().data());			
	 }
}


FMB::FMB(void)
{
}

FMB::~FMB(void)
{
}