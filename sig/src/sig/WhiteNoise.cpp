//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Apr 30 14:17:37 GMT-0800 1997
// Last Modified: Mon Jan 19 04:21:15 GMT-0800 1998
// Filename:      .../sig/code/Generator/WhiteNoise/WhiteNoise.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#include "WhiteNoise.h"
#include <iostream.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>



//////////////////////////////
//
// WhiteNoise::WhiteNoise
//	default values: aMaxAmplitude = 1.0, randomSeed = 0
//

WhiteNoise::WhiteNoise(sampleType aMaxAmplitude, int randomSeed) {
   setName("WhiteNoise");
   setAmplitude(aMaxAmplitude);
   seed(randomSeed);
   outputValue = 0;
   brandname = GENERATOR;
   action();
}



//////////////////////////////
//
// WhiteNoise::~WhiteNoise
//

WhiteNoise::~WhiteNoise() { }



//////////////////////////////
//
// WhiteNoise::action
//

void WhiteNoise::action(void) { 
   outputValue = amplitude * ((float)rand()/RAND_MAX*2.0 - 1.0);
}



//////////////////////////////
//
// WhiteNoise::getAmplitude
//

sampleType WhiteNoise::getAmplitude(void) {
   return amplitude;
}



//////////////////////////////
//
// WhiteNoise::output
//

sampleType WhiteNoise::output(int channel) { 
   return outputValue;
}



//////////////////////////////
//
// WhiteNoise::printState
//

void WhiteNoise::printState(void) { 
   cerr << "WhiteNoise amplitude = " << amplitude << endl;
}



//////////////////////////////
//
// WhiteNoise::seed
//	default value: aSeed = 0
//

void WhiteNoise::seed(int aSeed) {
   if (aSeed == 0) {
      srand(time(NULL));
   } else {
      srand(aSeed);
   }
}



//////////////////////////////
//
// WhiteNoise::setAmplitude
//

void WhiteNoise::setAmplitude(sampleType anAmplitude) { 
   amplitude = anAmplitude;
}





// md5sum:	7c0f36379e2b23d65a6da09d4357c6ac  WhiteNoise.cpp
