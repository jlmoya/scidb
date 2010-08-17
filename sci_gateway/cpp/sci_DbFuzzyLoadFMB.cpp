/* ==================================================================== */
/* Igor GRIDCHYN */
/* Scidb toolbox */
/* This file is released into the public domain */
/* ==================================================================== */

/* ==================================================================== */
#include "sci_util.h"
#include "sci_db.h"
/* ==================================================================== */

extern QString sDefaultConnection;

extern QMap<QString,QList<QString>> mslsProviderConnectionOptions;

extern QList<QString> lsCommonConnectionParameters;	
	
extern QList<QString> lsProviders;

extern "C" 
{
	int sci_DbFuzzyLoadFMB(char *fname)
	{
		SciErr sciErr;    	
		QSqlDatabase *db;
		FuzzySQL *fsql;

		CheckRhs(0,1);
		CheckLhs(0,1);

		if (Rhs == 0)
		{
			db = &QSqlDatabase::database(sDefaultConnection);
		}
		else
		{
			getDatabaseParam(fname, 1, &db);	        
		}				

		fsql = new FuzzySQL(db);		

		//writing the pointer to the connection object		
		sciErr = createPointer(pvApiCtx, Rhs + 1, (void*)fsql);			

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