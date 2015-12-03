//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon May 19 21:51:06 GMT-0800 1997
// Last Modified: Sun May 25 01:54:01 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/.../binauralBeats.cpp
// Syntax:        C++; sig
//
// Description:   make a stereo soundfile with (different) sinwaves in 
//                each channel
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   if (argc != 4) exitUsage(argv[0]); 

   int numSamples;
   float duration;
   cout << "How many seconds should the sound last: ";
   cin  >> duration;
   numSamples = (int)(duration * 44100 + 0.5);
  
   SoundHeader header;
   header.setHighStereo();

   // Elements:
   Osc          oscillator1;
   Osc          oscillator2;
   SoundFileOut outsound(argv[3], header);
   Envelope     ampEnv("(0 0; 2000 0.3; s; 10000 0)", numSamples);
   Envelope     freqEnv1(argv[1], numSamples);
   Envelope     freqEnv2(argv[2], numSamples);


   // Connections:
   oscillator1.connect(freqEnv1, 0);
   oscillator1.connect(ampEnv, 1);
   oscillator2.connect(freqEnv2, 0);
   oscillator2.connect(ampEnv, 1);
   outsound.connect(oscillator1, 0);   // left channel
   outsound.connect(oscillator2, 1);   // right channel

   Action action;
   action.tick(outsound, numSamples);


   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout 
   << "Generates a stereo soundfile with sinewave of different frequencies in\n"
   << "each channel.\n";
   cout << endl;
   cout << "Usage: " << command << " freqLeftEnv freqRightEnv outsound" 
        << endl;
   cout << endl;
   cout << "Program input: " << endl;
   cout << "   (a) duration of the sound" << endl;
   cout << "   (b) amplitude of the sound" << endl;
   cout << endl;

   exit(1);
}



