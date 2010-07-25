#ifndef FUZZYSQL_GLOBAL_H
#define FUZZYSQL_GLOBAL_H

#include <Qt/qglobal.h>

#ifdef FUZZYSQL_LIB
# define FUZZYSQL_EXPORT Q_DECL_EXPORT
#else
# define FUZZYSQL_EXPORT Q_DECL_IMPORT
#endif

#endif // FUZZYSQL_GLOBAL_H
