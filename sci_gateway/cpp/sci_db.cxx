/* ==================================================================== */
/* Igor GRIDCHYN */
/* Database toolbox */
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

		sciprint("Number of items: (%d)\n", iItemNumber);

		
		//setting connection params	

		sciprint("Map contains %d elements\n", qmConnParams.count());

		if(!qmConnParams.contains (QString("provider")))
		{
			sciprint("Map does not contain provider\n");
		}

		if(! qmConnParams.contains (QString("provider")))				
			Scierror(999, "At least provider must be specified!\n");				

		sciprint("Provider: %s\n", qmConnParams.value(QString("provider")).toLatin1().data());

		QSqlDatabase db = QSqlDatabase::addDatabase(qmConnParams.value(QString("provider")));

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

		//writing the pointer to the connection object
		sciErr = createPointer(pvApiCtx, Rhs + 1, (void*)&db);	

		sciprint("Pointer: %d\n", &db);			
		sciprint("User: %s\n", db.userName().toLatin1().data());

		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		LhsVar(1) = Rhs + 1;

		return 0;
	}

	int sci_DbQuery(char * fname)
	{

		return 0;
	}
/* ==================================================================== */	
} /* extern "C" */
/* ==================================================================== */