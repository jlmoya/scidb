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
	int sci_DbQuery(char * fname)
	{
		QSqlDatabase *db;
		QSqlQuery *qry;
		SciErr sciErr;

		//using default connection
		if(Rhs == 1)
		{
			QSqlDatabase db = QSqlDatabase::database(sDefaultConnection);			
			
			qry = new QSqlQuery(db);
		}
		else
		{
			getDatabaseParam(fname, 1, &db);		

			qry = new QSqlQuery(*db);
		}

		char *pcQueryString;
        sciGetStringAt(fname, 2, &pcQueryString);
		
		QString sQry = QString(pcQueryString);				

		if(!db->isOpen())
		{
			Scierror(999, "The connection is not opened!");
			return 0;
		}

		if(!db->isValid())
		{
			Scierror(999, "The connection is not valid!");
			return 0;
		}

		if(qry->exec(sQry))
		{
			sciErr = createPointer(pvApiCtx, Rhs + 1, (void*)qry);		
		}
		else
		{
			Scierror(999, "Cannot execute query: %s\n\t%s", db->lastError().text().toLatin1().data(), 
				qry->lastError().text().toLatin1().data());
		}

		LhsVar(1) = Rhs + 1;

		return 0;
	}
/* ==================================================================== */	
} /* extern "C" */
/* ==================================================================== */