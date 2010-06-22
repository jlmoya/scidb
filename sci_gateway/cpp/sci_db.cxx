/* ==================================================================== */
/* Allan CORNET */
/* DIGITEO 2009 */
/* Template toolbox_skeleton */
/* This file is released into the public domain */
/* ==================================================================== */

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
/* ==================================================================== */	

	QString sDefaultConnection = "";

	QMap<QString,QList<QString>> mslsProviderConnectionOptions = QMap<QString,QList<QString>>();

	QList<QString> lsCommonConnectionParameters = QList<QString>()
		<<"host"
		<<"port"
		<<"database"
		<<"user"
		<<"provider"
		<<"password";
	
	QList<QString> lsProviders = QList<QString>()
		<<"QPSQL"
		<<"QSQLITE"
		<<"QMYSQL"
		<<"QOCI"
		<<"QIBASE"
		<<"QDB2"
		<<"QODBC"
		<<"QSQLITE2"
		<<"QTDS";

	int sci_DbInitDatabaseModule(char *fname)
	{
		mslsProviderConnectionOptions.clear();

		QList<QString> lsPsqlConnectionOptions = QList<QString>() 
			<<"connect_timeout"
			<<"options"
			<<"tty"
			<<"requiressl"
			<<"service";
		mslsProviderConnectionOptions.insert("QPSQL", lsPsqlConnectionOptions);

		QList<QString> lsSqliteConnectionOptions = QList<QString>()
			<<"QSQLITE_BUSY_TIMEOUT"
			<<"QSQLITE_OPEN_READONLY"
			<<"QSQLITE_ENABLE_SHARED_CACHE";
		mslsProviderConnectionOptions.insert("QSQLITE", lsSqliteConnectionOptions);

		QList<QString> lsOdbcConnectionOptions = QList<QString>()
			<<"SQL_ATTR_ACCESS_MODE"
			<<"SQL_ATTR_LOGIN_TIMEOUT"
			<<"SQL_ATTR_CONNECTION_TIMEOUT"
			<<"SQL_ATTR_CURRENT_CATALOG"
			<<"SQL_ATTR_METADATA_ID"
			<<"SQL_ATTR_PACKET_SIZE"
			<<"SQL_ATTR_TRACEFILE"
			<<"SQL_ATTR_TRACE"
			<<"SQL_ATTR_CONNECTION_POOLING"
			<<"SQL_ATTR_ODBC_VERSION";
		mslsProviderConnectionOptions.insert("QODBC", lsOdbcConnectionOptions);

		QList<QString> lsMySqlConnectionOptions = QList<QString>()
			<<"CLIENT_COMPRESS"
			<<"CLIENT_FOUND_ROWS"
			<<"CLIENT_IGNORE_SPACE"
			<<"CLIENT_SSL"
			<<"CLIENT_ODBC"
			<<"CLIENT_NO_SCHEMA"
			<<"CLIENT_INTERACTIVE"
			<<"UNIX_SOCKET"
			<<"MYSQL_OPT_RECONNECT";
		mslsProviderConnectionOptions.insert("QMYSQL", lsMySqlConnectionOptions);

		QList<QString> lsOciConnectionOptions = QList<QString>()
			<<"OCI_ATTR_PREFETCH_ROWS"
			<<"OCI_ATTR_PREFETCH_MEMORY";
		mslsProviderConnectionOptions.insert("QOCI", lsOciConnectionOptions);

		QList<QString> lsDb2ConnectionOptions = QList<QString>()
			<<"SQL_ATTR_ACCESS_MODE"
			<<"SQL_ATTR_LOGIN_TIMEOUT";
		mslsProviderConnectionOptions.insert("QDB2", lsDb2ConnectionOptions);

		QList<QString> lsIbaseConnectionOptions = QList<QString>()
			<<"ISC_DPB_LC_CTYPE"
			<<"ISC_DPB_SQL_ROLE_NAME";
		mslsProviderConnectionOptions.insert("QIBASE", lsIbaseConnectionOptions);

		return 0;
	}


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

		sciprint("Size: %d\n", psqQuery->size());

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

	int sci_DbFetchStruct(char *fname)
	{
		SciErr sciErr;
		QSqlQuery *psqQuery;

		sciGetQSqlQueryAt(fname, 1, &psqQuery);

		if(!psqQuery->isActive())
		{
			Scierror(999, "Given query was not successfully executed.\n");
			return 0;
		}		

		if(!psqQuery->next())
		{
			Scierror(999, "No results in query.\n");
			return 0;
		}

		QSqlRecord rec = psqQuery-> record();		

		char **pstLabels = (char**)malloc(sizeof(char*)*(rec.count() + 2));
		pstLabels[0] = "st";
		pstLabels[1] = "dims";
		double pdblDims[]  = {1,1};

		for(int j=0; j<rec.count();j++)
		{
			QString sFieldName = rec.field(j).name();

			pstLabels[j+2] = (char*)malloc(sizeof(char)*sFieldName.length());
			strcpy(pstLabels[j+2], sFieldName.toLatin1().data());
		}

		int *piStructAddress;

		createMList(pvApiCtx, Rhs + 1, rec.count() + 2, &piStructAddress);
		createMatrixOfStringInList(pvApiCtx, Rhs + 1, piStructAddress, 1, 1, rec.count() + 2, pstLabels);
		createMatrixOfDoubleInList(pvApiCtx, Rhs + 1, piStructAddress, 2, 1, 2, pdblDims);

		for(int i=0; i < rec.count(); i++)
		{
			sciWriteVarIntoList(piStructAddress, i+3, rec.value(i));
		}

		LhsVar(1) = Rhs + 1;
	}

	int sci_DbFetchAllString(char *fname)
	{
		SciErr sciErr;
		QSqlQuery *psqQuery;

		sciGetQSqlQueryAt(fname, 1, &psqQuery);

		if(!psqQuery->isActive())
		{
			Scierror(999, "Given query was not successfully executed.\n");
			return 0;
		}

		QSqlRecord rec;
		char **ppcResultStrings = NULL;

		int iRecsCount = 0;
		rec = psqQuery->record();
		int iFiledCount = rec.count();

		while(psqQuery->next())
		{
			iRecsCount++;
			ppcResultStrings = (char**)realloc(ppcResultStrings, sizeof(char*)*iRecsCount*iFiledCount);

			QSqlRecord rec = psqQuery-> record();

			for(int i=0; i < rec.count(); i++)
			{
				QString sVal = rec.value(i).toString();
				ppcResultStrings[ ( iRecsCount - 1 ) * iFiledCount + i ] = (char*)malloc(sizeof(char) * sVal.length());
				strcpy(ppcResultStrings[( iRecsCount - 1 ) * iFiledCount + i ], sVal.toLatin1().data());
			}
		}

		char **ppcTransposedResults = NULL;		
		transposeStringMatrix(ppcResultStrings, iRecsCount, iFiledCount, &ppcTransposedResults);

		//sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, iRecsCount, iFiledCount, ppcResultStrings);
		sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, iRecsCount, iFiledCount, ppcTransposedResults);

		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		free(ppcResultStrings);
		free(ppcTransposedResults);

		LhsVar(1) = Rhs + 1;

		return 0;
	}

	int sci_DbFetchString(char *fname)
	{
		SciErr sciErr;

		QSqlQuery *psqQuery;

		sciGetQSqlQueryAt(fname, 1, &psqQuery);

		if(!psqQuery->isActive())
		{
			Scierror(999, "Given query was not successfully executed.\n");
			return 0;
		}
		
		if(!psqQuery->next())
		{
			Scierror(999, "No results in query.\n");
			return 0;
		}

		QSqlRecord rec = psqQuery-> record();

		char **ppcResultStrings  = (char**)malloc(sizeof(char*) * rec.count());

		for(int i=0; i < rec.count(); i++)
		{
			QString sVal = rec.value(i).toString();
			ppcResultStrings[i] = (char*)malloc(sizeof(char) * sVal.length());
			strcpy(ppcResultStrings[i], sVal.toLatin1().data());
		}

		sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, 1, rec.count(), ppcResultStrings);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		free(ppcResultStrings);

		LhsVar(1) = Rhs + 1;

		return 0;
	}


	int sci_DbFetchAllReal(char *fname)
	{
		SciErr sciErr;
		QSqlQuery *psqQuery;

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

		while(psqQuery->next())
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

	int sci_DbFetchReal(char *fname)
	{
		SciErr sciErr;

		QSqlQuery *psqQuery;

		sciGetQSqlQueryAt(fname, 1, &psqQuery);

		if(!psqQuery->isActive())
		{
			Scierror(999, "Given query was not successfully executed.\n");
			return 0;
		}
		
		if(!psqQuery->next())
		{
			Scierror(999, "No results in query.\n");
			return 0;
		}

		QSqlRecord rec = psqQuery-> record();
	
		double *pdResults  = (double*)malloc(sizeof(double) * rec.count());	

		bool *pbConvertOk = (bool*)malloc(sizeof(bool));

		for(int i=0; i < rec.count(); i++)
		{
			pdResults[i] = rec.value(i).toDouble(pbConvertOk);
			if(!(*pbConvertOk))
			{
				Scierror(999, "Cannot convert %d-th value (%s) to double.\n", i, rec.value(i).toString().toLatin1().data());

				free(pdResults);
				free(pbConvertOk);

				return 0;
			}
		}

	

		sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, rec.count(), pdResults);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		free(pdResults);
		free(pbConvertOk);

		LhsVar(1) = Rhs + 1;

		return 0;
	}
	int sci_DbConnect(char *fname)
	{
		SciErr sciErr;
	    
		int m1 = 0, n1 = 0;
		int *piAddr = NULL;
		char *pStVarOne = NULL;
		int lenStVarOne = 0;
		int iType1 = 0;	
		
		sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
		if(sciErr.iErr)
		{
		  printError(&sciErr, 0);
		  return 0;
		}

		QMap<QString, QString> qmConnParams;
		sciStructStringFields(piAddr, &qmConnParams, fname);

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

		int iItemNumber  =0;

		sciErr = getListItemNumber(pvApiCtx, piAddr, &iItemNumber);

		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		
		//setting connection params			
		if(! qmConnParams.contains (QString("provider")))				
		{
			Scierror(999, "At least provider must be specified!\n");						
			return 0;
		}

		if(!lsProviders.contains(qmConnParams.value(QString("provider"))))
		{
			Scierror(999, "Unknown provider: %s\n", qmConnParams.value(QString("provider")));
			return 0;
		}

		int iRand;
		char *cpRandName = (char*)malloc(sizeof(char)*30);
		
		do
		{
			iRand = rand();			
			sprintf(cpRandName, "%d", iRand);						
		}
		while (QSqlDatabase::contains(QString(cpRandName)));			

		sDefaultConnection = QString(cpRandName);

		//!!! the name must be provided by a user or be a name of output variable
		QSqlDatabase db = QSqlDatabase::addDatabase(qmConnParams.value(QString("provider")), QString(cpRandName));			
		
		db.setDatabaseName(qmConnParams.value(QString("database")));		

		if(qmConnParams.contains("user"))
			db.setUserName(qmConnParams.value(QString("user")));

		if(qmConnParams.contains("host"))
			db.setHostName(qmConnParams.value(QString("localhost")));

		if(qmConnParams.contains("password"))
			db.setPassword(qmConnParams.value(QString("password")));				

		if(qmConnParams.contains("port"))
		{
			bool bPortOk;
			int port = qmConnParams.value(QString("port")).toInt(&bPortOk);

			if(bPortOk)
				db.setPort(port);
			else
			{
				Scierror(999, "Port must be an integer number.\n");
				return 0;
			}
		}

		//setting special connection options
		QMapIterator<QString, QString> mi(qmConnParams);
		QString sSpecificConnectionParams = "";
		while (mi.hasNext()) 
		{
			mi.next();

			if(lsCommonConnectionParameters.contains(mi.key()))
				continue;
			
			if(!mslsProviderConnectionOptions
				.value(qmConnParams.value(QString("provider")))
				.contains(mi.key()))
			{
				sciprint("Warning: unknown connection parameter %s ignored!\n", mi.key().toLatin1().data());
			}
			else
			{
				sSpecificConnectionParams.append(mi.key() + " = " + mi.value() + ";");
			}			
		}
		db.setConnectOptions(sSpecificConnectionParams);

		//trying to open connection
		if (!db.open())
		{
			sciprint("Cannot open connection: %s\n", db.lastError().text().toLatin1().data());
		}
		else
		{
			sciprint("Connected to database %s as %s!\n", QString("database").toLatin1().data(),
				QString("user").toLatin1().data());
		}		

		QSqlDatabase *dbc = new QSqlDatabase(db);			

		//writing the pointer to the connection object		
		sciErr = createPointer(pvApiCtx, Rhs + 1, (void*)dbc);			

		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		LhsVar(1) = Rhs + 1;		

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
			getDatabaseParam(fname, 2, &db);		

			qry = new QSqlQuery(*db);
		}

		char *pcQueryString;
        sciGetStringAt(fname, 1, &pcQueryString);
		
		QString sQry = QString(pcQueryString);				

		if(qry->exec(sQry))
		{
			sciErr = createPointer(pvApiCtx, Rhs + 1, (void*)qry);		
		}
		else
		{
			Scierror(999, "Cannot execute query: %s\n\t%s", QSqlDatabase::database("default").lastError().text().toLatin1().data(), 
				qry->lastError().text().toLatin1().data());
		}

		LhsVar(1) = Rhs + 1;

		return 0;
	}

	int sci_DbLastError(char *fname)
	{
		SciErr sciErr;
		QSqlDatabase *db;

		char *cpLastError;

		if(Rhs == 0)
		{
			db = &(QSqlDatabase::database("default"));		
		}
		else
		{
			getDatabaseParam(fname, 1, &db);
		}

		cpLastError = db->lastError().text().toLatin1().data();

		sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, 1, 1, &cpLastError);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}	

		return 0;
	}	
/* ==================================================================== */	
} /* extern "C" */
/* ==================================================================== */