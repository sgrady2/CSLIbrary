//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Jun 11 07:01:51 GMT-0800 1997
// Last Modified: Wed Jun 11 07:01:54 GMT-0800 1997
// Filename:      .../sig/code/Filter/ReverbA/ReverbA.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "ReverbA.h"
#include <stdlib.h>


//////////////////////////////
//
// ReverbA::ReverbA
//	default value: aReverbTime = 1.0
//

ReverbA::ReverbA(sampleType aReverbTime) {
   setName("ReverbA");
   brandname = FILTER;
   setReverbTime(aReverbTime);
   
   comb1.setLoopTime(0.0297);     // values in Dodge&Jerse for Schroeder Reverb
   comb2.setLoopTime(0.0371);
   comb3.setLoopTime(0.0411);
   comb4.setLoopTime(0.0437);

   allpass1.setLoopTime(0.0050);
   allpass2.setLoopTime(0.0017);

   setStoredReverbTime(aReverbTime);

   allpass1.setReverbTime(0.09683);
   allpass2.setReverbTime(0.03292);

   // connections
   comb1.connect(inputSignal);
   comb2.connect(inputSignal);
   comb3.connect(inputSignal);
   comb4.connect(inputSignal);
   add.connect(comb1);
   add.connect(comb2);
   add.connect(comb3);
   add.connect(comb4);
   allpass1.connect(add);
   allpass2.connect(allpass1);

   act.freezeState(allpass2);
}



//////////////////////////////
//
// ReverbA::~ReverbA
//

ReverbA::~ReverbA() { }



//////////////////////////////
//
// ReverbA::action
//

void ReverbA::action(void) {
   inputSignal.setValue(getSignal());
   if (getStoredReverbTime() != getReverbTime()) {
      setStoredReverbTime(getReverbTime());
   }
   act.tickFrozenState();
}



//////////////////////////////
//
// ReverbA::getStoredReverbTime
//

sampleType ReverbA::getStoredReverbTime(void) {
   return reverbTime;
}



//////////////////////////////
//
// ReverbA::output
//

sampleType ReverbA::output(int channel) {
   return allpass2.output(0);
}



//////////////////////////////
//
// ReverbA::setStoredReverb
//

void ReverbA::setStoredReverbTime(sampleType aReverbTime) {
   reverbTime = aReverbTime;
   comb1.setReverbTime(aReverbTime);
   comb2.setReverbTime(aReverbTime);
   comb3.setReverbTime(aReverbTime);
   comb4.setReverbTime(aReverbTime);
}


// md5sum:	4a99b2399196a1d347cd6a254e639a00  ReverbA.cpp
