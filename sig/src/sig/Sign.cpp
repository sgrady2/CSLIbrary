//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Mar 25 17:12:01 GMT-0800 1998
// Last Modified: Wed Mar 25 17:12:03 GMT-0800 1998
// Filename:      ...sig/code/Filter/Sign/Sign.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#include "Sign.h"
#include <stdlib.h>


//////////////////////////////
//
// Sign::Sign
//

Sign::Sign(void) {
   setName("Sign");
   brandname = FILTER;
   clear();
}



//////////////////////////////
//
// Sign::~Sign
//

Sign::~Sign() { 
   // do nothing
}



//////////////////////////////
//
// Sign::action
//

void Sign::action(void) {
   if (getInput() < 0) {
      outputValue = -1;
   } else if (getInput() > 0) {
      outputValue = 1;
   } else {
      outputValue = 0;
   }
}



//////////////////////////////
//
// Sign::clear
//

void Sign::clear(void) {
   outputValue = 0.0;
}



//////////////////////////////
//
// Sign::getInput -- returns the current value of the input signal on
//	channel 0
//

sampleType Sign::getInput(void) {
   return inputs[0];
}



//////////////////////////////
//
// Sign::output
//

sampleType Sign::output(int channel) {
   return outputValue;
}



//////////////////////////////
//
// Sign::printState
//

void Sign::printState(void) {
   cout << "Current output = " << output(0) << endl;
   cout << "Current input = " << getInput() << endl;
}



//////////////////////////////
//
// Sign::setInput
//      default value: outputChannel = 0
//

void Sign::setInput(sampleType aValue) {
   connect(aValue, 0);
}

void Sign::setInput(Signal& aSignal, int outputChannel) {
   connect(aSignal, 0, outputChannel);
}

void Sign::setInput(Signal* aSignal, int outputChannel) {
   connect(aSignal, 0, outputChannel);
}



// md5sum:	a52edbd9abfabcb3d57b329d72d731df  Sign.cpp
