#include "FuzzyObject.h"

FuzzyObject::FuzzyObject(void)
{
}

FuzzyObject::~FuzzyObject(void)
{
}

int FuzzyObject::columnId() {return _columnId; }
int FuzzyObject::fuzzyId() { return _fuzzyId; }
QString FuzzyObject::fuzzyName() { return _fuzzyName; }
int FuzzyObject::fuzzyType() { return _fuzzyType; }
