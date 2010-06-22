/* ==================================================================== */
/* Igor GRICHYN */
/* Scidb toolbox */
/* This file is released into the public domain */
/* ==================================================================== */

/* ==================================================================== */
#include "sci_util.h"
#include "sci_db.h"
/* ==================================================================== */
extern "C"
{
	int sci_DbFetchAllStruct(char *fname)
	{
		SciErr sciErr;
		QSqlQuery *psqQuery;

		sciGetQSqlQueryAt(fname, 1, &psqQuery);

		if(!psqQuery->isActive())
		{
			Scierror(999, "Given query was not successfully executed.\n");
			return 0;
		}		

		char **pstLabels;
		double pdblDims[]  = {1,1};
		
		QSqlRecord rec;
		int iFieldsNumber;

		if(psqQuery->next())
		{
			rec = psqQuery-> record();

			iFieldsNumber = rec.count();

			pstLabels = (char**)malloc(sizeof(char*)*(iFieldsNumber + 2));
			pstLabels[0] = "st";
			pstLabels[1] = "dims";			

			for(int j=0; j<iFieldsNumber; j++)
			{
				QString sFieldName = rec.field(j).name();

				pstLabels[j+2] = (char*)malloc(sizeof(char)*sFieldName.length());
				strcpy(pstLabels[j+2], sFieldName.toLatin1().data());
			}
		}
		else
		{
			Scierror(999, "No results in query.\n");
			return 0;
		}

		QList<QList<QVariant>> llvRecords = QList<QList<QVariant>>();				

		do
		{
			rec = psqQuery-> record();	

			QList<QVariant> lvRecord = QList<QVariant>();
			
			for(int i=0; i<rec.count(); i++)
			{
				lvRecord.append(rec.value(i));
			}

			llvRecords.append(lvRecord);
		}
		while(psqQuery->next());

		int *piList, *piStruct;

		createList(pvApiCtx, Rhs + 1, llvRecords.size(), &piList);

		for (int i = 0; i < llvRecords.size(); ++i) 
		{
			SciErr err = createMListInList(pvApiCtx, Rhs + 1, piList, i + 1, iFieldsNumber + 2, &piStruct);
			createMatrixOfStringInList(pvApiCtx, Rhs + 1, piStruct, 1, 1, iFieldsNumber + 2, pstLabels);
			createMatrixOfDoubleInList(pvApiCtx, Rhs + 1, piStruct, 2, 1, 2, pdblDims);

			for (int j = 0; j < iFieldsNumber; j++) 
			{
				sciWriteVarIntoList(piStruct, j+3, llvRecords.at(i).at(j));
			}
		}

		LhsVar(1) = Rhs + 1;
	}
/* ==================================================================== */	
} /* extern "C" */
/* ==================================================================== */