//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 20 21:01:10 GMT-0800 1997
// Last Modified: Fri Mar 27 23:19:42 GMT-0800 1998
// Filename:      ...sig/code/Filter/Allpass/Allpass.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#include "Allpass.h"
#include <stdlib.h>
#include <math.h>


//////////////////////////////
//
// Allpass::Allpass
//	default value: aMaxDelay = 4000
//

Allpass::Allpass(int aMaxDelay) {
   setName("Allpass");
   brandname = FILTER;
   setMaxDelay(aMaxDelay);
   setDelay((sampleType)1000);        // input channel 1 default value
   setGain(0.95);                     // input channel 2 default value
   clear();
}



//////////////////////////////
//
// Allpass::~Allpass
//

Allpass::~Allpass() { 
   // do nothing
}



//////////////////////////////
//
// Allpass::action
//

void Allpass::action(void) {
   delayline.write(getInput() + getGain() * delayline[-getDelay()]);
   outputValue = delayline[0] * -getGain() + delayline[-getDelay()+1];
}



//////////////////////////////
//
// Allpass::clear -- initializes all internal variables.
//

void Allpass::clear(void) {
   outputValue = 0.0;
   delayline.zero();
}



//////////////////////////////
//
// Allpass::getDelay -- input channel 2.
//

sampleType Allpass::getDelay(void) {
   return inputs[2];
}



//////////////////////////////
//
// Allpass::getGain -- input channel 1.
//

sampleType Allpass::getGain(void) {
   return inputs[1];
}



//////////////////////////////
//
// Allpass::getInput -- input channel 0.
//

sampleType Allpass::getInput(void) {
   return inputs[0];
}



//////////////////////////////
//
// Allpass::getLoopTime -- returns the current loop delay time in seconds.
//

sampleType Allpass::getLoopTime(void) {
   return 1.0/getDelay()/getSrate();
}



//////////////////////////////
//
// Allpass::getMaxDelay -- returns the maximum delay in samples that
//	can currently be handled, unless setMaxDelay is called.
//

sampleType Allpass::getMaxDelay(void) {
   return delayline.getSize() - 1;
}



//////////////////////////////
//
// Allpass::getReverbTime -- returns the current reverb time
//	according to the gain and delay settings
//	Still need to check if this is right.
//

sampleType Allpass::getReverbTime(void) {
   return -3.0 * getLoopTime() / log10(getGain());   
}



//////////////////////////////
//
// Allpass::output -- returns the current output of the filter.
//	default value: channel = 0;
//

sampleType Allpass::output(int channel) {
   return outputValue;
}



//////////////////////////////
//
// Allpass::setDelay -- input channel 2 is the delay.
//      default value: outputChannel = 0
//

void Allpass::setDelay(sampleType aValue) {
   connect(aValue, 2);
}

void Allpass::setDelay(Signal& aSignal, int outputChannel) {
   connect(aSignal, 2, outputChannel);
}

void Allpass::setDelay(Signal* aSignal, int outputChannel) {
   connect(aSignal, 2, outputChannel);
}



//////////////////////////////
//
// Allpass::setGain -- input channel 1 is the gain.
//      default value: outputChannel = 0
//

void Allpass::setGain(sampleType aGain) {
   connect(aGain, 1);
}

void Allpass::setGain(Signal& aSignal, int outputChannel) {
   connect(aSignal, 1, outputChannel);
}

void Allpass::setGain(Signal* aSignal, int outputChannel) {
   connect(aSignal, 1, outputChannel);
}



//////////////////////////////
//
// Allpass::setInput -- input channel 0 is the incoming signal.
//      default value: outputChannel = 0
//

void Allpass::setInput(sampleType aValue) {
   connect(aValue, 0);
}

void Allpass::setInput(Signal& aSignal, int outputChannel) {
   connect(aSignal, 0, outputChannel);
}

void Allpass::setInput(Signal* aSignal, int outputChannel) {
   connect(aSignal, 0, outputChannel);
}



//////////////////////////////
//
// Allpass::setLoopTime -- can be used if no signal
//	is connected to input channel 1.  Input value 
//	is a time in seconds.  Will increase the
//	delayline size automatically if necessary.
//

void Allpass::setLoopTime(sampleType aLoopTime) {
   sampleType aDelay = getSrate()/aLoopTime;
   if (getMaxDelay() < aDelay) {
      setMaxDelay((int)aDelay);
   }
   setDelay(aDelay);
}



//////////////////////////////
//
// Allpass::setMaxDelay -- this is the maximum value that can
//	occur on input channel 1 (for predicitable results).
//	Default value for class construction is a delay of 4000 samples.
//

void Allpass::setMaxDelay(int aMax) {
   delayline.setSize(aMax);
}



//////////////////////////////
//
// Allpass::setReverbTime -- can be used if no signal is
//	connected to input channel 2.
//

void Allpass::setReverbTime(sampleType aReverbTime) {
   setGain(pow(0.001, getLoopTime()/aReverbTime));
}



// md5sum:	86c43c74a90eb7c18443d39abeb02ba8  Allpass.cpp
