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

#ifndef FUZZY_COL_H
#define FUZZY_COL_H

#include "QtIncludes.h"

class FSQL_EXPORT FuzzyCol
{
	int _tableId;
	int _columnId;
	QString _name;
	//1-8 (1-4 - attributes, 5-7 - degrees
	int _fType;	
	//for types 3-4
	int _len;
	//code of meaning of degree;
	int _codeSig;
	QString _columnName;
	QString _com;
	//unit of measurement
	QString _um;

public:
	int tableId();
	int columnId();	
	int fType();	
	int len();	
	int codeSig();
	QString columnName();
	QString com();	
	QString um();

	FuzzyCol(QSqlQuery qry);
	FuzzyCol(	int tableId,
				int columnId,								
				int fType,		
				int len,	
				int codeSig,
				QString columnName,
				QString com,	
				QString um);
    FuzzyCol(QMap<QString, QVariant> *mValues);
	FuzzyCol(void);
	~FuzzyCol(void);

	QMap<QString, QVariant>* FuzzyCol::ValuesMap();
};

#endif // FUZZY_COL_H