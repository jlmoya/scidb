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

#pragma once
#include "QtIncludes.h"

class __declspec( dllexport ) FuzzyApproxMuch
{
    int _columnId;
    double _margin;
    double _much;

public:
    int columnId();
    double margin();
    double much();

    FuzzyApproxMuch(QSqlQuery qry);

    FuzzyApproxMuch(int columnId,
                    double margin,
                    double much);


    FuzzyApproxMuch(QMap<QString, QVariant> *mValues);


    FuzzyApproxMuch(void);
    ~FuzzyApproxMuch(void);

    QMap<QString, QVariant>* ValuesMap();
};
