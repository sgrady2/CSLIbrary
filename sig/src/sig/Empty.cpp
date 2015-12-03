//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun May 18 14:06:20 GMT-0800 1997
// Last Modified: Sun May 18 14:06:23 GMT-0800 1997
// Filename:      ...sig/code/Filter/Empty/Empty.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "Empty.h"
#include <stdlib.h>


//////////////////////////////
//
// Empty::Empty
//

Empty::Empty(void) {
   setName("Empty");
   brandname = FILTER;
}



//////////////////////////////
//
// Empty::action
//

void Empty::action(void) { }



//////////////////////////////
//
// Empty::output
//	default value: channel = 0
//

sampleType Empty::output(int channel) {
   return inputs[channel];
}



// md5sum:	3a5c420c0c2c2c14478a0207d08260b1  Empty.cpp
