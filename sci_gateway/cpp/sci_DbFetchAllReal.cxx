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
	int sci_DbFetchAllReal(char *fname)
	{
		SciErr sciErr;
		QSqlQuery *psqQuery;

		CheckRhs(1,1);
		CheckLhs(0,1);

		sciGetQSqlQueryAt(fname, 1, &psqQuery);

		if(!psqQuery->isActive())
		{
			Scierror(999, "Given query was not successfully executed.\n");
			return 0;
		}

		QSqlRecord rec;
		double *pdResults = NULL;
		bool *pbConvertOk = (bool*)malloc(sizeof(bool));

		int iRecsCount = 0;
		rec = psqQuery->record();
		int iFiledCount = rec.count();

		if(!psqQuery->isValid())
		{
			psqQuery->next();
		}

		do
		{
			iRecsCount++;
			pdResults = (double*)realloc(pdResults, sizeof(double) * iRecsCount * iFiledCount);

			QSqlRecord rec = psqQuery-> record();

			for(int i=0; i < rec.count(); i++)
			{
				pdResults[( iRecsCount - 1 ) * iFiledCount + i ] = rec.value(i).toDouble(pbConvertOk);

				if(!(*pbConvertOk))
				{
					Scierror(999, "Cannot convert %d-th value (%s) to double.\n", i, rec.value(i).toString().toLatin1().data());
	
					free(pdResults);
					free(pbConvertOk);
	
					return 0;
				}
			}
		}
		while(psqQuery->next());

		double *pdTranspResults = NULL;
		transposeDoubleMatrix(pdResults, iFiledCount, iRecsCount, &pdTranspResults);

		//sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, iFiledCount, iRecsCount, pdResults);
		sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, iRecsCount, iFiledCount, pdTranspResults);

		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		
		free(pdResults);
		free(pbConvertOk);
		free(pdTranspResults);

		LhsVar(1) = Rhs + 1;

		return 0;
	}	
/* ==================================================================== */	
} /* extern "C" */
/* ==================================================================== */