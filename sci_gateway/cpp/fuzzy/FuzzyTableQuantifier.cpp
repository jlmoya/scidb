#include "FuzzyTableQuantifier.h"

FuzzyTableQuantifier::FuzzyTableQuantifier(void)
{
}

FuzzyTableQuantifier::~FuzzyTableQuantifier(void)
{
}

int FuzzyTableQuantifier::tableId() { return _tableId; }
QString FuzzyTableQuantifier::fuzzyName() { return _fuzzyName; }
int FuzzyTableQuantifier::fuzzyType() { return _fuzzyType; }
double FuzzyTableQuantifier::alpha() { return _alpha; }
double FuzzyTableQuantifier::beta() { return _beta; }
double FuzzyTableQuantifier::gamma() { return _gamma; }
double FuzzyTableQuantifier::delta() { return _delta; }