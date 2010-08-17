#include <QtCore>
#include <QtSql>

#include "FuzzyCol.h"
#include "FuzzyTableInfo.h"
#include "FuzzyDegreeSig.h"
#include "FuzzyObject.h"
#include "FuzzyLabel.h"
#include "FuzzyApproxMuch.h"
#include "FuzzyNearness.h"
#include "FuzzyCompatibleCol.h"
#include "FuzzyQualifier.h"
#include "FuzzyDegreeTable.h"
#include "FuzzyTableQuantifier.h"
#include "FuzzySystemQuantifier.h"

class __declspec( dllexport ) FMB 
{
	QMap<QString, FuzzyTableInfo> _mFuzzyTables;
	
	QMap<int, FuzzyCol> _mFuzzyColumns;
	QMap<int, FuzzyDegreeSig> _mFuzzyDegreeSig;
	QMap<int, FuzzyObject> _mFuzzyObjects;
	QMap<int, FuzzyLabel> _mFuzzyLables;
	QMap<int, FuzzyApproxMuch> _mFuzzyApproxMuch;
	QMap<int, QList<FuzzyNearness>> _mFuzzyNearness;
	QMap<int, QList<FuzzyCompatibleCol>> _mFuzzyCompatibleColumnAdapters;
	QMap<int, QList<FuzzyCompatibleCol>> _mFuzzyCompatibleColumnAdaptees;
	QMap<int, FuzzyQualifier> _mFuzzyQualifiers;
	QMap<int, int> _mFuzzyDegreesAssociatedColumns;	
	QMap<int, FuzzyDegreeTable> _mFuzzyyTableDegrees;
	QMap<QString, FuzzyTableQuantifier> _mFuzzyTableQuantifiers;
	QMap<QString, FuzzySystemQuantifier> _mFuzzySystemQuantifiers;

	QMap<QString, FuzzyLabel> _mFuzzyLabelsByName;
	QMap<QString, FuzzyCol> _mFuzzyColumnsByName;

	QSqlDatabase *_db;

public:
	///loadsinfo from tables and fills collections
	void LoadFMB();
	void SetDb(QSqlDatabase *pDb);

	FMB(void);
	~FMB(void);
};
