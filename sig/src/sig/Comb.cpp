//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 20 19:03:37 GMT-0800 1997
// Last Modified: Fri Mar 27 23:35:58 GMT-0800 1998
// Filename:      ...sig/code/Filter/Comb/Comb.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#include "Comb.h"
#include <stdlib.h>


//////////////////////////////
//
// Comb::Comb
//

Comb::Comb(void) {
   setName("Comb");
   brandname = FILTER;
   setMinFund(12);             // default minimum fundamental of comb is 12 Hz.
   setGain(0.5);               // related to reverb time of sound
   setFund(440);               // fundamental frequency of comb filter
   clear();
}



//////////////////////////////
//
// Comb::~Comb
//

Comb::~Comb() { 
   // do nothing
}



//////////////////////////////
//
// Comb::action
//

void Comb::action(void) {
   delayline.write(getSignal() + getGain() * outputValue);
   outputValue = delayline[getDelay()+1];
}



//////////////////////////////
//
// Comb::clear -- initialize internal variables
//

void Comb::clear(void) {
   outputValue = 0.0;
   delayline.zero();
}



//////////////////////////////
//
// Comb::getDelay
//

sampleType Comb::getDelay(void) {
   return getSrate()/getFund();
}



//////////////////////////////
//
// Comb::getFund
//

sampleType Comb::getFund(void) {
   return inputs[2];
}



//////////////////////////////
//
// Comb::getMinFund
//

sampleType Comb::getMinFund(void) {
   return delayline.getSize()/getSrate();
}



//////////////////////////////
//
// Comb::getGain
//

sampleType Comb::getGain(void) {
   return inputs[1];
}



//////////////////////////////
//
// Comb::getSignal
//

sampleType Comb::getSignal(void) {
   return inputs[0];
}



//////////////////////////////
//
// Comb::output
//

sampleType Comb::output(int channel) {
   return outputValue;
}



//////////////////////////////
//
// Comb::setDelay -- sets the delay in samples
//

void Comb::setDelay(sampleType aDelay) {
   connect(getSrate()/aDelay, 2);
}



//////////////////////////////
//
// Comb::setFund
//

void Comb::setFund(sampleType aFund) {
   connect(aFund, 2);
}



//////////////////////////////
//
// Comb::setLoopTime -- sets the delay time in seconds.
//

void Comb::setLoopTime(sampleType aLoopTime) {
   setDelay(getSrate()/aLoopTime);
}
   


//////////////////////////////
//
// Comb::setMinFund
//

void Comb::setMinFund(sampleType aMin) {
   delayline.setSize((int)(getSrate()/aMin));
}



//////////////////////////////
//
// Comb::setReverbTime
//

void Comb::setReverbTime(sampleType aReverbTime) {
   connect(1.0/aReverbTime, 1);
}



//////////////////////////////
//
// Comb::setGain
//

void Comb::setGain(sampleType aGain) {
   connect(aGain, 1);
}



// md5sum:	a1bc3df076c75e6dd4fcac570b945526  Comb.cpp
