//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun May 18 19:35:41 GMT-0800 1997
// Last Modified: Sun May 18 19:35:44 GMT-0800 1997
// Filename:      ...sig/code/src/Filter/Switch/Switch.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "Switch.h"
#include <stdlib.h>


//////////////////////////////
//
// Switch::Switch
//

Switch::Switch(int switchSample) {
   setDurationSamples(switchSample);
   trigger();
   setName("Switch");
   brandname = FILTER;
}



//////////////////////////////
//
// Switch::~Switch
//

Switch::~Switch() { }



//////////////////////////////
//
// Switch::action
//

void Switch::action(void) { 
   periodCounter++;
   if (periodCounter >= period) {
      if (inputs.signal(1) != NULL) {
         inputs.signal(1)->tick(tickNumber);
      }
   } else {
      if (inputs.signal(0) != NULL) {
         inputs.signal(0)->tick(tickNumber);
      }
   }
}



//////////////////////////////
//
// Switch::getDurationSamples
//

int Switch::getDurationSamples(void) {
   return period;
}



//////////////////////////////
//
// Switch::output
//

sampleType Switch::output(int channel) { 
   if (periodCounter >= period) {
      return inputs[1];
   } else {
      return inputs[0];
   }
}



//////////////////////////////
//
// Switch::setDurationSamples
//

void Switch::setDurationSamples(int switchSample) {
   if (switchSample <= 0) {
      period = 0;
   } else {
      period = switchSample;
   }
}



//////////////////////////////
//
// Switch::tick
//

void Switch::tick(long newTickNumber) {
   if (newTickNumber != tickNumber) {
      tickNumber = newTickNumber;
      action();
   }
}
         


//////////////////////////////
//
// Switch::trigger
//

void Switch::trigger(void) {
   periodCounter = 0;
}



// md5sum:	92c5d44795184e49bd99c9dd3babe48b  Switch.cpp
