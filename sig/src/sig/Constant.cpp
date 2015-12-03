//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Apr 28 21:46:28 GMT-0800 1997
// Last Modified: Mon Apr 28 21:46:28 GMT-0800 1997
// Filename:      ...sig/code/Generator/Constant/Constant.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#include <iostream.h>
#include "Constant.h"


//////////////////////////////
//
// Constant::Constant
//	default value: aValue = 0
//

Constant::Constant(sampleType aValue) {
   setName("Constant");
   value = aValue;
   brandname = GENERATOR;
}



//////////////////////////////
//
// Constant::~Constant
//

Constant::~Constant() { }



//////////////////////////////
//
// Constant::action
//

void Constant::action(void) { }



//////////////////////////////
//
// Constant::output
//

sampleType Constant::output(int channel) { 
   return value;
}



//////////////////////////////
//
// Constant::printState
//

void Constant::printState(void) { 
   cerr << "Constant value = " << value << endl;
}


//////////////////////////////
//
// Constant::setValue
//

void Constant::setValue(sampleType aValue) { 
   value = aValue;
}


// md5sum:	ea860c1421a48501bbce17d820b65f8f  Constant.cpp
