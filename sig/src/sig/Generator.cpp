//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Nov  1 23:35:02 PST 1996
// Last Modified: Thu Nov 27 21:21:47 GMT-0800 1997
// Filename:      ...sig/code/Generator/Generator.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../include; rm -f %b.o
//

#include "Generator.h"


//////////////////////////////
//
// Genetator::Generator
//

Generator::Generator(void) { 
   setName("Generator");
   brandname = GENERATOR;         // 0 = signal, 1 = generator, 2 = filter
}



//////////////////////////////
//
// Genetator::~Generator
//

Generator::~Generator() { }



//////////////////////////////
//
// Generator::printConnections
//	default value: flag = 0;
// 

void Generator::printConnections(int flag) {
   if (flag == 2 || flag == 3) {
      printState();
   }
}



//////////////////////////////
//
// Generator::tick
// 

void Generator::tick(int newTickNumber) {
   if (tickNumber != newTickNumber) {
      action();
      tickNumber = newTickNumber;
   }
}



// md5sum:	455054920e83a14762840b294bd1278d  Generator.cpp
