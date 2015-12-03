//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Apr 30 14:58:11 GMT-0800 1997
// Last Modified: Wed Apr 30 14:58:14 GMT-0800 1997
// Filename:      ...sig/code/Generator/FixedSine/FixedSine.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "FixedSine.h"
#include <iostream.h>
#include <math.h>


//////////////////////////////
//
// FixedSine::FixedSine
//	default values: anAmplitude = 1, aPhase = 0, aSrate = 44100
//

FixedSine::FixedSine(float aFrequency, float anAmplitude, float aPhase, 
     float aSrate) {
   setName("FixedSine");
   setFrequency(aFrequency);
   setAmplitude(anAmplitude);
   setPhase(aPhase);
   setSrate(aSrate);
   reset();
   outputValue = 0;
   brandname = GENERATOR;
}



//////////////////////////////
//
// FixedSine::~FixedSine
//

FixedSine::~FixedSine() { }



//////////////////////////////
//
// FixedSine::action
//

void FixedSine::action(void) { 
   outputValue = amplitude * 
      sin((angularFrequency * sampleNum + phase)/getSrate());
   sampleNum++;
}



//////////////////////////////
//
// FixedSine::getAmplitude
//

float FixedSine::getAmplitude(void) {
   return amplitude;
}



//////////////////////////////
//
// FixedSine::getFrequency
//

float FixedSine::getFrequency(void) { 
   return (float)(angularFrequency/2.0/PI);
}



//////////////////////////////
//
// FixedSine::getPhase
//

float FixedSine::getPhase(void) {
   return (float)(phase / PI);
}



//////////////////////////////
//
// FixedSine::output
//

sampleType FixedSine::output(int channel) { 
   return outputValue;
}



//////////////////////////////
//
// FixedSine::printState
//

void FixedSine::printState(void) { 
   cerr << "frequency = " << getFrequency() << endl;
   cerr << "srate     = " << getSrate() << endl;
   cerr << "phase     = " << getPhase() << endl;
   cerr << "time      = " << sampleNum << endl;
}



//////////////////////////////
//
// FixedSine::reset
//

void FixedSine::reset(void) {
   sampleNum = 0;
}



//////////////////////////////
//
// FixedSine::setAmplitude
//

void FixedSine::setAmplitude(float anAmplitude) {
   amplitude = anAmplitude;
}



//////////////////////////////
//
// FixedSine::setFrequency
//

void FixedSine::setFrequency(float aFrequency) { 
   angularFrequency = (float)(2.0*PI*aFrequency);
}



//////////////////////////////
//
// FixedSine::setPhase
//

void FixedSine::setPhase(float aPhase) {
   phase = (float)(aPhase * PI);
}


// md5sum:	718727f08478fe3a7cb30deb714eeb67  FixedSine.cpp
