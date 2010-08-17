#include "FuzzyDegreeSig.h"

FuzzyDegreeSig::FuzzyDegreeSig(QSqlQuery qry)
{
	_codeSig = qry.value(qry.record().indexOf("code_sig")).toInt();
	_significance = qry.value(qry.record().indexOf("significance")).toString();
}

FuzzyDegreeSig::FuzzyDegreeSig(void)
{
}

FuzzyDegreeSig::~FuzzyDegreeSig(void)
{
}

int FuzzyDegreeSig::codeSig(){ return _codeSig; }
QString FuzzyDegreeSig::significance(){ return _significance; }
