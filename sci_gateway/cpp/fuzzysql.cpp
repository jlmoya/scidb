#include "fuzzysql.h"

QMap<QString ,QString> functions;

QSqlDatabase *FuzzySQL::DataBase()
{
	return _dataBase;
}

FMB *FuzzySQL::FuzzyMetaBase()
{
	return &_fmb;
}

//specific for database,FMB must be loaded in constructor
FuzzySQL::FuzzySQL()
{
    functions[" FEQ "] = "feq";
    functions[" F= "] = "feq";
    functions[" FGT "] = "fgt";
    functions[" F> "] = "fgt";
    functions[" FGEQ "] = "fgeq";
    functions[" F>= "] = "fgeq";
    functions[" FLT "] = "flt";
    functions[" F< "] = "flt";
    functions[" FLEQ "] = "fleq";
    functions[" F<= "] = "fleq";
    functions[" FDIF "] = "fdif";
    functions[" NFEQ "] = "nfeq";
    functions[" NF= "] = "nfeq";
    functions[" NFGT "] = "nfgt";
    functions[" NF> "] = "nfgt";
    functions[" NFGEQ "] = "nfgeq";
    functions[" NF>= "] = "nfgeq";
    functions[" NFLT "] = "nflt";
    functions[" NF< "] = "nflt";
    functions[" NFLEQ "] = "nfleq";
    functions[" NF<= "] = "nfleq";
    functions[" NFDIF "] = "nfdif";
}

FuzzySQL::FuzzySQL(QSqlDatabase *db)
{
	_dataBase = db;

	_fmb.SetDb(db);
	_fmb.LoadFMB();
}

//LEFT OPERAND MUST BE COLUMN!
QString LeftOperand(QString query, int position, int *start)
{
    QString res = QString(query[position]);
    while(query[--position] != ' ')
    {
        res.insert(0, query[position]);
    }

    start = new int;
    *start = position;

    return res;
}

QString RightOperand(QString query, int position, int *end)
{
    QString res = QString(query[position]);
    while(query[++position] != ' ')
    {
        res.append(query[position]);
    }

    end = new int;
    *end = position;

    return res;
}

QString NextWord(QString str, int from, int *end)
{
    while(QString(" \n\t").contains(str.at(++from)));

    int start = from;

    while(!QString(" \n\t").contains(str.at(++from)));

    end = new int;
    *end = from;

    return str.mid(start, from - start);
}

bool IsCrispOperator(QString str)
{
    return str == ">" || str == "<" || str == "<=" || str == ">=" || str == "<>";
}

QString FuzzyOperatorCallReplacement(QString qry, int *start)
{
    QList<QString> funs = functions.values();
    for (int i = 0; i < funs.size(); ++i)
    {
        if(qry.contains(funs[i]) )
        {
            *start = qry.indexOf(funs[i]);
            return functions[funs[i]];
        }
    }

    *start = -1;
    return NULL;
}

int FirstSELECTPositionToLeftFrom(QString qry, int from)
{
    int start = 0, prev = -1;

    start = qry.indexOf("SELECT");

    while (start > from)
    {
        prev = start;
        start = qry.indexOf("SELECT");
    }

    return prev;
}

int FuzzySQL::OperandType(QString operand)
{
	if (operand.contains('+') ||
		operand.contains('-') ||
		operand.contains('*') ||
		operand.contains('/'))
	{
		return 7;
	}

	//trying to parse a fuzzy object out of it
	FuzzyConstant operConst(operand, _fmb);
	
	if (operConst.type >= 0)
	{
		return 10 + operConst.type;
	}

	//now considering we have a fuzzy column
	if (operand.contains('.'))
	{
		QStringList slOperParts = operand.split('.');

		if (slOperParts.length() > 2)
		{
			return 0;
		}

		if (!_fmb._mFuzzyTablesByName.contains(slOperParts.at(0)) ||
			!_fmb._mFuzzyColumnsByName.contains(slOperParts.at(1)))
		{
			return 10;
		}	
		else
		{
			FuzzyCol *col = _fmb._mFuzzyColumnsByName.value(slOperParts.at(1));
			
			return col->fType();
		}
	}
	else
	{
		if(!_fmb._mFuzzyColumnsByName.contains(operand))
		{
			return 10;
		}
		else
		{
			FuzzyCol *col = _fmb._mFuzzyColumnsByName.value(operand);
			
			return col->fType();
		}
	}

	return 0;
}

QList<QString> FuzzySQL::CallParameters(QString sArgument, int iArgType, QString marginAttribute)
{
	QList<QString> args;

	FuzzyCol *fcAssociatedColumn = _fmb._mFuzzyColumnsByName.value(marginAttribute);

	if (iArgType == 1)
	{
		args.append("3");
		args.append(sArgument);
		args.append("NULL");
		args.append("NULL");
		args.append("NULL");		

		return args;
	}

	if (iArgType == 2)
	{
		args.append(sArgument + "T");
		args.append(sArgument + "1");
		args.append(sArgument + "2");
		args.append(sArgument + "3");
		args.append(sArgument + "4");

		return args;
	}

	//constant
	if (iArgType > 10)
	{
		FuzzyConstant fConst(sArgument, _fmb);

		if (iArgType < 13)
		{
			args.append(QString(iArgType));
			args.append("NULL");
			args.append("NULL");
			args.append("NULL");
			args.append("NULL");		

			return args;
		}

		if (iArgType == 13)
		{
			args.append(QString(iArgType));
			args.append(QString::number(fConst.data[0]));
			args.append("NULL");
			args.append("NULL");
			args.append("NULL");

			return args;
		}

		if (iArgType == 14 || iArgType == 17)
		{
			//handling like a trapezoid, we already have values
			args.append("7");
			args.append(QString::number(fConst.data[0]));
			args.append(QString::number(fConst.data[1]));
			args.append(QString::number(fConst.data[2]));
			args.append(QString::number(fConst.data[3]));

			return args;
		}

		if (iArgType == 15)
		{
			args.append("5");
			args.append(QString::number(fConst.data[0]));
			args.append(QString::number(fConst.data[1]));
			args.append(QString::number(fConst.data[2]));
			args.append(QString::number(fConst.data[3]));

			return args;
		}

		if (iArgType == 16)
		{
			//need to know the column from which to take the margin
			FuzzyCol *col = _fmb._mFuzzyColumnsByName.value(marginAttribute);
			FuzzyApproxMuch *appr = _fmb._mFuzzyApproxMuch.value(col->columnId());

			double margin = appr->margin();

			args.append("5");
			args.append(QString::number(fConst.data[1] - margin));
			args.append(QString::number(fConst.data[1]));
			args.append(QString::number(fConst.data[1]));
			args.append(QString::number(fConst.data[1] + margin));

			return args;
		}		

		if (iArgType == 18)
		{
			args.append("8");
			args.append(QString::number(fConst.data[0]));
			args.append(QString::number(fConst.data[1]));
			args.append(QString::number(fConst.data[2]));
			args.append(QString::number(fConst.data[3]));

			return args;
		}		

		//11: distribution for 3 and 4-types : {L1, L2, L3, ... Ln}
		//finding all labels id's for first array, pitting 1s to second
		//13: distrubution for 2 and 4-types: {P1/L1, P2/L2, ... Pn/Ln}
		if (iArgType == 21 || iArgType == 23)
		{
			QMap<QString, FuzzyObject*> *mColumnLables = _fmb._mFuzzyLabelsByColumn.value(fcAssociatedColumn->columnId());
			
			QString argLables = "[";
			QString argValues = "[";

			for	(int i=0; i<fConst.labels.length(); i++)
			{
				argLables.append(QString::number(mColumnLables->value(fConst.labels.at(i))->fuzzyId()) + ", ");
				if (iArgType == 1)
				{
					argValues.append("1, ");
				}
				else //iArgType == 13
				{
					argValues.append(QString::number(fConst.data[i]) + ", ");
				}
			}

			argLables.remove(argLables.length() - 3, 2);
			argLables.append("]");
			argValues.remove(argLables.length() - 3, 2);
			argValues.append("]");

			//type of value - n-distribution in this case
			args.append("4");
			args.append(argLables);
			args.append(argValues);

			return args;
		}

		//types 15 and 16 are linguistic labels for fuzzy types 3 and 4
		if (iArgType == 25 || iArgType == 26)
		{
			args.append("[" + QString::number(_fmb._mFuzzyLabelsByColumn.value(fcAssociatedColumn->columnId())->value(fConst.labels.at(0))->fuzzyId()) + "]");
			args.append("[1]");
		}

		//fuzzy columns type 3,4
		if (iArgType == 3 || iArgType ==4)
		{
			QString colName = marginAttribute;
			QString argLabelIds = "[";
			QString argValues = "[";
			
			FuzzyCol *col = _fmb._mFuzzyColumnsByName.value(colName);

			for (int i = 0; i <  col->len(); i++)
			{
				argLabelIds.append(colName + "P" + QString::number(i) + ", ");
				argValues.append(colName + QString::number(i) + ", ");
			}			

			argLabelIds.remove(argLabelIds.length() - 3, 2);
			argLabelIds.append("]");
			argValues.remove(argLabelIds.length() - 3, 2);
			argValues.append("]");
			
			args.append(colName + "T");
			args.append(argLabelIds);
			args.append(argValues);

			return args;
		}
	}	
}

QString FuzzySQL::GetFullColumnName(QString operand, QString query, int iOperandStartIndex)
{
	int iOperType = OperandType(operand);

	if (iOperType < 4)
	{
		if (operand.contains('.'))
		{
			return operand;
		}
		else
		{
			//TO-DO: look for table, which contains this column in SELECT-statement
			//in case of disambiguity report an error (table must be specified!)

			FuzzyCol* col = _fmb.GetFuzzyCol(operand);
			return _fmb.GetFuzzyTableInfo(col->tableId())->name() + '.' + operand;
		}
	}

	//TO-DO: analyse expressions

	return "";		
}

//gets the part of a string inside brackets, iStart is the first character after a bracket
QString getArgumentFrom(QString expression, int iStart)
{
	int iBracketsCount = 1;
	int iPosition = iStart;

	while(iBracketsCount > 0 && iPosition < expression.length())	
	{
		if (expression.at(iPosition) == '(')
		{
			iBracketsCount++;
		}else if (expression.at(iPosition) == ')')
		{
			iBracketsCount--;
		}
	}

	return expression.mid(iStart, iPosition - iStart);
}

QList<QString> SplitComa(QString expression)
{
	QList<QString> list;

	int lastEnd = 0;
	int bracketsCount = 0;	

	for (int i = 0; i < expression.length(); i++)		
	{
		QChar cCurrent = expression.at(i);

		if (cCurrent == ')')
			bracketsCount--;
		else if (cCurrent == '(')			
			bracketsCount++;
		else if (cCurrent == ',' && bracketsCount == 0)		
		{
				list.append(expression.mid(lastEnd, i-lastEnd));
				lastEnd = i + 1;
		}		
	}

	list.append(expression.mid(lastEnd, expression.length() - lastEnd - 1));

	return list;
}

//Fuzzy SELECT ...
QString FuzzySQL::FSQL2SQL(QString queryString, QString *error)
{
    //replacing fuzzy functions:
    int *opInd;
    QString functionCall = FuzzyOperatorCallReplacement(queryString, opInd);
	int iCdegCount = 0;

	//while there are fuzzy operator calls replace them
    while (*opInd > 0)
    {
        int *leftStart, *rightEnd;

        QString left = LeftOperand(queryString, *opInd, leftStart);
        QString right = RightOperand(queryString, *opInd + 3, rightEnd);

		//defining types of operands: 1-columnType1, 2-columnType2, 3-ColumnType3, 4-ColumnType4,
		//5-expression; 10+constantType - constant
		int iLeftType = OperandType(left);
		int iRightType = OperandType(right);

		//defining participating columns (to know, for which column to search LABELS, quantifiers)
		QString sColumnLeft=GetFullColumnName(left, queryString, *leftStart), 
			sColumnRight = GetFullColumnName(right, queryString, *rightEnd - right.length());		

		//checking for compatibility
		
		//constructing calls
		QList<QString> lLeftArgs = CallParameters(left, iLeftType, sColumnRight);	
		QList<QString> lRightArgs = CallParameters(right, iRightType, sColumnLeft);

		if (lLeftArgs.count() != lRightArgs.count())
		{
			//report an error
			*error = "uncompatible arguments at fuzzy comparator at " + QString::number(*opInd);
			return "";
		}

		//FuzzyConstantType2 rightObj(right, _fmb);

		QString call;

		//if attrs of type 2
		if (lLeftArgs.count() == 5)
		{
			call = QString(functionCall + "( %1, %2, %3, %4, %5, %6, %7, %8, %9, %10)")
				.arg(lLeftArgs.at(0))
				.arg(lLeftArgs.at(1))
				.arg(lLeftArgs.at(2))
				.arg(lLeftArgs.at(3))
				.arg(lLeftArgs.at(4))						   
			   .arg(lRightArgs.at(0))
			   .arg(lRightArgs.at(1))
			   .arg(lRightArgs.at(2))
			   .arg(lRightArgs.at(3))         
			   .arg(lRightArgs.at(4));
		}
		else
		{
			call = QString(functionCall + "( %1, %2, %3, %4)")
				.arg(lLeftArgs.at(0))
				.arg(lLeftArgs.at(1))						   
			   .arg(lRightArgs.at(0))
			   .arg(lRightArgs.at(1));
		}

        QString oper, value, replacement;

        int *end1, *end2;

        QString next = NextWord(queryString, *rightEnd, end1);

        if (next == "THOLD")
        {
            next = NextWord(queryString, *end1, end2);
            bool *res;

            double threshold = next.toDouble(res);
            if (*res)
            {
                value.sprintf("%f", threshold);

                replacement = call + ">=" + value;
            }
        }
        else if(IsCrispOperator(next))
        {
            oper = next;

             next = NextWord(queryString, *end1, end2);
             bool *res;

             double threshold = next.toDouble(res);
             if (*res)
             {
                 value.sprintf("%f", threshold);

                 replacement = call + oper + value;
             }
        }
        else
        {
            bool *res;

            double value = next.toDouble(res);
            if (*res)
            {
                replacement = left + ">=" + value;
            }
            else
            {
                replacement = "";//left + ">= 0";
				//and need to add a CDEG-column for this comparator

				//ading CDEG column to SELECT statement
				int selStart = FirstSELECTPositionToLeftFrom(queryString, *leftStart);
        
				queryString = queryString.insert(selStart + 6, " " + call);
            }
        }

		//also need to check for 'CDEG(*)' which means to add all CDEG columns for fuzzy comparators
		//if no constrant was provided for fuzzy comparator, it must not be included to WHERE - statement
		//(it will ne replaced by replacement == ""
        queryString = queryString.replace(*leftStart, *rightEnd - *leftStart + 1, replacement);       

		//looking for next operator
        functionCall = FuzzyOperatorCallReplacement(queryString, opInd);
    }

	//now replacing CDEG-calls with proper t- and s- conorms
	//(min and max by default)
	while(queryString.contains("CDEG("))
	{
		int iCdegStart = queryString.indexOf("CDEG(");
>>>>>>> bafed66... Translator FSQL->SQL, fixed CRUDs, constructors for fuzzy objects, Scilab gateways for fuzzy CRUDs and queries:sci_gateway/cpp/fuzzysql.cpp

		QString arg = getArgumentFrom(queryString, iCdegStart + 5);

		TreeTokenNode cdegArgumentTree(arg);

		queryString.replace(iCdegStart, 6 + arg.length());
	}

	if(queryString.toUpper().indexOf("INSERT") > 0)
	{
		int endPosition, startPosition;
		QString w1 = NextWord(queryString, queryString.toUpper().indexOf("INSERT"), &endPosition);
		startPosition = endPosition;
		QString tableName = NextWord(queryString, startPosition + 1, &endPosition);

		QString columnNamesString = getArgumentFrom(queryString, endPosition + 2);
		
		QList<QString> columnNames = SplitComa(columnNamesString);

		QString valuesString = getArgumentFrom(queryString, queryString.toUpper().indexOf("VALUES") + 7);
		
		QList<QString> valuesList = SplitComa(valuesString);		

		for (int i = 0; i <	columnNames.count(); i++)
		{
			if (_fmb._mFuzzyColumnsByName.contains(tableName+"."+columnNames.at(i)))
			{
				FuzzyCol *fCol = _fmb._mFuzzyColumnsByName.value(tableName+"."+columnNames.at(i));
				QString colName = columnNames.at(i);

				int iType = OperandType(valuesList.at(i));
				QList<QString> args = CallParameters(valuesList.at(i), iType, tableName+"."+columnNames.at(i));

				if (fCol->fType() <= 2)
				{
					columnNames.removeAt(i);
					columnNames.insert(i, colName + "4");
					columnNames.insert(i, colName + "3");
					columnNames.insert(i, colName + "2");
					columnNames.insert(i, colName + "1");
					columnNames.insert(i, colName + "T");

					valuesList.removeAt(i);
					valuesList.insert(i, args.at(4));
					valuesList.insert(i, args.at(3));
					valuesList.insert(i, args.at(2));
					valuesList.insert(i, args.at(1));
					valuesList.insert(i, args.at(0));
				}
				else
				{
					columnNames.removeAt(i);
					for (int j = fCol->len(); j>=1; j--)
					{
						columnNames.insert(i, colName + QString::number(j));
						columnNames.insert(i, colName + "P" + QString::number(j));
					}
					columnNames.insert(i, colName + "T");

					valuesList.removeAt(i);
					for (int j = fCol->len(); j>=1; j--)
					{
						valuesList.insert(i, args.at(j));
					}
				}				
			}
		}
		
		queryString = "INSERT INTO " + tableName + "(";
		for (int i = 0; i < columnNames.count(); i++)
		{
			queryString.append(columnNames.at(i) + ", ");
		}
		queryString.remove(queryString.length() - 2, 2);
		queryString.append(") VALUES(");
		for (int i = 0; i < columnNames.count(); i++)
		{
			queryString.append(valuesList.at(i) + ", ");
		}
		queryString.remove(queryString.length() - 2, 2);
	}

    return queryString;
}



void FuzzySQL::CreateFuzzyMetaBaseOnDataBase()
{
	CreateFuzzyMetaBaseOnDataBase(_dataBase);
}

void FuzzySQL::CreateFuzzyMetaBaseOnDataBase(QSqlDatabase *db)
{
    QSqlQuery query(*db);

    query.exec("CREATE TABLE FUZZY_COL_LIST ("            
            "col       integer,"
            "F_TYPE         integer,"
            "LEN   integer,"
            "CODE_SIG        integer,"
            "COLUMN_NAME         varchar(50),"
            "COM varchar(50),"
            "UM varchar(20)"
        ");");

    query.exec("CREATE TABLE FUZZY_DEGREE_SIG ("
               "CODE_SIG integer, "
               "SIGNIFICANCE integer);"
               );

    query.exec("CREATE TABLE FUZZY_OBJECT_LIST ("
                       "OBJ_COL integer,"
                       "FUZZY_ID serial, "
                       "FUZZY_NAME varchar(40), "
                       "FUZZY_TYPE integer "
                    ");");

    query.exec("CREATE TABLE FUZZY_LABEL_DEF ("               
               "col integer, "
               "fuzzy_id integer,  "
               "alpha real, "
               "beta real, "
               "gamma real, "
               "delta real"
               ")");

    query.exec("CREATE TABLE FUZZY_APPROX_MUCH ("               
               "col integer, "
               "margin real, "
               "much real "
               ");");

    query.exec("CREATE TABLE FUZZY_NEARNESS_DEF ("               
               "col integer, "
               "fuzzy_id1 integer, "
               "fuzzy_id2 integer, "
               "degree real"
               ");");

    query.exec("CREATE TABLE FUZZY_COMPATIBLE_COL ("               
               "col1 integer, "               
               "col2 integer "
               ");");

    query.exec("CREATE TABLE FUZZY_QUALIFIERS_DEF ("               
               "col integer, "
               "fuzzy_id integer, "
               "QUALIFIER real "
               ");");

    query.exec("CRETE TABLE FUZZY_DEGREE_COLS ("               
               "col1 integer, "               
               "col2 integer "
               ");");

    query.exec("CREATE TABLE FUZZY_DEGREE_TABLE ("               
               "col integer, "
               "DEGREE_TYPE char"
               ");");

    query.exec("CREATE TABLE FUZZY_TABLE_QUANTIFIERS ("
                "obj integer, "
                "FUZZY_NAME varchar(20), "
                "FUZZY_TYPE integer, "
                "alpha real, "
                "beta real, "
                "gamma real, "
                "delta real"
               ");");

    query.exec("CREATE TABLE FUZZY_SYSTEM_QUANTIFIERS ("
                "fuzzy_name varchar(20), "
                "fuzzy_type integer, "
                "alpha real, "
                "beta real, "
                "gamma real, "
                "delta real"
               ");");

    query.exec("CREATE TABLE FUZZY_META_TABLES ("
               "table_id integer, "
               "name varchar(50) );"
               );

    query.exec("CREATE TABLE FUZZY_META_COLUMNS ("
               "col_id integer, "
               "table_id integer, "
               "name varchar(50) );"
               );

    query.exec("CREATE OR REPLACE FUNCTION feq (\
   t1 numeric, \
   a1 numeric, \
   b1 numeric, \
   c1 numeric, \
   d1 numeric, \
   t2 numeric, \
   a2 numeric, \
   b2 numeric, \
   c2 numeric, \
   d2 numeric\
)\
RETURNS real\
LANGUAGE plpgsql\
AS\
$$\
BEGIN \
IF (b1<b2 AND c1 <b2 OR b2<c1 AND c2<c1) THEN \
	RETURN 1;\
ELSE\\ \
	IF b1 < b2 THEN \
	--check slope intersection \
		IF d1 < a2 THEN \
			RETURN 0; \
		ELSE \
			RETURN (b2*d1 - c1*a2)/((b2-a2)-(c1-d1));\
		END IF;\
	ELSE \
 		IF d2 < b1 THEN\
			RETURN 0;\
		ELSE\
			RETURN (b1*d2 - c2*a1)/((b1-a1)-(c2-d2));\
		END IF;\
	END IF;\
END IF;	\
END\
$$\
		");

	query.exec("CREATE OR REPLACE FUNCTION fdif (\
   t1 numeric,\
   a1 numeric,\
   b1 numeric,\
   c1 numeric,\
   d1 numeric,\
   t2 numeric,\
   a2 numeric,\
   b2 numeric,\
   c2 numeric,\
   d2 numeric\
)\
RETURNS real\
LANGUAGE plpgsql\
AS\
$$  \
BEGIN \
\
RETURN 1 - nfeq(t1, a1, b1, c1, d1, t2, a2, b2, c2, d2); \
\
END\
$$");

    query.exec("CREATE OR REPLACE FUNCTION fdif (\
   t1 numeric, \
   a1 numeric, \
   b1 numeric, \
   c1 numeric, \
   d1 numeric, \
   t2 numeric, \
   a2 numeric, \
   b2 numeric, \
   c2 numeric, \
   d2 numeric\
)\
RETURNS real\
LANGUAGE plpgsql\
AS\
$$ \
BEGIN\
\
RETURN 1 - nfeq(t1, a1, b1, c1, d1, t2, a2, b2, c2, d2); \
\
END\
$$");

query.exec("CREATE OR REPLACE FUNCTION nfdif (\
   t1 numeric, \
   a1 numeric,\
   b1 numeric, \
   c1 numeric, \
   d1 numeric, \
   t2 numeric, \
   a2 numeric, \
   b2 numeric, \
   c2 numeric, \
   d2 numeric\
)\
RETURNS real\
LANGUAGE plpgsql\
AS\
$$  \
BEGIN \
\
RETURN 1 - feq(t1, a1, b1, c1, d1, t2, a2, b2, c2, d2); \
\
END\
$$");

query.exec("CREATE OR REPLACE FUNCTION fgt (\
   t1 numeric, \
   a1 numeric, \
   b1 numeric, \
   c1 numeric, \
   d1 numeric, \
   t2 numeric, \
   a2 numeric, \
   b2 numeric, \
   c2 numeric, \
   d2 numeric\
)\
RETURNS real\
LANGUAGE plpgsql\
AS\
$$\
BEGIN \
\
IF(c1 > d2) THEN\
	RETURN 1;\
ELSE\
	IF d1 > c2 THEN\
		RETURN (d1 - c2) / ( (d2 - c2) - (c1 - d1) );\
	ELSE \
 		RETURN 0;\
	END IF;\
END IF;	\
END\
$$");

query.exec("CREATE OR REPLACE FUNCTION nfgt (\
   t1 numeric, \
   a1 numeric, \
   b1 numeric, \
   c1 numeric, \
   d1 numeric, \
   t2 numeric, \
   a2 numeric, \
   b2 numeric, \
   c2 numeric, \
   d2 numeric\
)\
RETURNS real\
LANGUAGE plpgsql\
AS\
$$\
BEGIN \
\
IF(a1 >= d2) THEN\
	RETURN 1;\
ELSE\
	IF b1 > c2 THEN\
		RETURN (b1 - c2) / ( (d2 - c2) - (a1 - b1) );\
	ELSE \
 		RETURN 0;\
	END IF;\
END IF;	\
END\
$$");

query.exec("CREATE OR REPLACE FUNCTION fgeq (\
   t1 numeric, \
   a1 numeric, \
   b1 numeric, \
   c1 numeric, \
   d1 numeric, \
   t2 numeric, \
   a2 numeric, \
   b2 numeric, \
   c2 numeric, \
   d2 numeric\
)\
RETURNS real\
LANGUAGE plpgsql\
AS\
$$\
BEGIN \
\
IF(c1 >= b2) THEN\
	RETURN 1;\
ELSE\
	IF d1 > a2 THEN\
		RETURN (d1 - a2) / ( (b2 - a2) - (c1 - d1) );\
	ELSE \
 		RETURN 0;\
	END IF;\
END IF;	\
END\
$$");

query.exec("CREATE OR REPLACE FUNCTION nfgeq (\
   t1 numeric, \
   a1 numeric, \
   b1 numeric, \
   c1 numeric, \
   d1 numeric, \
   t2 numeric, \
   a2 numeric, \
   b2 numeric, \
   c2 numeric, \
   d2 numeric\
)\
RETURNS real\
LANGUAGE plpgsql\
AS\
$$\
BEGIN \
\
IF(a1 >= b2) THEN\
	RETURN 1;\
ELSE\
	IF b1 > a2 THEN\
		RETURN (b1 - a2) / ( (b2 - a2) - (a1 - b1) );\
	ELSE \
 		RETURN 0;\
	END IF;\
END IF;	\
END\
$$");

query.exec("CREATE OR REPLACE FUNCTION flt (\
   t1 numeric, \
   a1 numeric, \
   b1 numeric, \
   c1 numeric, \
   d1 numeric, \
   t2 numeric, \
   a2 numeric, \
   b2 numeric, \
   c2 numeric, \
   d2 numeric\
)\
RETURNS real\
LANGUAGE plpgsql\
AS\
$$\
BEGIN \
\
IF(b1 <= a2) THEN\
	RETURN 1;\
ELSE\
	IF a1 < b2 THEN\
		RETURN (a1 - b2) / ( (a2 - b2) - (b1 - a1) );\
	ELSE \
 		RETURN 0;\
	END IF;\
END IF;	\
END\
$$");

query.exec("CREATE OR REPLACE FUNCTION nflt (\
   t1 numeric, \
   a1 numeric, \
   b1 numeric, \
   c1 numeric, \
   d1 numeric, \
   t2 numeric, \
   a2 numeric, \
   b2 numeric, \
   c2 numeric, \
   d2 numeric\
)\
RETURNS real\
LANGUAGE plpgsql\
AS\
$$\
BEGIN \
\
IF(c1 <= a2) THEN\
	RETURN 1;\
ELSE\
	IF a1 < b2 THEN\
		RETURN (a1 - b2) / ( (a2 - b2) - (d1 - c1) );\
	ELSE \
 		RETURN 0;\
	END IF;\
END IF;	\
END\
$$");

query.exec("CREATE OR REPLACE FUNCTION fleq (\
   t1 numeric, \
   a1 numeric, \
   b1 numeric, \
   c1 numeric, \
   d1 numeric, \
   t2 numeric, \
   a2 numeric, \
   b2 numeric, \
   c2 numeric, \
   d2 numeric\
)\
RETURNS real\
LANGUAGE plpgsql\
AS\
$$\
BEGIN \
\
IF(b1 <= c2) THEN\
	RETURN 1;\
ELSE\
	IF a1 < d2 THEN\
		RETURN (d2 - a1) / ( (b1 - a1) - (c2 - d2) );\
	ELSE \
 		RETURN 0;\
	END IF;\
END IF;	\
END\
$$");

query.exec("CREATE OR REPLACE FUNCTION nfleq (\
   t1 numeric, \
   a1 numeric, \
   b1 numeric, \
   c1 numeric, \
   d1 numeric, \
   t2 numeric, \
   a2 numeric, \
   b2 numeric, \
   c2 numeric, \
   d2 numeric\
)\
RETURNS real\
LANGUAGE plpgsql\
AS\
$$\
BEGIN \
\
IF(b1 <= c2) THEN\
	RETURN 1;\
ELSE\
	IF c1 < d2 THEN\
		RETURN (c1 - d2) / ( (c2 - d2) - (d1 - c1) );\
	ELSE \
 		RETURN 0;\
	END IF;\
END IF;	\
END\
$$");

}
