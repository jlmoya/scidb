/*
 * Copyright (C) 2010 - Igor GRIDCHYN
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "sci_util.h"
#include "sci_db.h"
#include <stdio.h>
//#include <QtSql/sqldrivers/psql/qsql_psql.cpp>
//#include <QtSql/sqldrivers/mysql/qsql_mysql.cpp>
//#include <QtSql\sqldrivers\sqlite\qsql_sqlite.cpp>
//#include <QtSql/sqldrivers/ibase/qsql_ibase.cpp>
//#include <QtSql\sqldrivers\odbc\qsql_odbc.cpp>
//#include <QtSql\sqldrivers\oci\qsql_oci.cpp>
//#include <QtSql\sqldrivers\db2\qsql_db2.cpp>

extern QString sDefaultConnection;

extern QMap<QString,QList<QString> > mslsProviderConnectionOptions;

extern QList<QString> lsCommonConnectionParameters;

extern QList<QString> lsProviders;

extern "C"
{
    int sci_DbConnect(char *fname)
    {
        SciErr sciErr;

        int m1 = 0, n1 = 0;
        int *piAddr = NULL;
        char *pStVarOne = NULL;
        int lenStVarOne = 0;
        int iType1 = 0;
        QSqlDatabase db;
        int iRand;
        char *cpRandName = (char*)malloc(sizeof(char)*30);

        CheckRhs(1,2);
        CheckLhs(0,1);

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
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

        if ( iType1 != sci_mlist && iType1 != sci_strings)
        {
          Scierror(999,"%s: Wrong type for forst input argument #%d: A mlist or string expected.\n",fname,1);
          return 0;
        }

        if(iType1 == sci_mlist)

        {
            QMap<QString, QString> qmConnParams;
            sciStructStringFields(piAddr, &qmConnParams, fname);

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
                Scierror(999, "%s: Wrong value for input argument #1: At least a provider must be specified.\n", fname);
                return 0;
            }

            QString sProvider = qmConnParams.value("provider");
            qmConnParams.remove("provider");
            qmConnParams.insert("provider", "Q"+sProvider);

            if(!lsProviders.contains(qmConnParams.value(QString("provider"))))
            {
                Scierror(999, "Unknown provider: %s\n", qmConnParams.value(QString("provider")).toLatin1().data());
                return 0;
            }

            do
            {
                iRand = rand();
                sprintf(cpRandName, "%d", iRand);
            }
            while (QSqlDatabase::contains(QString(cpRandName)));

            sDefaultConnection = QString(cpRandName);

            //!!! the name must be provided by a user or be a name of output variable
            db = QSqlDatabase::addDatabase(qmConnParams.value(QString("provider")), QString(cpRandName));

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
                    //sciprint("Warning: unknown connection parameter %s ignored!\n", mi.key().toLatin1().data());
                    Scierror(999, "%s: Wrong value for input argument #1: \"%s\" is not a valid parameter.", fname, mi.key().toLatin1().data());
                }
                else
                {
                    sSpecificConnectionParams.append(mi.key() + " = " + mi.value() + ";");
                }
            }
            db.setConnectOptions(sSpecificConnectionParams);

            //trying to open connection
        }
        else
        {
            //params: provider and connection string
            char *provider, *connStr;

            sciGetStringAt(fname, 1, &provider);

            if(!lsProviders.contains(provider))
            {
                Scierror(999, "Unknown provider: %s\n", provider);
                return 0;
            }

            sciGetStringAt(fname, 2, &connStr);

            QSqlDriver *drv = NULL;


            QMap<QString, QString> *mConnectionStringMembers;
            if(getConnectionStringMembers(QString(connStr), &mConnectionStringMembers) > 0)
            {
                Scierror(999, "Invalid connection string.");
            }

            char *sUser = NULL, *sPassword = NULL, *sHost = NULL, *sDb = NULL;
            int iPort = 0, iSocket = 0;

            //if(!strcmp(provider, "QPSQL"))
            //{
            //    PGconn *con = PQconnectdb(connStr);
            //    drv =  new QPSQLDriver(con);
            //}

            //if (!strcmp(provider, "QMYSQL"))
            //{
            //    MYSQL *conn;
            //    mysql_init(conn);
            //    //need connection parameters
            //    mysql_real_connect(conn,
            //        "host",
            //        "user",
            //        "pass",
            //        "db",
            //        1234, //port
            //        "socket",
            //        12345);                 //client flag

            if (mConnectionStringMembers->contains("Uid"))
            {
                sUser = mConnectionStringMembers->value("Uid").toLatin1().data();
            }
            else
            {
                if (mConnectionStringMembers->contains("UserName"))
                {
                    sUser = mConnectionStringMembers->value("UserName").toLatin1().data();
                }
                else
                {
                    if(mConnectionStringMembers->contains("User ID"))
                    {
                        sUser = mConnectionStringMembers->value("User ID").toLatin1().data();
                    }
                    else
                    {
                        if(mConnectionStringMembers->contains("User"))
                        {
                            sUser = mConnectionStringMembers->value("User").toLatin1().data();
                        }
                    }
                }
            }

            if (mConnectionStringMembers->contains("Password"))
            {
                sPassword = mConnectionStringMembers->value("Password").toLatin1().data();
            }
            else
            {
                if (mConnectionStringMembers->contains("Pwd"))
                {
                    sPassword = mConnectionStringMembers->value("Pwd").toLatin1().data();
                }
            }

            if (mConnectionStringMembers->contains("Host"))
            {
                sHost = mConnectionStringMembers->value("Host").toLatin1().data();
            }
            else
            {
                if (mConnectionStringMembers->contains("Location"))
                {
                    sHost = mConnectionStringMembers->value("Location").toLatin1().data();
                }
                else
                {
                    if(mConnectionStringMembers->contains("User ID"))
                    {
                        sHost = mConnectionStringMembers->value("User ID").toLatin1().data();
                    }
                    else
                    {
                        if(mConnectionStringMembers->contains("Data Source"))
                        {
                            sHost = mConnectionStringMembers->value("Data Source").toLatin1().data();
                        }
                    }
                }
            }

            if (mConnectionStringMembers->contains("Database"))
            {
                sDb = mConnectionStringMembers->value("Database").toLatin1().data();
            }
            else
            {
                if (mConnectionStringMembers->contains("Data Source"))
                {
                    sDb = mConnectionStringMembers->value("Data Source").toLatin1().data();
                }
            }

            // if(!strcmp(provider, "QPSQL"))
            // {
                // PGconn *con = PQconnectdb(connStr);
                // drv =  new QPSQLDriver(con);
            // }

            //if (!strcmp(provider, "QMYSQL"))
            //{
            //    MYSQL *conn;
            //    mysql_init(conn);
            //
            //    mysql_real_connect(conn,
            //        sHost,
            //        sUser,
            //        sPassword,
            //        sDb,
            //        1234, //port
            //        "socket",
            //        12345);                 //client flag

            //    drv = new QMYSQLDriver(conn);
            //}

            //if (!strcmp(provider, "QSQLITE"))
            //{
            //    sqlite3 *conn;
            //    sqlite3_open(connStr, &conn);
            //    drv = new QSQLiteDriver(conn);
            //}

            //if (!strcmp(provider, "QIBASE"))
            //{
            //    isc_db_handle conn;
            //    ISC_STATUS status_vector[20];

            //    isc_attach_database( status_vector, strlen(sDb), sDb, &conn,  &conn);

            //    drv = new QIBaseDriver(conn);
            //}

            //if (!strcmp(provider, "QOCI"))
            //{
            //    OCIEnv *ociEnv = (OCIEnv *)0;
            //    //don't know which parameters to pass...
            //    OCIEnvCreate(&ociEnv, (ub4) OCI_DEFAULT, (dvoid *)0,
   //                         (dvoid * (*)(dvoid *, size_t)) 0,
   //                         (dvoid * (*)(dvoid *, dvoid *, size_t))0,
   //                         (void (*)(dvoid *, dvoid *)) 0,
   //                         (size_t)0, (dvoid **)0);

            //    OCISvcCtx *svchp = (OCISvcCtx *)0;
            //    OCIError *ociErr;
            //    OCILogon2(ociEnv, ociErr, &svchp,
            //        (CONST OraText *)"user", (ub4)strlen("user"),
            //        (CONST OraText *)"pass", (ub4)strlen("pass"),
            //        (CONST OraText *)"poolName", (ub4)strlen("poolname"), OCI_CPOOL);

            //    drv = new QOCIDriver(ociEnv, svchp);
            //}

            //if (!strcmp(provider, "QODBC") || strcmp(provider, "QDB2"))
            //{
            //    SQLHANDLE hEnv;
            //    SQLHANDLE hDbc;

            //    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
            //    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
            //    SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

            //    //need connection parameters
            //    SQLConnect(hDbc, (SQLCHAR*)"server",
            //        (SQLSMALLINT)strlen("server"),
            //        (SQLCHAR*)"user",
            //        (SQLSMALLINT)strlen("user"),
            //        (SQLCHAR*)"pass",
            //        (SQLSMALLINT)strlen("pass"));

            //    if (!strcmp(provider, "QODBC"))
            //        drv = new QODBCDriver(hEnv, hDbc);
            //    else;
            //        //drv = new QDB2Driver(hEnv, hDbc);
            //}

            do
            {
                iRand = rand();
                sprintf(cpRandName, "%d", iRand);
            }
            while (QSqlDatabase::contains(QString(cpRandName)));
            sDefaultConnection = QString(cpRandName);

            db = QSqlDatabase::addDatabase(drv, QString(cpRandName));
        }

        db.open();

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
/* ==================================================================== */
} /* extern "C" */
/* ==================================================================== */
