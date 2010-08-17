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

#include "FuzzyTableInfo.h"

FuzzyTableInfo::FuzzyTableInfo(void)
{
}

FuzzyTableInfo::FuzzyTableInfo(int id, QString name)
{
        _tableId = id;
        _name = name;
}


FuzzyTableInfo::FuzzyTableInfo(QMap<QString, QVariant> *mValues)
{
    if (mValues->contains("name"))
        _name = mValues->value("name").toString();
    if (mValues->contains("tableId"))
        _tableId = mValues->value("tableId").toInt();
}


FuzzyTableInfo::~FuzzyTableInfo(void)
{
}

int FuzzyTableInfo::tableId() { return _tableId; }

QString FuzzyTableInfo::name() { return _name; }

QMap<QString, QVariant>* FuzzyTableInfo::ValuesMap()
{
    QMap<QString, QVariant> *pMap = new QMap<QString, QVariant>();

    pMap->insert("tableId", QVariant(_tableId));
    pMap->insert("name", QVariant(_name));

    return pMap;
}

