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

#ifndef FUZZYSQL_H
#define FUZZYSQL_H

#include "TreeTokenNode.h"
#include "FuzzyConstant.h"

#include "QtIncludes.h"

#include "FMB.h" 

class FSQL_EXPORT FuzzySQL {
private:
    QSqlDatabase *_dataBase;
	FMB _fmb;
public:
	QSqlDatabase * DataBase();
	FMB *FuzzyMetaBase();
	QString LastError();

    FuzzySQL();
	FuzzySQL(QSqlDatabase *db);	

    QString FSQL2SQL(QString queryString, QString *error);

    void CreateFuzzyMetaBaseOnDataBase(QSqlDatabase *db);
	void CreateFuzzyMetaBaseOnDataBase();


	






	//============================================================//
	int FuzzySQL::OperandType(QString operand);
	QList<QString> FuzzySQL::CallParameters(QString sArgument, int iArgType, QString marginAttribute);
	QString FuzzySQL::GetFullColumnName(QString operand, QString query, int iOperandStartIndex);
};

#endif // FUZZYSQL_H
