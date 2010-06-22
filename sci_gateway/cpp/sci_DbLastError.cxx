/* ==================================================================== */
/* Igor GRIDCHYN */
/* Scidb toolbox */
/* This file is released into the public domain */
/* ==================================================================== */

/* ==================================================================== */
#include "sci_util.h"
#include "sci_db.h"
/* ==================================================================== */
extern "C"
{
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