//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Jun  7 08:47:36 GMT-0800 1997
// Last Modified: Sat Jun  7 08:47:39 GMT-0800 1997
// Filename:      ...sig/code/Filter/UnitDelay/UnitDelay.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "UnitDelay.h"
#include <stdlib.h>


//////////////////////////////
//
// UnitDelay::UnitDelay
//

UnitDelay::UnitDelay(void) {
   setName("UnitDelay");
   outputValue = 0;
   currentValue = 0;
   brandname = FILTER;
}



//////////////////////////////
//
// UnitDelay::~UnitDelay
//

UnitDelay::~UnitDelay() { }



//////////////////////////////
//
// UnitDelay::action
//

void UnitDelay::action(void) {
   outputValue = currentValue;
   currentValue = inputs[0];
}



//////////////////////////////
//
// UnitDelay::output
//

sampleType UnitDelay::output(int channel) {
   return outputValue;
}



// md5sum:	127d6c0654f172a848c86761823c5f61  UnitDelay.cpp
