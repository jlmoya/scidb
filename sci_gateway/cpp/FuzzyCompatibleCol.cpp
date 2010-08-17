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

#include "FuzzyCompatibleCol.h"

FuzzyCompatibleCol::FuzzyCompatibleCol(QSqlQuery qry)
{
        _columnId1 = qry.value(qry.record().indexOf("col1")).toInt();
        _columnId2 = qry.value(qry.record().indexOf("col2")).toInt();
}

FuzzyCompatibleCol::FuzzyCompatibleCol(int columnId1, int columnId2)
{
    _columnId1 = columnId1;
    _columnId2 = columnId2;
}

FuzzyCompatibleCol::FuzzyCompatibleCol(QMap<QString, QVariant> *mValues)
{
    if (mValues->contains("columnId1"))
        _columnId1 = mValues->value("columnId1").toInt();

    if (mValues->contains("columnId2"))
        _columnId2 = mValues->value("columnId2").toInt();


}

FuzzyCompatibleCol::FuzzyCompatibleCol(void)
{
}

FuzzyCompatibleCol::~FuzzyCompatibleCol(void)
{
}
int FuzzyCompatibleCol::columnId1(){ return _columnId1; }
int FuzzyCompatibleCol::columnId2(){ return _columnId2; }

QMap<QString, QVariant>* FuzzyCompatibleCol::ValuesMap()
{
    QMap<QString, QVariant> *pMap = new QMap<QString, QVariant>();

    pMap->insert("columnId1", QVariant(_columnId1));
    pMap->insert("columnId2", QVariant(_columnId1));

    return pMap;
}
