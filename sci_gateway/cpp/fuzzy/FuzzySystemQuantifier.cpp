#include "FuzzySystemQuantifier.h"

FuzzySystemQuantifier::FuzzySystemQuantifier(void)
{
}

FuzzySystemQuantifier::~FuzzySystemQuantifier(void)
{
}

QString FuzzySystemQuantifier::fuzzyName() { return _fuzzyName; }
int FuzzySystemQuantifier::fuzzyType() { return _fuzzyType; }
double FuzzySystemQuantifier::alpha() { return _alpha; }
double FuzzySystemQuantifier::beta() { return _beta; }
double FuzzySystemQuantifier::gamma() { return _gamma; }
double FuzzySystemQuantifier::delta() { return _delta; }
