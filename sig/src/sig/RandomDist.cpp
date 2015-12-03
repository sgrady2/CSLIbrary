//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Jun 11 22:18:18 GMT-0800 1997
// Last Modified: Wed Jun 11 22:18:21 GMT-0800 1997
// Filename:      ...sig/code/Filter/RandomDist/RandomDist.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "RandomDist.h"
#include <stdlib.h>


//////////////////////////////
//
// RandomDist::RandomDist
//

RandomDist::RandomDist(void) : Distribution() {
   setName("RandomDist");
   outputValue = 0;
   brandname = FILTER;
   doUniform(-1.0, 1.0);
}



//////////////////////////////
//
// RandomDist::~RandomDist
//

RandomDist::~RandomDist() { }



//////////////////////////////
//
// RandomDist::action
//

void RandomDist::action(void) {
   outputValue = value();
}



//////////////////////////////
//
// RandomDist::output
//

sampleType RandomDist::output(int channel) {
   return outputValue;
}


// md5sum:	39d042954343c238fa49a286824c94b8  RandomDist.cpp
