//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Mar 25 00:38:06 GMT-0800 1998
// Last Modified: Wed Mar 25 14:51:36 GMT-0800 1998
// Filename:      ...sig/code/Filter/DCBlock/DCBlock.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#include "DCBlock.h"


//////////////////////////////
//
// DCBlock::DCBlock
//

DCBlock::DCBlock(void) {
   setName("DCBlock");
   brandname = FILTER;
   setPolePosition(0.995);
   clear();
}



//////////////////////////////
//
// DCBlock::~DCBlock
//

DCBlock::~DCBlock() { 
   // do nothing
}



//////////////////////////////
//
// DCBlock::action
//

void DCBlock::action(void) {
   // difference equation:  y[n] = x[n] - x[n-1] + a * y[n-1]
   // where a = pole position on the z-plane

   outputValue = getInput() - previousInput + getPolePosition() * outputValue;
   previousInput = getInput();
}



//////////////////////////////
//
// DCBlock::clear
//

void DCBlock::clear(void) {
   outputValue = 0.0;
   previousInput = 0.0;
}



//////////////////////////////
//
// DCBlock::getInput -- returns the current value of the input signal
//	on channel 0
//

sampleType DCBlock::getInput(void) {
   return inputs[0];
}



//////////////////////////////
//
// DCBlock::getPolePosition -- returns the current value of the pole
//	position on channel 1.
//

sampleType DCBlock::getPolePosition(void) {
   return inputs[1];
}



//////////////////////////////
//
// DCBlock::output
//

sampleType DCBlock::output(int channel) {
   return outputValue;
}



//////////////////////////////
//
// DCBlock::printState
//

void DCBlock::printState(void) {
   cout << "Signal input   = " << getInput() << endl;
   cout << "Pole position  = " << getPolePosition() << endl;
   cout << "Current output = " << output(0) << endl;
}



//////////////////////////////
//
// DCBlock::setInput
//      default value: outputChannel = 0
//

void DCBlock::setInput(sampleType aValue) {
   connect(aValue, 0);
}

void DCBlock::setInput(Signal& aSignal, int outputChannel) {
   connect(aSignal, 0, outputChannel);
}

void DCBlock::setInput(Signal* aSignal, int outputChannel) {
   connect(aSignal, 0, outputChannel);
}



//////////////////////////////
//
// DCBlock::setPolePosition
//      default value: outputChannel = 0
//

void DCBlock::setPolePosition(sampleType aValue) {
   connect(aValue, 1);
}

void DCBlock::setPolePosition(Signal& aSignal, int outputChannel) {
   connect(aSignal, 1, outputChannel);
}

void DCBlock::setPolePosition(Signal* aSignal, int outputChannel) {
   connect(aSignal, 1, outputChannel);
}



// md5sum:	5dca94a72f10df108c4114e562ed7956  DCBlock.cpp
