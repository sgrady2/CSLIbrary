//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu May 15 13:28:50 GMT-0800 1997
// Last Modified: Thu May 15 13:28:53 GMT-0800 1997
// Filename:      ...sig/code/Generator/Impulse/Impulse.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "Impulse.h"
#include <iostream.h>


//////////////////////////////
//
// Impulse::Impulse
//	default value: aPeriod = 0x7fffffff, anAmplitude = 1.0
//

Impulse::Impulse(long aPeriod, sampleType anAmplitude) {
   setName("Impulse");
   setAmplitude(anAmplitude);
   setPeriod(aPeriod);
   brandname = GENERATOR;
   outputValue = 0;
   trigger();
}



//////////////////////////////
//
// Impulse::~Impulse
//

Impulse::~Impulse() { }



//////////////////////////////
//
// Impulse::action
//

void Impulse::action(void) { 
   if (counter % period) {
      outputValue = 0;
   } else {
      outputValue = amplitude;
   } 
   counter++;
}



//////////////////////////////
//
// Impulse::getAmplitude
//

sampleType Impulse::getAmplitude(void) {
   return amplitude;
}



//////////////////////////////
//
// Impulse::getPeriod
//

long Impulse::getPeriod(void) {
   return period;
}



//////////////////////////////
//
// Impulse::output
//

sampleType Impulse::output(int channel) { 
   return outputValue;
}



//////////////////////////////
//
// Impulse::printState
//

void Impulse::printState(void) { 
   cerr << " Period: " << getPeriod() << endl;
   cerr << " Amplitude: " << getAmplitude() << endl;
}



//////////////////////////////
//
// Impulse::setAmplitude
//

void Impulse::setAmplitude(sampleType anAmplitude) { 
   amplitude = anAmplitude;
}



//////////////////////////////
//
// Impulse::setPeriod
//

void Impulse::setPeriod(long aPeriod) { 
   period = aPeriod;
}



//////////////////////////////
//
// Impulse::setValue
//	default value: counterValue = 0
//

void Impulse::trigger(long counterValue) { 
   counter = counterValue;
}


// md5sum:	cf49df2ab415b2db85f466ded4c26a4e  Impulse.cpp
