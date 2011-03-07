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

#ifndef FUZZY_DEGREE_COL_H
#define FUZZY_DEGREE_COL_H

#include "QtIncludes.h"

class FSQL_EXPORT FuzzyDegreeCol
{
	int _columnId1;
	int _columnId2;

public:
	int columnId1();
	int columnId2();

	FuzzyDegreeCol(QSqlQuery qry);
	FuzzyDegreeCol(int columnId1, int columnId2);
    FuzzyDegreeCol(QMap<QString, QVariant> *mValues);
	FuzzyDegreeCol(void);
	~FuzzyDegreeCol(void);
};

#endif // FUZZY_DEGREE_COL_H