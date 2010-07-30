#pragma once
#include <QtCore>
#include <QtSql>

class __declspec( dllexport ) FuzzyDegreeTable
{
	int _tableId;
	int _columnId;
	QChar _degreeType;

public:
	int tableId();
	int columnId();
	QChar degreeType();

	FuzzyDegreeTable(QSqlQuery qry);
	FuzzyDegreeTable(void);
	~FuzzyDegreeTable(void);
};
