#include "FuzzyLabel.h"

FuzzyLabel::FuzzyLabel(void)
{
}

FuzzyLabel::~FuzzyLabel(void)
{
}

int FuzzyLabel::fuzzyId(){return _fuzzyId; }
double FuzzyLabel::alpha(){ return _alpha; }
double FuzzyLabel::beta() { return _beta; }
double FuzzyLabel::gamma() { return _gamma; }
double FuzzyLabel::delta() { return _delta; }
