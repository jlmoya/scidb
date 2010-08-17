#ifndef FUZZYOBJECTTYPE2_H
#define FUZZYOBJECTTYPE2_H

#include "QtIncludes.h"
#include "FMB.h"
#include "FuzzyLabel.h"

class __declspec( dllexport ) FuzzyConstant
{
public:
    FuzzyConstant();
    FuzzyConstant(QString str, FMB fuzzyMetaBase);

    int type;
    double* data;
    QList<QString> labels;
};

#endif // FUZZYOBJECTTYPE2_H
