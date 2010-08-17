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
	int sci_DbDisconnect(char *fname)
	{
		QSqlDatabase *db;

		CheckRhs(0,1);
		CheckLhs(0,1);

		if(Rhs==1)
		{
			getDatabaseParam(fname, 1, &db);
		}
		else
		{
			db = &QSqlDatabase::database(sDefaultConnection);
		}		

		db->close();
		
		return 0;
	}
/* ==================================================================== */	
} /* extern "C" */
/* ==================================================================== */