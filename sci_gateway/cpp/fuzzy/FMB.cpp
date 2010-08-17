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

		_mFuzzyLables.insert(fLbl.fuzzyId(), fLbl);
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
			QList<FuzzyNearness> nList;

			_mFuzzyNearness.insert(fNns.fuzzyId1(), nList);
		}

		QList<FuzzyNearness> fnList = _mFuzzyNearness.value(fNns.fuzzyId1());
		fnList.append(fNns);		
	}

	qry.exec("SELECT * FROM fuzzy_compatible_col");
	while(qry.next())
	{
		FuzzyCompatibleCol fCompat(qry);

		if (!_mFuzzyCompatibleColumnAdapters.contains(fCompat.columnId1()))
		{
			QList<FuzzyCompatibleCol> fCompList;

			_mFuzzyCompatibleColumnAdapters.insert(fCompat.columnId1(), fCompList);
		}

		QList<FuzzyCompatibleCol> fCompList = _mFuzzyCompatibleColumnAdapters.value(fCompat.columnId1());
		fCompList.append(fCompat);
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
		_mFuzzyDegreesAssociatedColumns.insert(qry.value(qry.record().indexOf("col1")).toInt(), 
			qry.value(qry.record().indexOf("col2")).toInt());		
	}

	qry.exec("SELECT * FROM fuzzy_degree_table");
	while(qry.next())
	{
		FuzzyDegreeTable fDegTable(qry);						

		_mFuzzyyTableDegrees.insert(fDegTable.tableId(), fDegTable);
	}

	qry.exec("SELECT * FROM fuzzy_table_quantifiers");
	while(qry.next())
	{
		FuzzyTableQuantifier fTabQ(qry);

		_mFuzzyTableQuantifiers.insert(fTabQ.fuzzyName(), fTabQ);
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

FMB::FMB(void)
{
}

FMB::~FMB(void)
{
}