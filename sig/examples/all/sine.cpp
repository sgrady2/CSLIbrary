//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon May 26 17:49:13 GMT-0800 1997
// Last Modified: Mon May 26 18:08:48 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigaudio/sine/sine.cpp
// Syntax:        C++; sig
// $Smake:        cc -Wall -DLINUX -O -o %b %f -I../include %
//                -L../lib -lg++ -lsig && strip %b
//
// Description:   play a sinewave live on a LINUX computer with the 
//		  Open Sound System
//


#include "sig.h"


int main(void) {
   Action action;

   // Elements:
   LinuxSoundOut  outsound(44100, 2, 0);     // srate, channels, port
   Envelope       ampEnv("(0 0; 1 0.9; s; 20000 0)");
   Osc            sine;
   Constant       freq;

   // Connections:
   outsound.connect(sine);         // left channel
   outsound.connect(sine);         // right channel
   sine.connect(freq);
   sine.connect(ampEnv);

   action.freezeState(outsound);

   double frequency = 1.0;
   double duration;
   long numSamples;

   while (frequency > 0) {
      cout << "What frequency do you want to hear (<= 0 to quit): ";
      cin  >> frequency;
      if (frequency <= 0) break;
      cout << "How long do you want to hear the sound (in seconds): ";
      cin  >> duration;
      numSamples = (long)(duration * 44100.0);

      ampEnv.setDurationSamples(numSamples);
      ampEnv.trigger();
      freq.setValue(frequency);

      action.tickFrozenState(numSamples);
      outsound.flushBuffer();
   }


   return 0;
}



