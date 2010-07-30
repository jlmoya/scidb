#ifndef FUZZYOBJECTTYPE2_H
#define FUZZYOBJECTTYPE2_H

#include <QtSql>
#include "FMB.h"
#include "FuzzyLabel.h"

class __declspec( dllexport ) FuzzyConstantType2
{
public:
    FuzzyConstantType2();    
    FuzzyConstantType2(QString str, FMB fuzzyMetaBase);

    int type;
    double* data;
};

#endif // FUZZYOBJECTTYPE2_H
