#include <string>
#include <QtCore/qstring.h>
#include <QtSql/qsql.h>
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqldriver.h>
#include <QtSql/qsqlquery.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlfield.h>
#include <QtSql/qsqlindex.h>


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
		QSqlDatabase *db	= NULL;

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

		//TEST: pointer at db
		db = (QSqlDatabase*)pvPtr;
		sciprint("User: %s\n", db->userName().toLatin1().data());

		//TEST:connect db user check		
		//db = &QSqlDatabase::database("default");				

		//sciprint("Pointer : 0x%08X\n", pvPtr);	
		//
		//sciprint("User: %s\n", db->userName().toLatin1().data());							

		//TEST: query
		//QSqlQuery *pQry= (QSqlQuery*)pvPtr;

		//sciprint("Number of results: %d", pQry->size());
	}
}