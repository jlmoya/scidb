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

#ifndef FUZZY_DEGREE_TABLE_H
#define FUZZY_DEGREE_TABLE_H

#include "QtIncludes.h"

class FSQL_EXPORT FuzzyDegreeTable
{
	int _tableId;
	int _columnId;
	QChar _degreeType;

public:
	int tableId();
	int columnId();
	QChar degreeType();

	FuzzyDegreeTable(QSqlQuery qry);
	FuzzyDegreeTable(	int tableId,
						int columnId,
						QChar degreeType);
	FuzzyDegreeTable(QMap<QString, QVariant> *mValues);
	FuzzyDegreeTable(void);
	~FuzzyDegreeTable(void);

	QMap<QString, QVariant>* ValuesMap();	
};

#endif // FUZZY_DEGREE_TABLE_H