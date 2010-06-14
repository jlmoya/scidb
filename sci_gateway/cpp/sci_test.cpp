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

	int sci_DbTest(char *filename)
	{
		SciErr sciErr;

		int iType			= 0;
		int* piAddr			= NULL;
		void* pvPtr			= NULL;
		double* pdblData	= NULL;

		if(GetType(1) == sci_pointer)
		{
			sciprint("The type of the first argument is sci_pointer.\n");
		}
		else
		{
            sciprint("The type of the first argument is %d.\n", GetType(1));
		}

		sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
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

		//sciprint("Converting to QSqlDatabase*...\n");

		QSqlDatabase *db = (QSqlDatabase*)pvPtr;			

		sciprint("Converted!\n");

		sciprint("Pointer: %d\n", db);	
		sciprint("User: %d\n", db->userName().toLatin1().data());	
		
	}
}