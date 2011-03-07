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

#ifndef FUZZY_SYSTEM_QUANTIFIER_H
#define FUZZY_SYSTEM_QUANTIFIER_H

#include "QtIncludes.h"

class FSQL_EXPORT FuzzySystemQuantifier
{
	QString _fuzzyName;
	int _fuzzyType;
	double _alpha, _beta, _gamma, _delta;

public:

	QString fuzzyName();
	int fuzzyType();

	double alpha();
	double beta();
	double gamma();
	double delta();

	FuzzySystemQuantifier(QSqlQuery qry);
	FuzzySystemQuantifier(	QString fuzzyName,
							int fuzzyType,
							double alpha, 
							double beta, 
							double gamma, 
							double delta);
	FuzzySystemQuantifier(QMap<QString, QVariant> *mValues);
	FuzzySystemQuantifier(void);
	~FuzzySystemQuantifier(void);

	QMap<QString, QVariant>* ValuesMap();	
};

#endif // FUZZY_SYSTEM_QUANTIFIER_H