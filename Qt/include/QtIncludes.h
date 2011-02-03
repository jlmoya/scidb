#ifndef __QTINCLUDES_H__
#define __QTINCLUDES_H__

#if defined(WIN32) || defined(WIN64)
#define FSQL_EXPORT __declspec( dllexport )
#else
#define FSQL_EXPORT
#endif

#include <QtCore/qmap.h>
#include <QtCore/qvariant.h>
#include <QtCore/qstringlist.h>

#include <QtSql/qsqlquery.h>
#include <QtSql/qsqlrecord.h>
#include <QtSql/qsqlerror.h>

#endif
