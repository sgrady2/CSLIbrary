//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon May 26 18:59:05 GMT-0800 1997
// Last Modified: Sat Jun 21 19:56:01 GMT-0800 1997
// Filename:      ...sig/code/Filter/Pan/Pan.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "Pan.h"
#include <stdlib.h>
#include <math.h>


//////////////////////////////
//
// Pan::Pan
//	default value: aPanType = CONSTANT_POWER_PAN
//

Pan::Pan(int aPanType) {
   setName("Pan");
   brandname = FILTER;
   setPanType(aPanType);
   clear();
}



//////////////////////////////
//
// Pan::~Pan
//

Pan::~Pan() { }



//////////////////////////////
//
// Pan::action
//

void Pan::action(void) {
   if (getPanType() == CONSTANT_POWER_PAN) {
      static sampleType weight;
      weight = sin(HALFPI * getPan());
      outputValueLeft  = getInput() * (1 - weight);
      outputValueRight = getInput() * weight;
   } else if (getPanType() == LINEAR_PAN) {
      outputValueLeft  = getInput() * (1 - getPan());
      outputValueRight = getInput() * getPan();
   } else {
      cerr << "Error: unknown pan type: " << getPanType() << endl;
      exit(1);
   }
}



//////////////////////////////
//
// Pan::clear
//

void Pan::clear(void) {
   outputValueLeft  = 0;
   outputValueRight = 0;
}


//////////////////////////////
//
// Pan::doLinearPan
//

void Pan::doLinearPan(void) {
   panType = LINEAR_PAN;
}



//////////////////////////////
//
// Pan::doPowerPan
//

void Pan::doPowerPan(void) {
   panType = CONSTANT_POWER_PAN;
}



//////////////////////////////
//
// Pan::getInput
//

sampleType Pan::getInput(void) {
   return inputs[0];
}



//////////////////////////////
//
// Pan::getPan
//

sampleType Pan::getPan(void) {
   return inputs[1];
}



//////////////////////////////
//
// Pan::getPanType
//

int Pan::getPanType(void) {
   return panType;
}



//////////////////////////////
//
// Pan::output
//

sampleType Pan::output(int channel) {
   if (channel < 1) {
      return outputValueLeft;
   } else {
      return outputValueRight;
   }
}



//////////////////////////////
//
// Pan::printState
//

void Pan::printState(void) {
   cout << "     Input Signal: " << getInput() << endl;
   cout << "     Pan Location: " << getPan()   << endl;
}



//////////////////////////////
//
// Pan::setInput
// 	default value: outputChannel = 0
//

void Pan::setInput(sampleType aValue) {
   connect(aValue, 0);
}

void Pan::setInput(Signal& aSignal, int outputChannel) {
   connect(aSignal, 0, outputChannel);
}

void Pan::setInput(Signal* aSignal, int outputChannel) {
   connect(aSignal, 0, outputChannel);
}



//////////////////////////////
//
// Pan::setPan
//	default value: outputChannel = 0
//

void Pan::setPan(sampleType aPanLocation) {
   connect(aPanLocation, 1);
}


void Pan::setPan(Signal& aSignal, int outputChannel) {
   connect(aSignal, 1, outputChannel);
}


void Pan::setPan(Signal* aSignal, int outputChannel) {
   connect(aSignal, 1, outputChannel);
}



//////////////////////////////
//
// Pan::setPanType
//

void Pan::setPanType(int aPanType) {
   switch (aPanType) {
      case LINEAR_PAN:
         doLinearPan();
         break;
      case CONSTANT_POWER_PAN:
         doPowerPan();
         break;
      default:
         cerr << "Error: unknown pan type: " << aPanType << endl;
         exit(1);
   }
}


// md5sum:	a99f92b71508dbc57e3eb28bb9ca744c  Pan.cpp
