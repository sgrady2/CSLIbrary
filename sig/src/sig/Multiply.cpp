//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Nov  2 10:25:09 PST 1996
// Last Modified: Wed Apr 23 22:33:52 GMT-0800 1997
// Filename:      .../sig/code/Filter/Multiply/Multiply.cpp
// Syntax:        C++ 
// $Smake:        cc -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "Multiply.h"
#include <stdlib.h>


//////////////////////////////
//
// Multiply::Multiply
//

Multiply::Multiply(void) {
   setName("Multiply");
   Output = 0.0;
   brandname = FILTER;
}



//////////////////////////////
//
// Multiply::action
//

void Multiply::action(void) {
   Output = 1.0;
   for (int i=0; i<inputs.getSize(); i++) {
      Output *= inputs[i];
   }
}



//////////////////////////////
//
// Multiply::output
//

sampleType Multiply::output(int channel) {
   return Output;
}


// md5sum:	04c76681248907ce3a349d261731093f  Multiply.cpp
