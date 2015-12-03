//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Jul  6 20:38:49 PDT 1997
// Last Modified: Sat Jul 26 14:55:23 GMT-0800 1997
// Filename:      ...sig/code/Filter/Allpass1/Allpass1.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "Allpass1.h"
#include <iostream.h>


//////////////////////////////
//
// Allpass1::Allpass1
//

Allpass1::Allpass1(void) {
   setName("Allpass1");
   brandname = FILTER;
   setGain(0.5);
   clear();
}



//////////////////////////////
//
// Allpass1::~Allpass1
//

Allpass1::~Allpass1() { }



//////////////////////////////
//
// Allpass1::action
//

void Allpass1::action(void) {
   outputValue = holdValue + getGain() * (outputValue - getInput());
   holdValue = getInput();
}



//////////////////////////////
//
// Allpass1::clear
//

void Allpass1::clear(void) {
   outputValue = 0.0;
   holdValue = 0.0;
}


//////////////////////////////
//
// Allpass1::getInput
//

sampleType Allpass1::getInput(void) {
   return inputs[0];
}



//////////////////////////////
//
// Allpass1::getGain
//

sampleType Allpass1::getGain(void) {
   return inputs[1];
}



//////////////////////////////
//
// Allpass1::output
//

sampleType Allpass1::output(int channel) {
   return outputValue;
}



//////////////////////////////
//
// Allpass1::printState
//

void Allpass1::printState(void) {
   cout << "   input signal = " << getInput() << endl;
   cout << "   gain         = " << getGain()  << endl;
}



//////////////////////////////
//
// Allpass1::setInput
//	default value: outputChannel = 0
//

void Allpass1::setInput(sampleType aValue) {
   connect(aValue, 0);
}

void Allpass1::setInput(Signal& aSignal, int outputChannel) {
   connect(aSignal, 0);
}

void Allpass1::setInput(Signal* aSignal, int outputChannel) {
   connect(aSignal, 0);
}



//////////////////////////////
//
// Allpass1::setGain
//	default value: outputChannel = 0
//

void Allpass1::setGain(sampleType aGain) {
   connect(aGain, 1);
}

void Allpass1::setGain(Signal& aSignal, int outputChannel) {
   connect(aSignal, 1);
}

void Allpass1::setGain(Signal* aSignal, int outputChannel) {
   connect(aSignal, 1);
}



// md5sum:	9e026afbdb685efddbaa33a060092755  Allpass1.cpp
