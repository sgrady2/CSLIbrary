//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue May 13 22:51:42 GMT-0800 1997
// Last Modified: Tue May 13 22:51:45 GMT-0800 1997
// Filename:      ...sig/code/Filter/OnePole/OnePole.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "OnePole.h"
#include <stdlib.h>


//////////////////////////////
//
// OnePole::OnePole
//	default values: aFeedback = 0, aGain = 1
//

OnePole::OnePole(sampleType aFeedback, sampleType aGain) {
   setFeedback(aFeedback);
   setGain(aGain);
   setName("OnePole");
   outputValue = 0;
   previousOutput = 0;
   brandname = FILTER;
}



//////////////////////////////
//
// OnePole::action
//

OnePole::~OnePole() { }



//////////////////////////////
//
// OnePole::action
//

void OnePole::action(void) {
   previousOutput = outputValue;

   if (inputs.connectionQ(1)) {             // feedback connected to input 1
      if (inputs.connectionQ(2)) {          // gain connected to input 2
         outputValue = inputs[2] * inputs[0] - inputs[1] * previousOutput;
      } else {                              // gain not connected to input 2
         outputValue = gain * inputs[0] - inputs[1] * previousOutput;
      }
   } else {                                 // feedback not connected to input 1
      if (inputs.connectionQ(2)) {          // gain connected to input 2
         outputValue = inputs[2] * inputs[0] - feedback * previousOutput;
      } else {                              // gain not connected to input 2
         outputValue = gain * inputs[0] - feedback * previousOutput;
      }
   }
}



//////////////////////////////
//
// OnePole::getFeedback
//

sampleType OnePole::getFeedback(void) {
   return feedback;
}



//////////////////////////////
//
// OnePole::getGain
//

sampleType OnePole::getGain(void) {
   return gain;
}



//////////////////////////////
//
// OnePole::output
//

sampleType OnePole::output(int channel) {
   return outputValue;
}



//////////////////////////////
//
// OnePole::setFeedback
//

void OnePole::setFeedback(sampleType aFeedback) {
   feedback = aFeedback;
}



//////////////////////////////
//
// OnePole::setGain
//

void OnePole::setGain(sampleType aGain) {
   gain = aGain;
}






// md5sum:	67c2613471599ea5fb8114b9b2c97aa1  OnePole.cpp
