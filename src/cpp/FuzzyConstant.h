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

#ifndef FUZZY_CONSTANT_H
#define FUZZY_CONSTANT_H

#include "QtIncludes.h"
#include "FMB.h"
#include "FuzzyLabel.h"

class FSQL_EXPORT FuzzyConstant
{
public:
    FuzzyConstant();    
    FuzzyConstant(QString str, FMB fuzzyMetaBase);

    int type;
    double* data;
	QList<QString> labels;
};

#endif // FUZZY_CONSTANT_H
