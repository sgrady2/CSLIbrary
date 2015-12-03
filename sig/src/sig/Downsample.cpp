//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun May 18 17:46:47 GMT-0800 1997
// Last Modified: Sun May 18 17:46:50 GMT-0800 1997
// Filename:      ...sig/code/Filter/Downsample/Downsample.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "Downsample.h"
#include <stdlib.h>


//////////////////////////////
//
// Downsample::Downsample
//

Downsample::Downsample(int aPeriod) {
   setName("Downsample");
   brandname = GENERATOR;            // treat as a generator for Action class
   setPeriod(aPeriod);
   trigger();
}



//////////////////////////////
//
// Downsample::~Downsample
//

Downsample::~Downsample() { }



//////////////////////////////
//
// Downsample::action
//

void Downsample::action(void) {
   if (periodCount % period == 0) {
      periodCount = 0;
      inputs.tick(tickNumber++);
   }
   periodCount++;
}



//////////////////////////////
//
// Downsample::getPeriod
//

int Downsample::getPeriod(void) {
   return period;
}



//////////////////////////////
//
// Downsample::output
//

sampleType Downsample::output(int channel) {
   return inputs[channel];
}



//////////////////////////////
//
// Downsample::output
//	default value: counter = -1
//

void Downsample::setPeriod(int aPeriod, int counter) {
   if (aPeriod > 0) {
      period = aPeriod;
   } else {
      cerr << "Error in " << getName() << ". cannot set period <= 0." 
           << " Ignoring value: " << aPeriod << endl;
   }
   if (counter >= 0) {
      trigger(counter);
   }
}



//////////////////////////////
//
// Downsample::tick
//

void Downsample::tick(int newTickNumber) {
   if (tickNumber != newTickNumber) {
      tickNumber = newTickNumber;
      action();
   }
}



//////////////////////////////
//
// Downsample::trigger
// 	default value: counter = 0
//

void Downsample::trigger(int counter) {
   periodCount = counter;
}


// md5sum:	cc118f2fa101588db36641feabb2cc8f  Downsample.cpp
