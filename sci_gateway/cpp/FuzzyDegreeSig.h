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

class __declspec( dllexport ) FuzzyDegreeSig
{
    int _codeSig;
    QString _significance;

public:
    int codeSig();
    QString significance();

    FuzzyDegreeSig(QSqlQuery qry);
    FuzzyDegreeSig(    int codeSig,
                    QString significance);
    FuzzyDegreeSig(QMap<QString, QVariant> *mValues);
    FuzzyDegreeSig(void);
    ~FuzzyDegreeSig(void);

    QMap<QString, QVariant>* ValuesMap();
};
