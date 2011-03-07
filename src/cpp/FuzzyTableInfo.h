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

#ifndef FUZZY_TABLE_INFO_H
#define FUZZY_TABLE_INFO_H

#include "QtIncludes.h"

class FSQL_EXPORT FuzzyTableInfo
{
	int _tableId;
	QString _name;

public:
	int tableId();
	QString name();

	FuzzyTableInfo(void);
	FuzzyTableInfo(int id, QString name);
	FuzzyTableInfo(QMap<QString, QVariant> *mValues);

	~FuzzyTableInfo(void);

	QMap<QString, QVariant>* ValuesMap();	
};

#endif // FUZZY_TABLE_INFO_H