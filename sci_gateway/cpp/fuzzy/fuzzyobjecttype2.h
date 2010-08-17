#ifndef FUZZYOBJECTTYPE2_H
#define FUZZYOBJECTTYPE2_H

#include <QtSql>

class __declspec( dllexport ) FuzzyObjectType2
{
public:
    FuzzyObjectType2();
    FuzzyObjectType2(QString str);

    int type;
    double* data;
};

#endif // FUZZYOBJECTTYPE2_H
