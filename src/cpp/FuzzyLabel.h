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

#ifndef FUZZY_LABEL_H
#define FUZZY_LABEL_H

#include "QtIncludes.h"

class FSQL_EXPORT FuzzyLabel
{
	int _fuzzyId;
	double _alpha, _beta, _gamma, _delta;

public:
	int fuzzyId();
	double alpha();
	double beta();
	double gamma();
	double delta();

	FuzzyLabel(QSqlQuery qry);
	FuzzyLabel(	int fuzzyId,
				double alpha, double beta, double gamma, double delta);
	FuzzyLabel(QMap<QString, QVariant> *mValues);
	FuzzyLabel(void);
	~FuzzyLabel(void);

	void SetValues(double alpha, double beta, double gamma, double delta);

	QMap<QString, QVariant>* ValuesMap();	
};

#endif // FUZZY_LABEL_H
