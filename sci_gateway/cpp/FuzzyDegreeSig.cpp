#include "FuzzyDegreeSig.h"

FuzzyDegreeSig::FuzzyDegreeSig(QSqlQuery qry)
{
	_codeSig = qry.value(qry.record().indexOf("code_sig")).toInt();
	_significance = qry.value(qry.record().indexOf("significance")).toString();
}

FuzzyDegreeSig::FuzzyDegreeSig(	int codeSig,
					QString significance)
{
	_codeSig = codeSig;
	_significance = significance;
}

FuzzyDegreeSig::FuzzyDegreeSig(QMap<QString, QVariant> *mValues)
{	
	if (mValues->contains("codeSig"))
		_codeSig = mValues->value("codeSig").toInt();

	if (mValues->contains("significance"))
		_significance = mValues->value("significance").toString();

	
}

FuzzyDegreeSig::FuzzyDegreeSig(void)
{
}

FuzzyDegreeSig::~FuzzyDegreeSig(void)
{
}

int FuzzyDegreeSig::codeSig(){ return _codeSig; }
QString FuzzyDegreeSig::significance(){ return _significance; }

QMap<QString, QVariant>* FuzzyDegreeSig::ValuesMap()
{
	QMap<QString, QVariant> *pMap = new QMap<QString, QVariant>();

	pMap->insert("codeSig", QVariant(_codeSig));
	pMap->insert("significance", QVariant(_significance));	

	return pMap;
}