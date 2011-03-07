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

#ifndef FUZZY_NEARNESS_H
#define FUZZY_NEARNESS_H

#include "QtIncludes.h"

class FSQL_EXPORT FuzzyNearness
{
	int _fuzzyId1;
	int _fuzzyId2;
	double _degree;

public:
	int fuzzyId1();
	int fuzzyId2();
	double degree();

	FuzzyNearness(QSqlQuery qry);
	FuzzyNearness(	int fuzzyId1,
					int fuzzyId2,
					double degree);
	FuzzyNearness(QMap<QString, QVariant> *mValues);
	FuzzyNearness(void);
	~FuzzyNearness(void);

	QMap<QString, QVariant>* ValuesMap();	
};
#endif // FUZZY_NEARNESS_H