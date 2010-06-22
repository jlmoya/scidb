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