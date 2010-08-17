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
	int sci_DbFuzzyLastError(char *fname)
	{
		SciErr sciErr;    	
		QSqlDatabase *db;
		FuzzySQL *fsql;

		CheckRhs(1,1);
		CheckLhs(0,1);

		getFSQLParam(fname, 1, &fsql);		

		char *lastError = (char*)malloc(sizeof(char)*strlen(fsql->LastError().toLatin1().data()));		
		strcpy(lastError, fsql->LastError().toLatin1().data());

		sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, 1, 1, &lastError);

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