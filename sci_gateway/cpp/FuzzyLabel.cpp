#include "FuzzyLabel.h"

FuzzyLabel::FuzzyLabel(QSqlQuery qry)
{
	_fuzzyId = qry.value(qry.record().indexOf("fuzzy_id")).toInt();
	_alpha = qry.value(qry.record().indexOf("alpha")).toDouble();
	_beta= qry.value(qry.record().indexOf("beta")).toDouble();
	_gamma = qry.value(qry.record().indexOf("gamma")).toDouble();
	_delta = qry.value(qry.record().indexOf("delta")).toDouble();
}

FuzzyLabel::FuzzyLabel(	int fuzzyId,
				double alpha, double beta, double gamma, double delta)
{
	_fuzzyId = fuzzyId;
	_alpha = alpha;
	_beta = beta;
	_gamma = gamma;
	_delta = delta;
}

FuzzyLabel::FuzzyLabel(void)
{
}

FuzzyLabel::~FuzzyLabel(void)
{
}

void FuzzyLabel::SetValues(double alpha, double beta, double gamma, double delta)
{
	_alpha = alpha;
	_beta = beta;
	_gamma = gamma;
	_delta = delta;
}

FuzzyLabel::FuzzyLabel(QMap<QString, QVariant> *mValues)
{
	if (mValues->contains("fuzzyId"))
		_fuzzyId = mValues->value("fuzzyId").toInt();
	if (mValues->contains("alpha"))
		_alpha = mValues->value("alpha").toDouble();
	if (mValues->contains("beta"))
		_beta = mValues->value("beta").toDouble();
	if (mValues->contains("gamma"))
		_gamma = mValues->value("gamma").toDouble();
	if (mValues->contains("delta"))
		_delta = mValues->value("delta").toDouble();


}

int FuzzyLabel::fuzzyId(){return _fuzzyId; }
double FuzzyLabel::alpha(){ return _alpha; }
double FuzzyLabel::beta() { return _beta; }
double FuzzyLabel::gamma() { return _gamma; }
double FuzzyLabel::delta() { return _delta; }

QMap<QString, QVariant>* FuzzyLabel::ValuesMap()
{
	QMap<QString, QVariant> *pMap = new QMap<QString, QVariant>();

	pMap->insert("fuzzyId", QVariant(_fuzzyId));
	pMap->insert("alpha", QVariant(_alpha));
	pMap->insert("beta", QVariant(_beta));			
	pMap->insert("gamma", QVariant(_gamma));
	pMap->insert("delta", QVariant(_delta));

	return pMap;
}
