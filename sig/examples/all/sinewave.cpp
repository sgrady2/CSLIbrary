//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Apr 30 15:51:15 GMT-0800 1997
// Last Modified: Sat May 24 23:58:05 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/sinewave/sinewave.cpp
// Syntax:        C++; sig
//
// Description:   make a soundfile with the specified sinewave frequency/cycles
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   if (argc != 2) exitUsage(argv[0]); 

   float frequency;
   cout << "Enter frequency for the sinewave: ";
   cin  >> frequency;

   float numCycles;
   cout << "How many cycles of the sine wave in output: ";
   cin  >> numCycles;

   float amplitude;
   cout << "What amplitude: ";
   cin  >> amplitude;

   float phase;
   cout << "What phase: ";
   cin  >> phase;

   int numSamples;
   numSamples = (int)(1.0/frequency*numCycles*44100 + 0.5);

   SoundHeader header;
   header.setHighMono();

   // Elements:
   FixedSine     sinewave(frequency, amplitude, phase);
   SoundFileOut  outsound(argv[1], header);

   // Connections:
   outsound.connect(sinewave);

   Action action;
   action.tick(outsound, numSamples);


   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout <<  
      "Creates a fixed-frequency sinusoid for a specified number of cycles" 
        << endl;
   cout << endl;
   cout << "Usage: " << command << " outsound" << endl;
   cout << endl;
   cout << "Program input: " << endl;
   cout << "   (a) frequency in Hz of sinusoid" << endl;
   cout << "   (b) number of cycles in output sound" << endl;
   cout << "   (c) amplitude of sinusoid [0 .. 1]" << endl;
   cout << "   (d) phase of sinusoid at beginning of soundfile" << endl;
   cout << endl;

   exit(1);
}



