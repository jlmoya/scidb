#include "sci_util.h"

int sciGetQueryParam(char *fname, int iPos, QSqlQuery **query)
{
	int *piAddr;
	void *pvPtr;
	SciErr sciErr;

	sciErr = getVarAddressFromPosition(pvApiCtx, iPos, &piAddr);
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

	*query = (QSqlQuery*)pvPtr;		

	return 0;
}

int getDatabaseParam(char *fname, int iPos, QSqlDatabase **db)
{
	int *piAddr;
	void *pvPtr;
	SciErr sciErr;

	sciErr = getVarAddressFromPosition(pvApiCtx, iPos, &piAddr);
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

	*db = (QSqlDatabase*)pvPtr;

	return 0;
}
int sciWriteVarIntoList(int *piList, int position, QVariant vValue)
{
	switch(vValue.type())
	{
		case QVariant::Bool:
		{
			bool bField = vValue.toBool();
			int iBool;
			if(bField)
				iBool = 1;
			else
				iBool = 0;
			
			createMatrixOfBooleanInList(pvApiCtx, Rhs+1, piList, position, 1, 1, &iBool);
			break;
		}

		case QVariant::Int:
		{
			int iField = vValue.toInt();

			int *res = (int*)malloc(sizeof(int));
			*res = iField;

			createMatrixOfInteger32InList(pvApiCtx, Rhs + 1, piList, position, 1, 1, res);
			break;
		}

		case QVariant::UInt:
		{
			unsigned int iField = vValue.toUInt();

			unsigned int *res = (unsigned int*)malloc(sizeof(unsigned int));
			*res = iField;

			createMatrixOfUnsignedInteger32InList(pvApiCtx, Rhs + 1, piList, position, 1, 1, res);
			break;
		}

    	case QVariant::LongLong:
		{
			int iField = vValue.toInt();

			int *res = (int*)malloc(sizeof(int));
			*res = iField;

			createMatrixOfInteger32InList(pvApiCtx, Rhs + 1, piList, position, 1, 1, res);	
			break;
		}

		case QVariant::ULongLong:
		{
			unsigned int iField = vValue.toUInt();

			unsigned int *res = (unsigned int*)malloc(sizeof(unsigned int));
			*res = iField;

			createMatrixOfUnsignedInteger32InList(pvApiCtx, Rhs + 1, piList, position, 1, 1, res);

			break;
		}

		case QVariant::Double:
		{
			double dField = vValue.toDouble();

			double *res = (double*)malloc(sizeof(double));
			*res = dField;
			
			sciprint("Value: %f\n", *res);

			createMatrixOfDoubleInList(pvApiCtx, Rhs+1, piList, position, 1, 1, res);
			break;
		}

		case QVariant::String:
		{
			char *pcField = vValue.toString().toLatin1().data();

			createMatrixOfStringInList(pvApiCtx, Rhs+1, piList, position, 1, 1, &pcField);
			break;
		}

		default:
		{
			sciprint("Unknown var type with code: %d. Writing as string.\n", vValue.type());

			char *pcField = vValue.toString().toLatin1().data();

			createMatrixOfStringInList(pvApiCtx, Rhs+1, piList, position, 1, 1, &pcField);

			break;
		}
	}

	return 0;
}

int sciStructStringFields(int *piAddr, QMap<QString, QString> *map, char *fname)
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

		return 0;
}



int sciGetStringAt(char *fname, int iPos, char **ppcResult)
{
		SciErr sciErr;
		int i;
		int iRows       = 0;
		int iCols       = 0;
		int* piLen      = NULL;
		char **pstData = NULL;
		int *piPos;

		sciErr = getVarAddressFromPosition(pvApiCtx, iPos, &piPos);

		sciErr = getMatrixOfString(pvApiCtx, piPos, &iRows, &iCols, NULL, NULL);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		piLen = (int*)malloc(sizeof(int) * iRows * iCols);
		sciErr = getMatrixOfString(pvApiCtx, piPos, &iRows, &iCols, piLen, NULL);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
		}

		sciErr = getMatrixOfString(pvApiCtx, piPos, &iRows, &iCols, piLen, pstData);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		*ppcResult = pstData[0];

		return 0;
}
int sciGetQSqlQueryAt(char *fname, int iPos, QSqlQuery **ppSqlQuery)
{
	SciErr sciErr;
	    
		int m1 = 0, n1 = 0;
		int *piAddr = NULL;
		char *pStVarOne = NULL;
		int lenStVarOne = 0;
		int iType1 = 0;	
		void *pvPtr;

		sciErr = getVarAddressFromPosition(pvApiCtx, iPos, &piAddr);
		if(sciErr.iErr)
		{
		  printError(&sciErr, 0);
		  return 0;
		}

		sciErr = getVarType(pvApiCtx, piAddr, &iType1);
		if(sciErr.iErr)
		{
		  printError(&sciErr, 0);
		  return 0;
		} 

		if ( iType1 != sci_pointer )
		{
		  Scierror(999,"%s: Wrong type for input argument #%d: A pointer expected.\n", fname, 1);
		  return 0;
		}

		sciErr = getPointer(pvApiCtx, piAddr, &pvPtr);

		*ppSqlQuery = (QSqlQuery*)pvPtr;

		return 0;
}

int transposeDoubleMatrix(double *pdMatr, int iRows, int iCols, double **ppdRes)
{
	*ppdRes = (double*)malloc( sizeof(double) * iRows * iCols);

	for(int i=0; i < iRows; i++)
	{
		for(int j=0; j < iCols; j++)
		{
			(*ppdRes)[i*iCols + j] = pdMatr[j*iRows + i];
		}
	}

	return 0;
}

int transposeStringMatrix(char **pdMatr, int iRows, int iCols, char ***ppdRes)
{
	*ppdRes = (char**)malloc( sizeof(char*) * iRows * iCols);

	for(int i=0; i < iRows; i++)
	{
		for(int j=0; j < iCols; j++)
		{
			(*ppdRes)[i*iCols + j] = (char*)malloc(strlen(pdMatr[j*iRows + i]));
			strcpy((*ppdRes)[i*iCols + j], pdMatr[j*iRows + i]);
		}
	}

	return 0;
}