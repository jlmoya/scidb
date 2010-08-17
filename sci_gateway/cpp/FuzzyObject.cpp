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

#include "FuzzyObject.h"

FuzzyObject::FuzzyObject(QSqlQuery qry)
{
    _columnId = qry.value(qry.record().indexOf("col")).toInt();
    _fuzzyId = qry.value(qry.record().indexOf("fuzzy_id")).toInt();
    _fuzzyName  = qry.value(qry.record().indexOf("fuzzy_name")).toString();
    _fuzzyType = qry.value(qry.record().indexOf("fuzzy_type")).toInt();
}

FuzzyObject::FuzzyObject(int columnId,
                int fuzzyId,
                QString fuzzyName,
                int fuzzyType)
{
    _columnId = columnId;
    _fuzzyId = fuzzyId;
    _fuzzyName = fuzzyName;
    _fuzzyType = fuzzyType;
}

FuzzyObject::FuzzyObject(QMap<QString, QVariant> *mValues)
{
    if (mValues->contains("columnId"))
        _columnId = mValues->value("columnId").toInt();

    if (mValues->contains("fuzzyId"))
        _fuzzyId = mValues->value("fuzzyId").toInt();

    if (mValues->contains("fuzzyName"))
        _fuzzyName = mValues->value("fuzzyName").toString();

    if (mValues->contains("fuzzyType"))
        _fuzzyType = mValues->value("fuzzyType").toInt();
}

FuzzyObject::FuzzyObject(void)
{
}

FuzzyObject::~FuzzyObject(void)
{
}

int FuzzyObject::columnId() {return _columnId; }
int FuzzyObject::fuzzyId() { return _fuzzyId; }
QString FuzzyObject::fuzzyName() { return _fuzzyName; }
int FuzzyObject::fuzzyType() { return _fuzzyType; }

QMap<QString, QVariant>* FuzzyObject::ValuesMap()
{
    QMap<QString, QVariant> *pMap = new QMap<QString, QVariant>();

    pMap->insert("columnId", QVariant(_columnId));
    pMap->insert("fuzzyId", QVariant(_fuzzyId));
    pMap->insert("fuzzyName", QVariant(_fuzzyName));
    pMap->insert("fuzzyType", QVariant(_fuzzyType));

    return pMap;
}
