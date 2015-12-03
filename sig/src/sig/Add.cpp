//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Nov 17 14:18:31 PST 1996
// Last Modified: Sun Jul 27 10:26:59 GMT-0800 1997
// Filename:      ...sig/code/Filter/Add/Add.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "Add.h"
#include <stdlib.h>


//////////////////////////////
//
// Add::Add
//

Add::Add(void) {
   setName("Add");
   brandname = FILTER;
   clear();
}



//////////////////////////////
//
// Add::~Add
//

Add::~Add() { 
   // do nothing
}



//////////////////////////////
//
// Add::action
//

void Add::action(void) {
   static int i;
   outputValue = 0.0;
   for (i=0; i<inputs.getSize(); i++) {
         outputValue += inputs[i];
   }
}



//////////////////////////////
//
// Add::clear
//

void Add::clear(void) {
   outputValue = 0.0;
}



//////////////////////////////
//
// Add::output
//

sampleType Add::output(int channel) {
   return outputValue;
}



//////////////////////////////
//
// Add::printState
//

void Add::printState(void) {
   for (int i=0; i<inputs.getSize(); i++) {
      cout << "AddInput[" << i << "] = " << inputs[i] << endl;
   }
}




// md5sum:	5f2dedb641cb74a81dd7d412984155fe  Add.cpp
