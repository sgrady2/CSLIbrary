//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Feb  8 23:12:20 PST 1999
// Last Modified: Mon Feb  8 23:12:25 PST 1999
// Filename:      ...sig/code/Filter/Smoother/Smoother.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "Smoother.h"


//////////////////////////////
//
// Smoother::Smoother
//	default values: aFactor = 0.1
//

Smoother::Smoother(sampleType aFactor) {
   clear();
   setFactor(aFactor);
   setInput(0);

   setName("Smoother");
   brandname = FILTER;
}



//////////////////////////////
//
// Smoother::~Smoother
//

Smoother::~Smoother() { }



//////////////////////////////
//
// Smoother::action
//    difference equation:   y[n] = y[n-1] + k(x[n] - y[n-1])
//                           y[n] = k x[n] + (1-k) y[n-1]
//

void Smoother::action(void) {
   currentOutput = getFactor() * getInput() + (1-getFactor()) * currentOutput;
}



//////////////////////////////
//
// Smoother::getInput
//

sampleType Smoother::getInput(void) {
   return inputs[0];
}



//////////////////////////////
//
// Smoother::getFactor -- the k value in the difference equation above.
//

sampleType Smoother::getFactor(void) {
   return inputs[1];
}



//////////////////////////////
//
// Smoother::output
//

sampleType Smoother::output(int channel) {
   return currentOutput;
}



//////////////////////////////
//
// Smoother::setFactor
//

void Smoother::setFactor(sampleType aFactor) {
   inputs.setDefault(1, aFactor);
}


void Smoother::setFactor(Signal& aSignal, int outputChannel) {
   connect(aSignal, 1, outputChannel);
}


void Smoother::setFactor(Signal* aSignal, int outputChannel) {
   connect(aSignal, 1, outputChannel);
}



//////////////////////////////
//
// Smoother::setInput
//

void Smoother::setInput(sampleType anInput) {
   inputs.setDefault(0, anInput);
}


void Smoother::setInput(Signal& aSignal, int outputChannel) {
   connect(aSignal, 0, outputChannel);
}


void Smoother::setInput(Signal* aSignal, int outputChannel) {
   connect(aSignal, 0, outputChannel);
}



// md5sum:	ba061fedd29f2b2a68940205aca6148b  Smoother.cpp
