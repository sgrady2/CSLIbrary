//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Nov  1 23:35:02 PST 1996
// Last Modified: Wed May 14 00:42:37 GMT-0800 1997
// Filename:      ...sig/code/Filter/Filter.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../include && rm -f %b.o
//

#include <stdlib.h>
#include <iostream.h>

#include "Filter.h"


//////////////////////////////
//
// Filter::Filter
//

Filter::Filter(void) {
   setName("Filter");
   brandname = FILTER;
}



//////////////////////////////
//
// Filter::~Filter
//

Filter::~Filter() { }



//////////////////////////////
//
// Filter::connect
//   default value: slot = NONE, channel = 0
//

void Filter::connect(Signal* in, int slot, int channel) {
   inputs.insert(in, slot, channel);
}

void Filter::connect(Signal& in, int slot, int channel) {
   inputs.insert(in, slot, channel);
}

//   default values: no default values for the following function:
void Filter::connect(sampleType defaultValue, int slot) {
   inputs.setDefault(slot, defaultValue);
}



//////////////////////////////
//
// Filter::disconnect
//

Signal* Filter::disconnect(int slot) {
   return inputs.remove(slot);
}

int Filter::disconnect(Signal& toRemove) {
   return inputs.remove(toRemove);
}

int Filter::disconnect(Signal* toRemove) {
   return inputs.remove(*toRemove);
}


//////////////////////////////
//
// Filter::printConnections
// 	default value: flag = 0
//

void Filter::printConnections(int flag) {
   int i;
   cerr << "Filter: " << getName()
        << ". Number of connections: " << inputs.getSize() << endl;
   if (flag == 2 || flag == 3) {
      cerr << " +++++ " << endl;
      printState();
      cerr << " +++++ " << endl;
   }
   for (i=0; i<inputs.getSize(); i++) {
      cerr << " +++ " << i << " +  ";
      if (inputs.connectionQ(i)) {
         cerr << inputs.signal(i)->getName() << endl;
      } else {
         cerr << "EMPTY" << endl;
      }
   }
   cerr << " +++++" << endl << endl;

   if (flag == 1 || flag == 3) {
      for (i=0; i<inputs.getSize(); i++) { 
         if (inputs.connectionQ(i)) {
            inputs.signal(i)->printConnections(flag);
   }  }  }
}



//////////////////////////////
//
// Filter::tick
//

void Filter::tick(int newTickNumber) {
   if (tickNumber != newTickNumber) {
      inputs.tick(newTickNumber);
      tickNumber = newTickNumber;
      action();
   }
}


// md5sum:	bc5e42fe61094539bb6492ef1288e5ff  Filter.cpp
