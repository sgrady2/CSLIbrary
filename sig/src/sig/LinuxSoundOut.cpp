//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu May  8 01:14:05 GMT-0800 1997
// Last Modified: Thu May  8 01:14:09 GMT-0800 1997
// References:    Basic Sound I/O code from stilti@ccrma.stanford.edu
// Filename:      ...sig/src/Filter/LinuxSoundOut/LinuxSoundOut.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "LinuxSoundOut.h"
#include "helperFunctions.h"


//////////////////////////////
//
// LinuxSoundOut::LinuxSoundOut
//

LinuxSoundOut::LinuxSoundOut(MY_FLOAT srate, int channels, int port) {
   setName("LinuxSoundOut");

   bufferSize = 512; 
   buffer = new short[bufferSize];
   bufferIndex = 0;

   truesound = new RTSoundIO(srate, channels);

   brandname = FILTER;
}



//////////////////////////////
//
// LinuxSoundOut::~LinuxSoundOut
//

LinuxSoundOut::~LinuxSoundOut() {
   zeroSend();
   if (truesound != NULL) {
      delete truesound;
   }
   truesound = NULL;
   if (buffer != NULL) {
      delete [] buffer;
   }
   buffer = NULL;
}



//////////////////////////////
//
// LinuxSoundOut::action
//

void LinuxSoundOut::action(void) { 
   buffer[bufferIndex++] = SampleToShort(inputs[0]);
   buffer[bufferIndex++] = SampleToShort(inputs[1]);
   if (bufferIndex >= bufferSize) {
      sendBuffer();
      bufferIndex = 0;
   }
}



//////////////////////////////
//
// LinuxSoundOut::flushBuffer
//

void LinuxSoundOut::flushBuffer(void) {
   zeroSend();
}



//////////////////////////////
//
// LinuxSoundOut::output
//

sampleType LinuxSoundOut::output(int channel) {
   return inputs[channel];
}


//////////////////////////////////////////////////////////////////////////////
//
// Private functions:
//


//////////////////////////////
//
// LinuxSoundOut::sendBuffer
//

void LinuxSoundOut::sendBuffer(void) {
   truesound->playBuffer(buffer, bufferSize);
}



//////////////////////////////
//
// LinuxSoundOut::zeroSend
//

void LinuxSoundOut::zeroSend(void) {
   if (bufferIndex == 0) {
      return;
   }
   while (bufferIndex < bufferSize) {
      buffer[bufferIndex++] = 0;
      buffer[bufferIndex++] = 0;
   } 
   sendBuffer();
   bufferIndex = 0;
}


// md5sum:	cdc56984c494dbaf984563abd4909b19  LinuxSoundOut.cpp
