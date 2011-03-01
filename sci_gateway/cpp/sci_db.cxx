/* ==================================================================== */
/* Igor GRICHYN */
/* Scidb toolbox */
/* This file is released into the public domain */
/* ==================================================================== */

/* ==================================================================== */
#include "sci_db.h"
/* ==================================================================== */

QString sDefaultConnection = "";

QMap< QString,QList<QString> > mslsProviderConnectionOptions = QMap< QString, QList<QString> >();

QList<QString> lsCommonConnectionParameters = QList<QString>();
	
QList<QString> lsProviders = QList<QString>();