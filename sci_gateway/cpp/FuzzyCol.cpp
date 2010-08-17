#include "FuzzyCol.h"

FuzzyCol::FuzzyCol(QSqlQuery qry)
{
    _tableId = qry.value(qry.record().indexOf("obj")).toInt();
    _columnId = qry.value(qry.record().indexOf("col")).toInt();
    _fType = qry.value(qry.record().indexOf("f_type")).toInt();
     _len = qry.value(qry.record().indexOf("len")).toInt();
    _codeSig  = qry.value(qry.record().indexOf("code_sig")).toInt();
    _columnName = qry.value(qry.record().indexOf("column_name")).toString();
    _com = qry.value(qry.record().indexOf("com")).toString();
    _um = qry.value(qry.record().indexOf("um")).toString();
}

FuzzyCol::FuzzyCol(    int tableId,
                int columnId,
                int fType,
                int len,
                int codeSig,
                QString columnName,
                QString com,
                QString um)
{
    _tableId = tableId;
    _columnId = columnId;
    _fType = fType;
    _len = len;
    _codeSig = codeSig;
    _columnName = columnName;
    _com = com;
    _um = um;
}

FuzzyCol::FuzzyCol(QMap<QString, QVariant> *mValues)
{
    if (mValues->contains("tableId"))
        _tableId = mValues->value("tableId").toInt();

    if (mValues->contains("columnId"))
        _columnId = mValues->value("columnId").toInt();

    if (mValues->contains("fType"))
        _fType = mValues->value("fType").toInt();

    if (mValues->contains("len"))
        _len = mValues->value("len").toInt();

    if (mValues->contains("codeSig"))
        _codeSig = mValues->value("codeSig").toInt();

    if (mValues->contains("columnName"))
        _columnName = mValues->value("columnName").toString();

    if (mValues->contains("com"))
        _com = mValues->value("com").toString();

    if (mValues->contains("um"))
        _um = mValues->value("um").toString();
}

FuzzyCol::FuzzyCol(void)
{
}

FuzzyCol::~FuzzyCol(void)
{
}

int FuzzyCol::tableId()
{
    return _tableId;
}
int FuzzyCol::columnId(){ return _columnId; }
int FuzzyCol::fType(){ return _fType; }
int FuzzyCol::len(){ return _len; }
int FuzzyCol::codeSig(){ return _codeSig; }
QString FuzzyCol::columnName(){ return _columnName; }
QString FuzzyCol::com(){ return _com; }
QString FuzzyCol::um(){ return _um; }

QMap<QString, QVariant>* FuzzyCol::ValuesMap()
{
    QMap<QString, QVariant> *pMap = new QMap<QString, QVariant>();

    pMap->insert("columnId", QVariant(_columnId));
    pMap->insert("fType", QVariant(_fType));
    pMap->insert("len", QVariant(_len));
    pMap->insert("codeSig", QVariant(_codeSig));
    pMap->insert("columnName", QVariant(_columnName));
    pMap->insert("com", QVariant(_com));
    pMap->insert("um", QVariant(_um));

    return pMap;
}

