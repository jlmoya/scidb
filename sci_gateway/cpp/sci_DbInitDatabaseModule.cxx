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
/* ==================================================================== */
} /* extern "C" */
/* ==================================================================== */
