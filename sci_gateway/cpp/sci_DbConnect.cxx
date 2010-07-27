/* ==================================================================== */
/* Igor GRIDCHYN */
/* Scidb toolbox */
/* This file is released into the public domain */
/* ==================================================================== */

/* ==================================================================== */
#include "sci_util.h"
#include "sci_db.h"
#include <stdio.h>
//#include <QtSql\drivers\psql\qsql_psql.cpp>
//#include <QtSql\drivers\mysql\qsql_mysql.cpp>
//#include <QtSql\drivers\sqlite\qsql_sqlite.cpp>
//#include <QtSql\drivers\ibase\qsql_ibase.cpp>
//#include <QtSql\drivers\odbc\qsql_odbc.cpp>
/* ==================================================================== */

extern QString sDefaultConnection;

extern QMap<QString,QList<QString> > mslsProviderConnectionOptions;

extern QList<QString> lsCommonConnectionParameters;	
	
extern QList<QString> lsProviders;

extern "C" 
{
	int sci_DbConnect(char *fname)
	{
		SciErr sciErr;

		int m1 = 0, n1 = 0;
		int *piAddr = NULL;
		char *pStVarOne = NULL;
		int lenStVarOne = 0;
		int iType1 = 0;	
		QSqlDatabase db;
		int iRand;
		char *cpRandName = (char*)malloc(sizeof(char)*30);

		sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
		if(sciErr.iErr)
		{
		  printError(&sciErr, 0);
		  return 0;
		}

		sciErr = getVarType(pvApiCtx, piAddr, &iType1);
		if(sciErr.iErr)
		{
		  printError(&sciErr, 0);
		  return 0;
		} 

		if ( iType1 != sci_mlist && iType1 != sci_strings)
		{
		  Scierror(999,"%s: Wrong type for forst input argument #%d: A mlist or string expected.\n",fname,1);
		  return 0;
		}

		if(iType1 == sci_mlist)

		{
			QMap<QString, QString> qmConnParams;
			sciStructStringFields(piAddr, &qmConnParams, fname);
	
			int iItemNumber  =0;

			sciErr = getListItemNumber(pvApiCtx, piAddr, &iItemNumber);

			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			
			//setting connection params			
			if(! qmConnParams.contains (QString("provider")))				
			{
				Scierror(999, "%s: Wrong value for input argument #1: At least a provider must be specified.\n", fname);						
				return 0;
			}

			if(!lsProviders.contains(qmConnParams.value(QString("provider"))))
			{
				Scierror(999, "Unknown provider: %s\n", qmConnParams.value(QString("provider")));
				return 0;
			}
			
			do
			{
				iRand = rand();			
				sprintf(cpRandName, "%d", iRand);						
			}
			while (QSqlDatabase::contains(QString(cpRandName)));			

			sDefaultConnection = QString(cpRandName);

			//!!! the name must be provided by a user or be a name of output variable
			db = QSqlDatabase::addDatabase(qmConnParams.value(QString("provider")), QString(cpRandName));			
			
			db.setDatabaseName(qmConnParams.value(QString("database")));		

			if(qmConnParams.contains("user"))
				db.setUserName(qmConnParams.value(QString("user")));

			if(qmConnParams.contains("host"))
				db.setHostName(qmConnParams.value(QString("localhost")));

			if(qmConnParams.contains("password"))
				db.setPassword(qmConnParams.value(QString("password")));				

			if(qmConnParams.contains("port"))
			{
				bool bPortOk;
				int port = qmConnParams.value(QString("port")).toInt(&bPortOk);

				if(bPortOk)
					db.setPort(port);
				else
				{
					Scierror(999, "Port must be an integer number.\n");
					return 0;
				}
			}

			//setting special connection options
			QMapIterator<QString, QString> mi(qmConnParams);
			QString sSpecificConnectionParams = "";
			while (mi.hasNext()) 
			{
				mi.next();

				if(lsCommonConnectionParameters.contains(mi.key()))
					continue;
				
				if(!mslsProviderConnectionOptions
					.value(qmConnParams.value(QString("provider")))
					.contains(mi.key()))
				{
					//sciprint("Warning: unknown connection parameter %s ignored!\n", mi.key().toLatin1().data());
					Scierror(999, "%s: Wrong value for input argument #1: \"%s\" is not a valid parameter.", fname, mi.key().toLatin1().data());
				}
				else
				{
					sSpecificConnectionParams.append(mi.key() + " = " + mi.value() + ";");
				}			
			}
			db.setConnectOptions(sSpecificConnectionParams);

			//trying to open connection
		}
		else
		{
			//params: provider and connection string
			char *provider, *connStr;

			sciGetStringAt(fname, 1, &provider);

			if(!lsProviders.contains(provider))
			{
				Scierror(999, "Unknown provider: %s\n", provider);
				return 0;
			}

			sciGetStringAt(fname, 2, &connStr);

			QSqlDriver *drv;

			//if(!strcmp(provider, "QPSQL"))
			//{
			//	sciprint("Connecting to PostgreSQL with connection string %s ...\n", connStr);
			//	PGconn *con = PQconnectdb(connStr);
			//	drv =  new QPSQLDriver(con);							
			//}	

			//if (!strcmp(provider, "QMYSQL"))
			//{
			//	MYSQL *conn;
			//	mysql_init(conn);
			//	mysql_real_connect();				

			//	drv = new QMYSQLDriver(conn);
			//}

			//if (!strcmp(provider, "QSQLITE"))
			//{
			//	sqlite3 *conn;
			//	sqlite3_open(connStr, &conn);
			//	drv = new QSQLiteDriver(conn);
			//}

			//if (!strcmp(provider, "QIBASE"))
			//{
			//	isc_db_handle conn;
			//	ISC_STATUS status_vector[20];

			//	isc_detach_database(status_vector, &conn);

			//	drv = new QIBaseDriver(conn);
			//}

			do
			{
				iRand = rand();	
				sprintf(cpRandName, "%d", iRand);
			}
			while (QSqlDatabase::contains(QString(cpRandName)));
			sDefaultConnection = QString(cpRandName);

			db = QSqlDatabase::addDatabase(drv, QString(cpRandName));
		}

		db.open();

		QSqlDatabase *dbc = new QSqlDatabase(db);

		//writing the pointer to the connection object
		sciErr = createPointer(pvApiCtx, Rhs + 1, (void*)dbc);

		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		LhsVar(1) = Rhs + 1;

		return 0;
	}
/* ==================================================================== */	
} /* extern "C" */
/* ==================================================================== */