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

#ifndef FUZZY_COMPATIBLE_COL_H
#define FUZZY_COMPATIBLE_COL_H

#include "QtIncludes.h"

class FSQL_EXPORT FuzzyCompatibleCol
{
	int _columnId1, _columnId2;

public:
	int columnId1();
	int columnId2();

	FuzzyCompatibleCol(QSqlQuery qry);
	FuzzyCompatibleCol(int columnId1, int columnId2);
    FuzzyCompatibleCol(QMap<QString, QVariant> *mValues);
	FuzzyCompatibleCol(void);
	~FuzzyCompatibleCol(void);

	QMap<QString, QVariant>* ValuesMap();	
};

#endif // FUZZY_COMPATIBLE_COL_H