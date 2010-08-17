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
	int sci_DbFuzzyHasFMB(char *fname)
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

		QStringList slTables = db->tables();

		bool bResult = true;

		if (!slTables.contains("fuzzy_approx_much") ||
			!slTables.contains("fuzzy_col_list") ||
			!slTables.contains("fuzzy_compatible_col") ||
			!slTables.contains("fuzzy_degree_cols") ||
			!slTables.contains("fuzzy_degree_sig") ||
			!slTables.contains("fuzzy_degree_table") ||
			!slTables.contains("fuzzy_label_def") ||
			!slTables.contains("fuzzy_meta_tables") ||
			!slTables.contains("fuzzy_nearness_def") ||
			!slTables.contains("fuzzy_object_list") ||
			!slTables.contains("fuzzy_qualifiers_def") ||
			!slTables.contains("fuzzy_system_quantifiers") ||
			!slTables.contains("fuzzy_table_quantifiers") 
			)
		{
			bResult = false;
		}
		
		int iResult = (int)bResult;

		sciErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, 1, 1, &iResult);

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