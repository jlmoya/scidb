/* ==================================================================== */
/* Igor GRIDCHYN */
/* Database toolbox */
/* This file is released into the public domain */
/* ==================================================================== */

#include "sci_util.h"

int SciStructStringFields(int *piAddr, QMap<QString, QString> *map, char *fname)
{
		SciErr sciErr;
	    QList<QString> namesList;
		QList<QString> valuesList;

		int m1 = 0, n1 = 0;
		//int *piAddr = NULL;
		char *pStVarOne = NULL;
		int lenStVarOne = 0;
		int iType1 = 0;

	    sciErr = getVarType(pvApiCtx, piAddr, &iType1);
		if(sciErr.iErr)
		{
		  printError(&sciErr, 0);
		  return 0;
		} 

		if ( iType1 != sci_mlist )
		{
		  Scierror(999,"%s: Wrong type for input argument #%d: A mlist expected.\n",fname,1);
		  return 0;
		}

		int iItemNumber  = 0;

		sciErr = getListItemNumber(pvApiCtx, piAddr, &iItemNumber);

		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		//sciprint("Number of items: (%d)\n", iItemNumber);

		int * piListItem = NULL;

		for(int i = 0 ; i < iItemNumber ; i++)		
		{
			//sciprint("Getting List item Address...\n");
			sciErr = getListItemAddress(pvApiCtx, piAddr, i + 1, &piListItem);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			
			int iType = 0;
			sciErr = getVarType(pvApiCtx, piListItem, &iType);
			
			if(iType != sci_strings)
				continue;

			int iRows, iCols;

			//getting string			
			//sciprint("Getting rows/ columns count...\n");
		
			sciErr = getMatrixOfStringInList(pvApiCtx, piAddr, i + 1, &iRows, &iCols, NULL, NULL);			
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

			int *piLen = (int*)malloc(sizeof(int) * iRows * iCols);
			sciErr = getMatrixOfStringInList(pvApiCtx, piAddr, i + 1, &iRows, &iCols, piLen, NULL);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

			char **pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
			for(int j = 0 ; j < iRows * iCols ; j++)
			{
				pstData[j] = (char*)malloc(sizeof(char) * (piLen[j] + 1));//+ 1 for null termination
			}			

			sciErr = getMatrixOfStringInList(pvApiCtx, piAddr, i + 1, &iRows, &iCols, piLen, pstData);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

			//sciprint("Item #%d : %s\n", i, pstData[0]);

			if(i==0)
			for(int j = 2 ; j < iRows * iCols ; j++)
			{
				//filling the list of param names
				namesList.append(pstData[j]);
				//sciprint("Item #%d:%d : %s\n", i, j, pstData[j]);
			}

			if(i>=2)
			{
				valuesList.append(pstData[0]);
			}
		}

		sciprint("Obtained values:\n");

		for (int i = 0; i < iItemNumber - 2; i++)
		{
			sciprint("%s == %s\n", namesList[i].toLatin1().data(), valuesList[i].toLatin1().data());

			map->insert(namesList[i], valuesList[i]);
		}
}

