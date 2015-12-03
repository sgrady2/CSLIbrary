//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Nov 17 00:41:03 PST 1996
// Last Modified: Wed Apr 23 22:38:37 GMT-0800 1997
// Filename:      ...sig/code/Filter/Scale/Scale.cpp
// Syntax:        C++ 
// $Smake:        cc -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "Scale.h"
#include <stdlib.h>


//////////////////////////////
//
// Scale::Scale
//	default value = 1.0
//

Scale::Scale(sampleType value) {
   setName("Scale");
   scaleValue = value;
   Output.allowGrowth();
   Output.zero();
   brandname = FILTER;
}



//////////////////////////////
//
// Scale::action
//

void Scale::action(void) {
   for (int i=0; i<inputs.getSize(); i++) {
      Output[i] = scaleValue * inputs[i];
   }
}



//////////////////////////////
//
// Scale::getScale
//

sampleType Scale::getScale(void) {
   return scaleValue;
}



//////////////////////////////
//
// Scale::output
//

sampleType Scale::output(int channel) {
   return Output[channel];
}



//////////////////////////////
//
// Scale::setScale
//

void Scale::setScale(sampleType newScale) {
   scaleValue = newScale;
}

// md5sum:	bd15ce7071b6a159b35811fb08b2abcc  Scale.cpp
