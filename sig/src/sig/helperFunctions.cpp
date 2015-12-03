//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Nov  1 23:35:02 PST 1996
// Last Modified: Fri Nov  1 23:35:02 PST 1996
// Filename:      ...sig/code/utilities/helperFunctions/helperFunctions.cpp
// Syntax:        C++ 
// $Smake:        g++ -Wall -g -c %f -I../../include && rm %b.o
//

#include "helperFunctions.h"
#include <fstream.h>
#include <string.h>
#include <stdlib.h>


//////////////////////////////
//
// SampleToShort -- converts a number in the range -1.0 to 1.0
//	to a number in the range -32768 to 32767.  If the number
//      is out of the input range then it will be limited to the
//	maximum in the direction (and not wrap around).
//

short SampleToShort(sampleType aSample) {
   static int output;

   output = (int)(aSample * 0x7fff + 0.5);

   // prevent wrap around:
   if (output > 32767) output = 32767;
   else if (output < -32768) output = -32768;

   return (short)output;
}
   


//////////////////////////////
//
// SampleToLong -- converts a number in the range -1.0 to 1.0
//	to a number in the range -2147483647 to 2147483648.  If the 
//      number is out of the input range then it will be limited to 
//	the maximum in that direction (and not wrap around).
//

long SampleToLong(sampleType aSample) {
   static long output;

   // prevent wrap around:
   if (aSample >= 1.0) {
      output = (long)0x80000000;
   } else if (aSample <= -1.0) {
      output = (long)0x7fffffff;
   } else {
      output = (long)(aSample * 0x7fffffff + 0.5);
   }

   return output;
}



//////////////////////////////
//
// SampleToChar -- converts a number in the range -1.0 to 1.0
//	to a number in the range -127 to 128.  If the 
//      number is out of the input range then it will be limited to 
//	the maximum in that direction (and not wrap around).
//

char SampleToChar(sampleType aSample) {
   static char output;

   // prevent wrap around:
   if (aSample >= 1.0) {
      output = (char)0x7f;
   } else if (aSample <= -1.0) {
      output = (char)0x80;
   } else {
      output = (char)(aSample * 0x7f + 0.5);
   }

   return output;
}



//////////////////////////////
//
// Tick
//

void Tick(Signal& sig) {
   static int ticker = 0;
   sig.tick(ticker);
   ticker++;
}



// md5sum:	9e3eba57996267996bd1b66d62c557e6  helperFunctions.cpp
