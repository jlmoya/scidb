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

#include "FuzzyDegreeCol.h"

FuzzyDegreeCol::FuzzyDegreeCol(int columnId1, int columnId2)
{
    _columnId1 = columnId1;
    _columnId2 = columnId2;
}

FuzzyDegreeCol::FuzzyDegreeCol(QMap<QString, QVariant> *mValues)
{
    if (mValues->contains("columnId1"))
        _columnId1 = mValues->value("columnId1").toInt();

    if (mValues->contains("columnId2"))
        _columnId2 = mValues->value("columnId2").toInt();
}

FuzzyDegreeCol::FuzzyDegreeCol(void)
{
}

FuzzyDegreeCol::~FuzzyDegreeCol(void)
{
}

int FuzzyDegreeCol::columnId1() { return _columnId1; }
int FuzzyDegreeCol::columnId2() { return _columnId2; }
