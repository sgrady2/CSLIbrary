//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Nov  3 20:09:26 PST 1997
// Last Modified: Mon Nov  3 20:09:29 PST 1997
// Filename:      ...sig/code/Filter/ResoundOut/ResoundOut.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#include "ResoundOut.h"
#include "helperFunctions.h"
#include <stdlib.h>


//////////////////////////////
//
// ResoundOut::ResoundOut
//

ResoundOut::ResoundOut(short* soundData, SoundHeader& header) {
   data = soundData;
   setName("ResoundOut");
   sndheader = header;
   sampleCount = 0;
   brandname = FILTER;
}



//////////////////////////////
//
// ResoundOut::~ResoundOut
//


ResoundOut::~ResoundOut() {
   // do nothing
}



//////////////////////////////
//
// ResoundOut::action
//

void ResoundOut::action(void) {
   static int i;

   if (sampleCount / sndheader.getChannels() >= sndheader.getSamples()-1) {
      // trying to go beyond allocated space for sound
      cerr << "Error: Invalid array location in output sound." << endl;
      exit(1);
   }
 
   for (i=0; i<sndheader.getChannels(); i++) {
      #ifdef OTHEREND
         ushort lowbyte;
         ushort hibyte;
         data[sampleCount] = SampleToShort(inputs[i]);
         lowbyte = 0x0f & data[sampleCount];
         hibyte = 0x0f & (data[sampleCount] >> 8);
         data[sampleCount] = (lowbyte << 8) + hibyte;
      #else
         data[sampleCount] = SampleToShort(inputs[i]);
      #endif
   }
   sampleCount++;
}



//////////////////////////////
//
// ResoundOut::clear
//

void ResoundOut::clear(void) {
   sampleCount = 0;
}



//////////////////////////////
//
// ResoundOut::output
//

sampleType ResoundOut::output(int channel) {
   return inputs[channel];
}



// md5sum:	f45763b4b95fd64c7a015739d12a16f2  ResoundOut.cpp
