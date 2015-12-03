//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 27 11:19:52 GMT-0800 1997
// Last Modified: Sun Apr 27 11:19:55 GMT-0800 1997
// Filename:      ...sig/code/Filter/Delayline/Delayline.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "Delayline.h"
#include <iostream.h>
#include <stdlib.h>


//////////////////////////////
//
// Delayline::Delayline
//

Delayline::Delayline(int delay) : Delay(delay) { 
   delayAmount = delay;
   setName("Delayline");
   brandname = FILTER;
}



//////////////////////////////
//
// Delayline::~Delayline
//

Delayline::~Delayline() { }



//////////////////////////////
//
// Delayline::action
//


void Delayline::action(void) {
   write(inputs[0]);
}



//////////////////////////////
//
// Delayline::getDelay
//

ulong Delayline::getDelay(void) {
   return delayAmount;
}



//////////////////////////////
//
// Delayline::output
//    default value: int index = 0
//

sampleType Delayline::output(int index) {
   return read((long)(-delayAmount+index));
}

sampleType Delayline::output(float index) {
   return read(index);
}



//////////////////////////////
//
// Delayline::setDelay
//

void Delayline::setDelay(long delayamount) { 
   if (delayamount < 1) {
      cerr << "Error: Cannot have a negative (zero) delay: "
           << delayamount << " in a Delayline object." << endl;
      exit(1);
   }

   delayAmount = delayamount;
   if ((ulong)delayAmount > getSize()) {
      setSize(delayAmount);
   }
}



// md5sum:	968f0e2793616fdc829d7c91ee9fa936  Delayline.cpp
