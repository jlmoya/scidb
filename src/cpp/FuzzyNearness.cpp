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

#include "FuzzyNearness.h"

FuzzyNearness::FuzzyNearness(QSqlQuery qry)
{
	_fuzzyId1 = qry.value(qry.record().indexOf("fuzzy_id1")).toInt();
	_fuzzyId2 = qry.value(qry.record().indexOf("fuzzy_id2")).toInt();
	_degree = qry.value(qry.record().indexOf("degree")).toDouble();
}

FuzzyNearness::FuzzyNearness(	int fuzzyId1,
					int fuzzyId2,
					double degree)
{
	_fuzzyId1 = fuzzyId1;
	_fuzzyId2 = fuzzyId2;
	_degree = degree;
}
					
FuzzyNearness::FuzzyNearness(QMap<QString, QVariant> *mValues)
{
	if (mValues->contains("fuzzyId1"))
		_fuzzyId1 = mValues->value("fuzzyId1").toInt();

	if (mValues->contains("fuzzyId2"))
		_fuzzyId2 = mValues->value("fuzzyId2").toInt();

	if (mValues->contains("degree"))
		_degree = mValues->value("degree").toDouble();

}

FuzzyNearness::FuzzyNearness(void)
{
}

FuzzyNearness::~FuzzyNearness(void)
{
}

int FuzzyNearness::fuzzyId1() { return _fuzzyId1; }
int FuzzyNearness::fuzzyId2() { return _fuzzyId2; }
double FuzzyNearness::degree() { return _degree; }

QMap<QString, QVariant>* FuzzyNearness::ValuesMap()
{
	QMap<QString, QVariant> *pMap = new QMap<QString, QVariant>();

	pMap->insert("fuzzyId1", QVariant(_fuzzyId1));
	pMap->insert("fuzzyId2", QVariant(_fuzzyId2));
	pMap->insert("degree", QVariant(_degree));			

	return pMap;
}