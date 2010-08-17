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

#include "FuzzyDegreeTable.h"

FuzzyDegreeTable::FuzzyDegreeTable(QSqlQuery qry)
{
    _tableId =  qry.value(qry.record().indexOf("obj")).toInt();
    _columnId =  qry.value(qry.record().indexOf("col")).toInt();
    _degreeType =  qry.value(qry.record().indexOf("degree_type")).toChar();
}

FuzzyDegreeTable::FuzzyDegreeTable(    int tableId,
                        int columnId,
                        QChar degreeType)
{
    _tableId = tableId;
    _columnId = columnId;
    _degreeType = degreeType;
}

FuzzyDegreeTable::FuzzyDegreeTable(QMap<QString, QVariant> *mValues)
{
    if (mValues->contains("tableId"))
        _tableId = mValues->value("tableId").toInt();
    if (mValues->contains("columnId"))
        _columnId = mValues->value("columnId").toInt();
    if (mValues->contains("degreeType"))
        _degreeType = mValues->value("degreeType").toChar();



}

FuzzyDegreeTable::FuzzyDegreeTable(void)
{
}

FuzzyDegreeTable::~FuzzyDegreeTable(void)
{
}

int FuzzyDegreeTable::tableId(){ return _tableId; }
int FuzzyDegreeTable::columnId(){ return _columnId; }
QChar FuzzyDegreeTable::degreeType(){ return _degreeType; }

QMap<QString, QVariant>* FuzzyDegreeTable::ValuesMap()
{
    QMap<QString, QVariant> *pMap = new QMap<QString, QVariant>();

    pMap->insert("tableId", QVariant(_tableId));
    pMap->insert("columnId", QVariant(_columnId));
    pMap->insert("degreeType", QVariant(_degreeType));

    return pMap;
}
