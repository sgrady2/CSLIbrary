//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Apr 19 20:48:50 GMT-0800 1997
// Last Modified: Sat Apr 19 20:48:53 GMT-0800 1997
// Filename:      ...sig/code/Generator/Function/Function.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#include <iostream.h>
#include "Function.h"

static sampleType aSimpleFunction(double input = 0.0) {
   return input;
}


//////////////////////////////
//
// Function::Function
//

Function::Function(funcPtr aFunction = aSimpleFunction) {
   setName("Function");
   outputValue = 0;
   setFunction(aFunction);
   reset();
   brandname = GENERATOR;
}



//////////////////////////////
//
// Function::~Function
//

Function::~Function() { }



//////////////////////////////
//
// Function::action
//

void Function::action(void) { 
   outputValue = generatingFunction(timeVariable);
   timeVariable += 1.0;
}



//////////////////////////////
//
// Function::output
//

sampleType Function::output(int channel) { 
   return outputValue;
}



//////////////////////////////
//
// Function::printState
//

void Function::printState(void) { }



//////////////////////////////
//
// Function::reset
// 	default variable: newTime = 0;
//

void Function::reset(double newTime) { 
   timeVariable = newTime;
}



//////////////////////////////
//
// Function::setFunction
//

void Function::setFunction(funcPtr aNewFunction) { 
   generatingFunction = aNewFunction;
}



// md5sum:	a880c0b94b3560aaaf24811e29799dfd  Function.cpp
