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
/* ==================================================================== */
} /* extern "C" */
/* ==================================================================== */
