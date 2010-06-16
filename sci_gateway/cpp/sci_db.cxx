/* ==================================================================== */
/* Allan CORNET */
/* DIGITEO 2009 */
/* Template toolbox_skeleton */
/* This file is released into the public domain */
/* ==================================================================== */

#include <string>
#include <QtCore\QString.h>
#include <QtSql\qsql.h>
#include <QtSql\qsqldatabase.h>
#include <QtSql\qsqldriver.h>
#include <QtSql\qsqlquery.h>
#include <QtSql\qsqlerror.h>
#include <QtSql\qsqlfield.h>
#include <QtSql\qsqlindex.h>

/* ==================================================================== */
#include "sci_util.h"
/* ==================================================================== */
extern "C" 
{
/* ==================================================================== */	
  #include "stack-c.h"
  #include "api_scilab.h"
  #include "Scierror.h"
  #include "MALLOC.h"
  #include <cstdlib>
  #include "sciprint.h"
/* ==================================================================== */	

	int sci_DbConnect(char *fname)
	{
		SciErr sciErr;
	    
		int m1 = 0, n1 = 0;
		int *piAddr = NULL;
		char *pStVarOne = NULL;
		int lenStVarOne = 0;
		int iType1 = 0;	
		
		sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
		if(sciErr.iErr)
		{
		  printError(&sciErr, 0);
		  return 0;
		}

		QMap<QString, QString> qmConnParams;
		SciStructStringFields(piAddr, &qmConnParams, fname);

		sciErr = getVarType(pvApiCtx, piAddr, &iType1);
		if(sciErr.iErr)
		{
		  printError(&sciErr, 0);
		  return 0;
		} 

		if ( iType1 != sci_mlist )
		{
		  Scierror(999,"%s: Wrong type for input argument #%d: A mlist expected.\n",fname,1);
		  return 0;
		}

		int iItemNumber  =0;

		sciErr = getListItemNumber(pvApiCtx, piAddr, &iItemNumber);

		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		
		//setting connection params			
		if(! qmConnParams.contains (QString("provider")))				
			Scierror(999, "At least provider must be specified!\n");						

		QSqlDatabase db = QSqlDatabase::addDatabase(qmConnParams.value(QString("provider")), "default");		

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
			}
		}

		if (!db.open())
		{
			sciprint("Cannot open connection: %s\n", db.lastError().text().toLatin1().data());
		}
		else
		{
			sciprint("Connected to database %s as %s!\n", QString("database").toLatin1().data(),
				QString("user").toLatin1().data());
		}		

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


	int getDatabaseParam(char *fname, int iPos, QSqlDatabase **db)
	{
		int *piAddr;
		void *pvPtr;
		SciErr sciErr;

		sciErr = getVarAddressFromPosition(pvApiCtx, iPos, &piAddr);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = getPointer(pvApiCtx, piAddr, &pvPtr);
		
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}		

		*db = (QSqlDatabase*)pvPtr;		

		return 0;
	}

	int sci_DbDisconnect(char *fname)
	{
		QSqlDatabase *db;

		if(Rhs==1)
		{
			getDatabaseParam(fname, 1, &db);
		}
		else
		{
			db = &QSqlDatabase::database("default");
		}		

		db->close();
		
		return 0;
	}

	int sci_DbQuery(char * fname)
	{
		QSqlDatabase *db;
		QSqlQuery *qry;
		SciErr sciErr;

		//using default connection
		if(Rhs == 1)
		{
			QSqlDatabase db = QSqlDatabase::database("default");			
			
			qry = new QSqlQuery(db);
		}
		else
		{
			getDatabaseParam(fname, 2, &db);		

			qry = new QSqlQuery(*db);
		}

		char *pcQueryString;
        sciGetStringAt(fname, 1, &pcQueryString);
		
		QString sQry = QString(pcQueryString);				

		if(qry->exec(sQry))
		{
			sciErr = createPointer(pvApiCtx, Rhs + 1, (void*)qry);		
		}
		else
		{
			Scierror(999, "Cannot execute query: %s\n\t%s", QSqlDatabase::database("default").lastError().text().toLatin1().data(), 
				qry->lastError().text().toLatin1().data());
		}

		LhsVar(1) = Rhs + 1;

		return 0;
	}

	int sci_DbLastError(char *fname)
	{
		SciErr sciErr;
		QSqlDatabase *db;

		char *cpLastError;

		if(Rhs == 0)
		{
			db = &(QSqlDatabase::database("default"));		
		}
		else
		{
			getDatabaseParam(fname, 1, &db);
		}

		cpLastError = db->lastError().text().toLatin1().data();

		sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, 1, 1, &cpLastError);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}	

		return 0;
	}	
/* ==================================================================== */	
} /* extern "C" */
/* ==================================================================== */