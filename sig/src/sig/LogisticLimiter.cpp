//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Mar  7 17:35:12 PST 1999
// Last Modified: Sun Mar  7 17:36:29 PST 1999
// Filename:      ...sig/code/Filter/LogisticLimiter/LogisticLimiter.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "LogisticLimiter.h"
#include <stdlib.h>
#include <math.h>


//////////////////////////////
//
// LogisticLimiter::LogisticLimiter
//

LogisticLimiter::LogisticLimiter(void) {
   setName("LogisticLimiter");
   brandname = FILTER;
   clear();
}



//////////////////////////////
//
// LogisticLimiter::~LogisticLimiter
//

LogisticLimiter::~LogisticLimiter() { 
   // do nothing
}



//////////////////////////////
//
// LogisticLimiter::action
//

void LogisticLimiter::action(void) {
   outputValue = 2.0/(1.0 + exp(-inputs[0])) - 1;
}



//////////////////////////////
//
// LogisticLimiter::clear
//

void LogisticLimiter::clear(void) {
   outputValue = 0.0;
}



//////////////////////////////
//
// LogisticLimiter::output
//

sampleType LogisticLimiter::output(int channel) {
   return outputValue;
}



//////////////////////////////
//
// LogisticLimiter::printState
//

void LogisticLimiter::printState(void) {
      cout << "LogisticLimiterInput[0] = " << inputs[0] << endl;
      cout << "LogisticLimiterOutput[0] = " << outputValue << endl;
}




// md5sum:	fc1bf47bc98092577ab1f1287b5a14d6  LogisticLimiter.cpp
