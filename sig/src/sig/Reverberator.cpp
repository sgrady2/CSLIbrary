//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Jun 11 15:12:42 GMT-0800 1997
// Last Modified: Wed Jun 11 20:09:02 GMT-0800 1997
// Filename:      ...sig/code/Filter/Reverberator/Reverberator.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "Reverberator.h"
#include <stdlib.h>
#include <string.h>


//////////////////////////////
//
// Reverberator::Reverberator
//

Reverberator::Reverberator(void) {
   setName("Reverberator");
   reverb = NULL;
   brandname = FILTER;

   multiply.connect(inputSignal, 0);
   multiply.connect(reverbAmount, 1);
   add.connect(inputSignal, 0);

   setReverbType("ReverbA");
}



//////////////////////////////
//
// Reverberator::~Reverberator
//

Reverberator::~Reverberator() { 
   if (reverb != NULL) delete reverb;
}



//////////////////////////////
//
// Reverberator::action
//

void Reverberator::action(void) {
   reverbAmount.setValue(getReverbAmount());
   reverbTime.setValue(getReverbTime());
   inputSignal.setValue(getSignal());
   act.tickFrozenState();
}



//////////////////////////////
//
// Reverberator::getReverbAmount
//

sampleType Reverberator::getReverbAmount(void) {
   return inputs[2];
}



//////////////////////////////
//
// Reverberator::getReverbTime
//

sampleType Reverberator::getReverbTime(void) {
   return inputs[1];
}



//////////////////////////////
//
// Reverberator::getSignal
//

sampleType Reverberator::getSignal(void) {
   return inputs[0];
}



//////////////////////////////
//
// Reverberator::output
//

sampleType Reverberator::output(int channel) {
   return add.output(0);
}



//////////////////////////////
//
// Reverberator::setReverbAmount
//

void Reverberator::setReverbAmount(sampleType aReverbAmount) {
   connect(aReverbAmount, 2);
}



//////////////////////////////
//
// Reverberator::setReverbTime
//

void Reverberator::setReverbTime(sampleType aReverbTime) {
   connect(aReverbTime, 1);
}



//////////////////////////////
//
// Reverberator::setReverbType
//

void Reverberator::setReverbType(const char* name) {
   if (strcmp(name, "ReverbA") == 0) {
      setReverbType(REVERBA);
   } else {
      cerr << "Unknown reverb type: " << name << endl;
      exit(1);
   }
}

void Reverberator::setReverbType(int reverbType) {
   switch (reverbType) {
      case REVERBA:
         if (reverb != NULL) delete reverb;
         reverb = new ReverbA;
         add.connect(*reverb, 1);
         reverb->connect(multiply, 0);
         reverb->connect(reverbTime, 1);
         break;
      default:
         cerr << "Unknown reverb type: " << reverbType << endl;
         exit(1);
   }

   act.freezeState(add);
}


// md5sum:	fee58de6ded6d5b133131940a12f6378  Reverberator.cpp
