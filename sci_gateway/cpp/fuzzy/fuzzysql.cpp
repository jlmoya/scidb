#include "fuzzysql.h"
#include "fuzzyobjecttype2.h"

QMap<QString ,QString> functions;

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

//Fuzzy SELECT ...
QString FuzzySQL::FSQL2SQL(QString queryString)
{
    //replacing fuzzy functions:
    int *opInd;
    QString functionCall = FuzzyOperatorCallReplacement(queryString, opInd);

    //int opInd = queryString.indexOf(" FEQ ", 0, Qt::CaseSensitive);

    while (*opInd > 0)
    {
        int *leftStart, *rightEnd;

        QString left = LeftOperand(queryString, *opInd, leftStart);
        QString right = RightOperand(queryString, *opInd + 3, rightEnd);

        FuzzyObjectType2 rightObj(right);

        //ONLY FOR ATTRS TYPE 2
        QString call = QString(functionCall + "( %1, %2, %3, %4, %5, %6, %7, %8, %9, %10)")
                       .arg(left+"T")
                       .arg(left+"1")
                       .arg(left+"2")
                       .arg(left+"3")
                       .arg(left+"4")
                       .arg(rightObj.type)
                       .arg(rightObj.data[0])
                       .arg(rightObj.data[1])
                       .arg(rightObj.data[2])
                       .arg(rightObj.data[3]);

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

                replacement = left + "_deg" + ">=" + value;
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

                 replacement = left + "_deg" + oper + value;
             }
        }
        else
        {
            bool *res;

            double value = next.toDouble(res);
            if (*res)
            {
                replacement = left + "_deg" + ">=" + value;
            }
            else
            {
                replacement = left + "_deg" + ">= 0";
            }
        }

        queryString = queryString.replace(*leftStart, *rightEnd - *leftStart + 1, replacement);

        //ading CDEG column to SELECT statement
        int selStart = FirstSELECTPositionToLeftFrom(queryString, *leftStart);

        //error if <0

        queryString = queryString.insert(selStart + 6, call + " as " + left + "_deg");

        functionCall = FuzzyOperatorCallReplacement(queryString, opInd);
    }

    return queryString;
}

void FuzzySQL::InsertLabelTrapecic(QString name, int a, int b, int c, int d, QSqlDatabase db)
{
    QString query = "INSERT INTO linguistic_labels(id,  \"type\", \"name\") VALUES (5, 1, '" + name + "')";

    qDebug() << query;

    QSqlQuery qry("INSERT INTO linguistic_labels(id,  \"type\", \"name\") VALUES (5, 1, '" + name + "')", db);

    qry.exec();
}

QList<QString> FuzzySQL::GetLinguisticLabels(QSqlDatabase db)
{
    QSqlQuery query(db);//("SELECT * FROM linguistic_labels", db);

    if(!query.exec("SELECT * FROM linguistic_labels;"))
    {
        qDebug() << "False executing query!\n" << query.lastError();
    }

    QList<QString> lstr;

    while (query.next()) {
        qDebug() << "Inserting";
        lstr.insert(0, query.value(0).toString());
    }

    return lstr;
}

void FuzzySQL::CreateTableMetaInfo(QSqlDatabase db, QString tableName)
{
    QSqlQuery query(db);

    query.exec("INSERT INTO FUZZY_META_TABLES (name) VALUES (" + tableName + ")");
}

void FuzzySQL::CreateColumnMetaInfo(QSqlDatabase db, QString tableName, QString columnName)
{
    QSqlQuery query(db);

    query.exec("INSERT INTO FUZZY_META_COLUMNS (table_id, name) VALUES ("
               "(SELECT table_id from FUZZY_META_TABLES where name = "+tableName + ", " + columnName+")"
               ")");
}

QString FuzzySQL::ColumnIdQuery(QString columnName)
{
	return "SELECT col_id FROM FUZZY_META_COLUMNS WHERE name = " + columnName;
}

QString FuzzySQL::ColumnIdQuery(QString table, QString columnName)
{
	return "SELECT col_id FROM FUZZY_META_COLUMNS WHERE name = " + columnName + 
		"AND table_id = " + "(SELECT table_id from FUZZY_META_TABLES WHERE name = "+table+")";
}

QString FuzzySQL::TableIdQuery(QString tableName)
{
	return "SELECT table_id FROM FUZZY_META_TABLES WHERE name = " + tableName;
}

void FuzzySQL::CreateFuzzyCol(QString column, int f_type, int len, int code_sig, 
							  QString columnName, QString com, QString um)
{
	 QSqlQuery query(dataBase);
     query.prepare("INSERT INTO FUZZY_COL_LIST"
		 "VALUES (:column, :f_type, :len, :code_sig, :columnName, :com, :um)");
     query.bindValue(":column", column);
     query.bindValue(":f_type", f_type);
     query.bindValue(":len", len);
	 query.bindValue(":code_sig", code_sig);
	 query.bindValue(":columnName", columnName);
	 query.bindValue(":com", com);
	 query.bindValue(":um", um);
     query.exec();
}

void FuzzySQL::CreateFuzzyDegreeSig(int code_sig, int significance)
{
	 QSqlQuery query(dataBase);
     query.prepare("INSERT INTO FUZZY_DEGREE_SIG"
		 "VALUES (:code_sig, :significance)");
     query.bindValue(":code_sig", code_sig);
     query.bindValue(":significance", significance);
     query.exec();
}

void FuzzySQL::CreateFuzzyObject(QString column, QString fuzzy_name, int fuzzy_type)
{
	QSqlQuery query(dataBase);
     query.prepare("INSERT INTO FUZZY_OBJECT_LIST"
		 "VALUES (:column, NULL, :fuzzy_name, :fuzzy_type)");
     query.bindValue(":column", ColumnIdQuery(column));
     query.bindValue(":fuzzy_name", fuzzy_name);
     query.bindValue(":fuzzy_type", fuzzy_type);
     query.exec();
}

void FuzzySQL::CreateFuzzyLabelDef(QString table, QString column, int fuzzy_id, 
		double alpha, double beta, double gamma, double delta)
{
	QSqlQuery query(dataBase);
     query.prepare("INSERT INTO FUZZY_LABEL_DEF"
		 "VALUES (:column, NULL, :fuzzy_id, :alpha, :beta, :gamma, :delta)");
     query.bindValue(":column", ColumnIdQuery(table, column));
     query.bindValue(":fuzzy_id", fuzzy_id);
     query.bindValue(":alpha", alpha);
	 query.bindValue(":beta", beta);
	 query.bindValue(":gamma", gamma);
	 query.bindValue(":delta", delta);
	 query.exec();
}

void FuzzySQL::CreateFuzzyApproxMuch(QString table, QString column, double margin, double much)
{
	QSqlQuery query(dataBase);
     query.prepare("INSERT INTO FUZZY_APPROX_MUCH"
		 "VALUES (:column, :margin, :much)");
     query.bindValue(":column", ColumnIdQuery(table, column));
     query.bindValue(":margin", margin);
     query.bindValue(":much", much);
     query.exec();
}

void FuzzySQL::CreateFuzzyNearness(QString table, QString column, int fuzzy_id1, int fuzzy_id2,
		double degree)
{
	QSqlQuery query(dataBase);
     query.prepare("INSERT INTO FUZZY_NEARNESS_DEF"
		 "VALUES (:column, :fuzzy_id1, :fuzzy_id2, :degree)");
     query.bindValue(":column", ColumnIdQuery(table, column));
     query.bindValue(":fuzzy_id1", fuzzy_id1);
     query.bindValue(":fuzzy_id2", fuzzy_id2);
	 query.bindValue(":degree", degree);
     query.exec();
}


void FuzzySQL::CreateFuzzyCompatibleCol(QString table1, QString col1, QString table2, QString col2)
{
	 QSqlQuery query(dataBase);
     query.prepare("INSERT INTO FUZZY_COMPATIBLE_COL"
		 "VALUES (:column1, :column2)");
     query.bindValue(":column1", ColumnIdQuery(table1, col1));
	 query.bindValue(":column2", ColumnIdQuery(table2, col2));     
     query.exec();
}

void FuzzySQL::CreateFuzzyQualifier(QString table, QString column, int fuzzy_id, double qualifier)
{
	 QSqlQuery query(dataBase);
     query.prepare("INSERT INTO FUZZY_QUALIFIERS_DEF"
		 "VALUES (:column, :fuzzy_id, :qualifier)");
     query.bindValue(":column", ColumnIdQuery(table, column));
	 query.bindValue(":fuzzy_id", fuzzy_id);     
	 query.bindValue(":qualifier", qualifier);     
     query.exec();
}

void FuzzySQL::CreateFuzzyDegreeCol(QString table1, QString col1, QString table2, QString col2)
{
		 QSqlQuery query(dataBase);
     query.prepare("INSERT INTO FUZZY_DEGREE_COLS"
		 "VALUES (:column1, :column2)");
     query.bindValue(":column1", ColumnIdQuery(table1, col1));
	 query.bindValue(":column2", ColumnIdQuery(table2, col2));     
     query.exec();
}

void FuzzySQL::CreateFuzzyDegreeTable(QString table, QString col, char degree_type)
{
		 QSqlQuery query(dataBase);
     query.prepare("INSERT INTO FUZZY_DEGREE_TABLE"
		 "VALUES (:column, :deree_type)");
     query.bindValue(":column", ColumnIdQuery(table, col));
	 query.bindValue(":degree_type", degree_type);     
     query.exec();
}

void FuzzySQL::CreateFuzzyTableQuantifier(QString table, QString fuzzy_name, int fuzzy_type, 
		double alpha, double beta, double gamma, double delta)
{
	 QSqlQuery query(dataBase);
     query.prepare("INSERT INTO FUZZY_TABLE_QUANTIFIERS"
		 "VALUES (:table, :fuzzy_name, :fuzzy_type, :alpha, :beta, :gamma, :delta)");
     query.bindValue(":table", TableIdQuery(table));
	 query.bindValue(":fuzzy_name", fuzzy_name);     
	 query.bindValue(":fuzzy_type", fuzzy_type);  
	 query.bindValue(":alpha", alpha);
	 query.bindValue(":beta", beta);
	 query.bindValue(":gamma", gamma);
	 query.bindValue(":delta", delta);
     query.exec();
}

void FuzzySQL::CreateFuzzySystemQuantifier(QString fuzzy_name, int fuzzy_type, double alpha,
		double beta, double gamma, double delta)
{
	 QSqlQuery query(dataBase);
     query.prepare("INSERT INTO FUZZY_SYSTEM_QUANTIFIERS"
		 "VALUES (:fuzzy_name, :fuzzy_type, :alpha, :beta, :gamma, :delta)");     
	 query.bindValue(":fuzzy_name", fuzzy_name);     
	 query.bindValue(":fuzzy_type", fuzzy_type);  
	 query.bindValue(":alpha", alpha);
	 query.bindValue(":beta", beta);
	 query.bindValue(":gamma", gamma);
	 query.bindValue(":delta", delta);
     query.exec();
}






void FuzzySQL::CreateFuzzyMetaBaseOnDataBase(QSqlDatabase db)
{
    QSqlQuery query(db);

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
