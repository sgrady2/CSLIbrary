//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Nov  1 23:35:02 PST 1996
// Last Modified: Mon Apr 28 19:29:55 GMT-0800 1997
// Filename:      ...sig/code/Signal/Signal.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../include && rm -f %b.o
//

#include "Signal.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>


//////////////////////////////
//
// Signal Constructor
//

Signal::Signal(void) { 
   signalName = NULL;
   setName("Signal");
   setSrate(44100);
   tickNumber = INT_MAX; 
   brandname = 0;               // 0 = Signal, 1 = Generator, 2 = Filter
}



//////////////////////////////
//
// Signal::~Signal
//

Signal::~Signal() {
   if (signalName != NULL) {
      delete [] signalName;
   }
}



//////////////////////////////
//
// Signal::getName
//

char const* Signal::getName(void) const {
   return signalName;
}



//////////////////////////////
//
// Signal::getSrate
//

float Signal::getSrate(void) const {
   return srate;
}



//////////////////////////////
//
// Signal::getType
//

int Signal::getType(void) const {
   return brandname;
}


//////////////////////////////
//
// Signal::output
//

sampleType Signal::output(double index) {
   return output((int)index);
}


//////////////////////////////
//
// Signal::setName
//

void Signal::setName(const char* newname) {
   if (signalName != NULL) delete [] signalName;
   signalName = new char[strlen(newname)+1];
   strcpy(signalName, newname);
}



//////////////////////////////
//
// Signal::setSrate
//

void Signal::setSrate(float aSrate) {
   if (aSrate < 1) {
      cerr << "Error: you cannot have the nonpositive sampling rate "
           << aSrate << " in the object " << getName() << "." << endl;
      exit(1);
   }
   srate = aSrate;
}


////////////////////////////////////////////////////////////////////////////////
//
// Private functions
//


// md5sum:	76b56da456f92e8d02df0da90cf1d09b  Signal.cpp
