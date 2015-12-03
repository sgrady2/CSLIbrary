//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Nov  2 23:37:12 PST 1996
// Last Modified: Sat Nov  2 23:37:21 PST 1996
// Filename:      ...sig/code/misc/helperFunctions/helperFunctions.h
// Syntax:        C++ 
// $Smake:        g++ -O -o %b %f -I../../../include && strip %b
//


#ifndef _HELPERFUNCTIONS_H_INCLUDED
#define _HELPERFUNCTIONS_H_INCLUDED

#include "Signal.h"
#include "sigTypes.h"
#include <fstream.h>


//////////////////////////////
//
// SampleToShort -- converts a number in the range -1.0 to 1.0
//	to a number in the range -32768 to 32767.  If the number
//      is out of the input range then it will be limited to the
//	maximum in the direction (and not wrap around).
//

short SampleToShort(sampleType aSample);



//////////////////////////////
//
// SampleToLong -- converts a number in the range -1.0 to 1.0
//      to a number in the range -2147483647 to 2147483648.  If the
//      number is out of the input range then it will be limited to
//      the maximum in that direction (and not wrap around).
//

long SampleToLong(sampleType aSample);

   

//////////////////////////////
//
// SampleToChar -- converts a number in the range -1.0 to 1.0
//      to a number in the range -127 to 128.  If the
//      number is out of the input range then it will be limited to
//      the maximum in that direction (and not wrap around).
//

char SampleToChar(sampleType aSample);



//////////////////////////////
//
// Tick
//

void Tick(Signal& sig);


#endif  _HELPERFUNCTIONS_H_INCLUDED


// md5sum:	f1cf4db36095a7c98988102d0c8ff258  helperFunctions.h
