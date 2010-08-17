#include <QtCore>

class __declspec( dllexport ) FuzzyTableInfo
{
	int _tableId;
	QString _name;

public:
	int tableId();
	QString name();

	FuzzyTableInfo(void);
	FuzzyTableInfo::FuzzyTableInfo(int id, QString name);

	~FuzzyTableInfo(void);
};
