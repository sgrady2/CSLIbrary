//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun May 11 21:59:38 GMT-0800 1997
// Last Modified: Fri May 30 00:34:01 PDT 1997
// Filename:      ...sig/code/Filter/Osc/Osc.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "Osc.h"
#include <stdlib.h>
#include <math.h>

#ifndef TWOPI
   #define TWOPI 6.2831853
#endif

int Osc::initSineTable = 1;
PeriodTable Osc::sineTable;


//////////////////////////////
//
// Osc::Osc
//	default value: anAmplitude = 1.0
//

Osc::Osc(sampleType anAmplitude) {
   setName("Osc");
   brandname = FILTER;
   setAmplitude(anAmplitude);
   outputValue = 0;
   phaseSum = 0;

   tableIncrement = 0.0;
   quality = OSC_MEDIUM_QUALITY;       // linear interpolation
   if (initSineTable) {
      initSineTable = 0;
      sineTable.setSize(1024);
      sineTable.sine();
   }
}



//////////////////////////////
//
// Osc::~Osc
//

Osc::~Osc() { }



//////////////////////////////
//
// Osc::action
//

void Osc::action(void) {
   switch (quality) {
      case OSC_HIGH_QUALITY:
         outputValue = getAmplitude() * sin(phaseSum);
         phaseSum += TWOPI * getFrequency() / getSrate();
         break;
      case OSC_MEDIUM_QUALITY:
      case OSC_LOW_QUALITY:
         outputValue = getAmplitude() * sineTable[tableIncrement];
         tableIncrement += sineTable.getSize() * getFrequency() / getSrate();
         if (tableIncrement > sineTable.getSize()) {
            tableIncrement -= sineTable.getSize();
         }
         break;
      default:
         cerr << "Error: unkonwn Osc quality" << endl;
         exit(1);
   }
}



//////////////////////////////
//
// Osc::doHighQuality
//

void Osc::doHighQuality(void) {
   quality = OSC_HIGH_QUALITY;
}



//////////////////////////////
//
// Osc::doMediumQuality
//

void Osc::doMediumQuality(void) {
   quality = OSC_MEDIUM_QUALITY;
   sineTable.doLinearInterpolation();
}



//////////////////////////////
//
// Osc::doLowQuality
//

void Osc::doLowQuality(void) {
   quality = OSC_LOW_QUALITY;
   sineTable.doConstantInterpolation();
}



//////////////////////////////
//
// Osc::getAmplitude
//

sampleType Osc::getAmplitude(void) {
   return inputs[1];
}



//////////////////////////////
//
// Osc::getFrequency
//

sampleType Osc::getFrequency(void) {
   return inputs[0];
}



//////////////////////////////
//
// Osc::output
//

sampleType Osc::output(int channel) {
   return outputValue;
}



//////////////////////////////
//
// Osc::setAmplitude
//

void Osc::setAmplitude(sampleType anAmplitude) {
   connect(anAmplitude, 1);
}



//////////////////////////////
//
// Osc::setPhase
//

void Osc::setPhase(sampleType aNormalizedPhase) {
   tableIncrement = sineTable.getSize() * aNormalizedPhase;
   phaseSum = aNormalizedPhase * TWOPI;
}
   




// md5sum:	4a1e1131de162eb4eb645eb82c7f0cd9  Osc.cpp
