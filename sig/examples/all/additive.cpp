//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Jun 12 21:08:27 GMT-0800 1997
// Last Modified: Thu Jun 12 21:08:30 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/additive/additive.cpp
// Syntax:        C++; sig
//
// Description:   makes a soundfile with the specified moveable 
//		  frequency/cycles sinewave
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   if (argc < 4)  exitUsage(argv[0]); 

   float duration;
   cout << "How many seconds does the sound last: ";
   cin  >> duration;

   int numSamples;
   numSamples = (int)(duration * 44100 + 0.5);
  
   SoundHeader header;
   header.setHighMono();

   int numOsc = (argc - 2)/2;

   // Elements:
   Osc          oscillator[numOsc];
   SoundFileOut outsound(argv[argc-1], header);
   Envelope     freqEnv[numOsc];
   Envelope     ampEnv[numOsc];
   Add          add;

   // Connections:
   for (int i=0; i<numOsc; i++) {
      freqEnv[i].setEnvelope(argv[2 * i + 1]);
      ampEnv[i].setEnvelope(argv[2 * i + 2]);
      freqEnv[i].setDurationSamples(numSamples);
      ampEnv[i].setDurationSamples(numSamples);

      oscillator[i].connect(freqEnv[i], 0);
      oscillator[i].connect(ampEnv[i], 1);
      add.connect(oscillator[i]);
   }
   outsound.connect(add);

   Action action;
   action.tick(outsound, numSamples);


   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << 
      "Generates any number of oscillators with freq and amp envelopes."<< endl;
   cout << endl;
   cout << "Usage: " << command << " freqEnv1 ampEnv1 "
        << "[freqEnv2 ampEnv2 ...] outsound" << endl;
   cout << endl;
   cout << "Program input: " << endl;
   cout << "   (a) length of output sound in seconds" << endl;
   cout << endl;
   cout << "Example envelopes: pairs of time, amp points" << endl;
   cout << "   constant env of 1: \"(0 1; 100 1)\"  " << endl;
   cout << "   triangle env:      \"(0 0; 1 1; 2 0)\" " << endl;
   cout << "   ramp at begin/end: \"(0 0; 1000 1; s; 2000 0)\" " << endl;
   cout << "      with stick points time is given in samples. " << endl;
   cout << endl;

   exit(1);
}



