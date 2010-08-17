#include <QtCore>

class __declspec( dllexport ) FuzzyColumnInfo
{
	int _columnId;
	QString _name;

public:
	int columnId();
	QString name();
	FuzzyColumnInfo(void);
	~FuzzyColumnInfo(void);
};
