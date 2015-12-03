//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Jun 11 19:03:44 GMT-0800 1997
// Last Modified: Wed Jun 11 19:03:47 GMT-0800 1997
// Filename:      .../sig/code/Reverb/Reverb.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../include && rm -f %b.o
//

#include <stdlib.h>
#include "Reverb.h"


//////////////////////////////
//
// Reverb::Reverb
//

Reverb::Reverb(void) {
   setName("Reverb");
   brandname = FILTER;
}



//////////////////////////////
//
// Reverb::~Reverb
//

Reverb::~Reverb() { }



//////////////////////////////
//
// Reverb::getReverbTime
//

sampleType Reverb::getReverbTime(void) {
   return inputs[1];
}



//////////////////////////////
//
// Reverb::getSignal
//

sampleType Reverb::getSignal(void) {
   return inputs[0];
}



//////////////////////////////
//
// Reverb::setReverbTime
//

void Reverb::setReverbTime(sampleType aReverbTime) {
   connect(aReverbTime, 1);
}


// md5sum:	9aa0e106bee461f043621b1f803ec9ae  Reverb.cpp
