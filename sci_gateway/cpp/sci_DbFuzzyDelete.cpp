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
	int sci_DbFuzzyDelete(char *fname)
	{
		SciErr sciErr;    	
		QSqlDatabase *db;
		FuzzySQL *fsql;
		char *pcObjectType;
		int *piObjectId;

		//arguments: FSQL-pointer, object type, {object id | object id1, object id2 (string or int) }
		// ( 3 variants total )

		CheckRhs(3,4);
		CheckLhs(0,1);

		getFSQLParam(fname, 1, &fsql);		
		sciGetStringAt(fname, 2, &pcObjectType);	
		
		
		if (Rhs == 3)
		{
			if (!strcmp(pcObjectType, "FSQ"))
			{
				char **psObjectName;
				sciGetStringAt(fname, 3, psObjectName);

				fsql->FuzzyMetaBase()->DeleteFuzzySystemQuantifier(*psObjectName);
			}
			else
			{
				sciGetIntAt(fname, 3, piObjectId);

				if (!strcmp(pcObjectType, "FMT"))		
				{
					fsql->FuzzyMetaBase()->DeleteFuzzyTableInfo(*piObjectId);
				} else if (!strcmp(pcObjectType, "FCL"))
				{
					fsql->FuzzyMetaBase()->DeleteFuzzyColumn(*piObjectId);
				} else if (!strcmp(pcObjectType, "FOB"))
				{
					fsql->FuzzyMetaBase()->DeleteFuzzyObject(*piObjectId);
				}else if (!strcmp(pcObjectType, "FLD"))
				{
					fsql->FuzzyMetaBase()->DeleteFuzzyLabel(*piObjectId);
				}else if (!strcmp(pcObjectType, "FAM"))
				{
					fsql->FuzzyMetaBase()->DeleteFuzzyApproxMuch(*piObjectId);
				}else if (!strcmp(pcObjectType, "FQD"))
				{
					fsql->FuzzyMetaBase()->DeleteFuzzyQualifier(*piObjectId);
				}else if (!strcmp(pcObjectType, "FDC"))
				{
					fsql->FuzzyMetaBase()->DeleteFuzzyDegreeCol(*piObjectId);
				}else if (!strcmp(pcObjectType, "FDT"))
				{
					fsql->FuzzyMetaBase()->DeleteFuzzyDegreeTable(*piObjectId);				}
				else if (!strcmp(pcObjectType, "FDS"))
				{
					fsql->FuzzyMetaBase()->DeleteFuzzyDegreeSig(*piObjectId);
				}				
				else if (strcmp(pcObjectType, "FTQ") && strcmp(pcObjectType, "FND") && strcmp(pcObjectType, "FCC"))
				{
					Scierror(999, "%s: Unknow object type %s", fname, pcObjectType);
					return 0;
				}
				else
				{
					Scierror(999, "%s: Wrong argument number for object type %s. 4 arguments expectd", fname, pcObjectType);
					return 0;
				}
			}
		}
		else if (Rhs == 4)
		{			
			if (!strcmp(pcObjectType, "FTQ"))
			{
				char **psStringId;
				sciGetStringAt(fname, 4, psStringId);

				fsql->FuzzyMetaBase()->DeleteFuzzyTableQuantifier(*piObjectId, *psStringId);
			} else if (!strcmp(pcObjectType, "FND"))
			{
				int iSecondId;

				sciGetIntAt(fname, 4, &iSecondId);

				fsql->FuzzyMetaBase()->DeleteFuzzyNearness(*piObjectId, iSecondId);
			}else if (!strcmp(pcObjectType, "FCC"))
			{
				int iSecondId;
				sciGetIntAt(fname, 4, &iSecondId);
				
				fsql->FuzzyMetaBase()->DeleteFuzzyCompatibleCol(*piObjectId, iSecondId);
			}	
			else if (strcmp(pcObjectType, "FMT") &&
				strcmp(pcObjectType, "FCL") && 
				strcmp(pcObjectType, "FOB") &&
				strcmp(pcObjectType, "FLD") &&
				strcmp(pcObjectType, "FAM") &&				
				strcmp(pcObjectType, "FQD") &&
				strcmp(pcObjectType, "FDC") &&
				strcmp(pcObjectType, "FDT") &&
				strcmp(pcObjectType, "FDS"))
			{
				Scierror(999, "%s: Unknow object type %s", fname, pcObjectType);
				return 0;
			}
			else
			{
				Scierror(999, "%s: Wrong argument number for object type %s. 3 arguments expectd", fname, pcObjectType);
				return 0;
			}
		}	

		Lhs = Rhs + 1;

		return 0;
	}
/* ==================================================================== */	
} /* extern "C" */
/* ==================================================================== */