//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Nov 19 00:39:03 GMT-0800 1997
// Last Modified: Wed Nov 19 00:39:00 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/hybrid-ring/hybrid-ring.cpp
// Syntax:        C++; sig
//
// a ring modulator with additional mixing of input sound
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   char* infile = NULL;
   char* outfile = NULL;
   char* modenv = NULL;
   char* inampenv = NULL;
   char* freqenv = NULL;

   if (argc == 6) {
      freqenv = argv[1];
      modenv = argv[2];
      inampenv = argv[3];
      infile = argv[4];
      outfile = argv[5];
   } else if (argc == 5) {
      freqenv = argv[1];
      modenv = argv[2];
      inampenv = "(0 0; 1 0)";
      infile = argv[3];
      outfile = argv[4];
   } else if (argc == 4) {
      freqenv = argv[1];
      modenv = "(0 1; 1 1)";
      inampenv = "(0 0; 1 0)";
      infile = argv[2];
      outfile = argv[3];
   } else {
      exitUsage(argv[0]); 
   }

   SoundHeader header(infile);

   // Elements:
   SoundFileIn  insound(infile);
   Envelope     freqEnv(freqenv, header.getSamples());
   Envelope     modEnv(modenv, header.getSamples());
   Envelope     inampEnv(inampenv, header.getSamples());
   Osc          oscillator;
   SoundFileOut outsound(outfile, header);
   Multiply     ringmodLeft, ringmodRight;
   Add          mixLeft, mixRight;
   Multiply     mulLeft, mulRight;

   // Connections:
   outsound.connect(mixLeft);
   if (header.getChannels() == 2) {
      outsound.connect(mixRight);
   }
   mixLeft.connect(ringmodLeft);
   mixLeft.connect(mulLeft);
   mixRight.connect(ringmodRight);
   mixRight.connect(mulRight);
   mulLeft.connect(insound);
   mulLeft.connect(inampEnv);
   mulRight.connect(insound, 0, 1);
   mulRight.connect(inampEnv, 1);
   ringmodLeft.connect(insound);
   ringmodLeft.connect(oscillator);
   ringmodRight.connect(insound, 0, 1);
   ringmodRight.connect(oscillator);
   oscillator.connect(freqEnv, 0);
   oscillator.connect(modEnv, 1);

   Action action;
   action.tick(outsound, header.getSamples());


   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << "A ring modulator with input sound mixing control." << endl;
   cout << "Creates an oscillator which is multiplied with the input sound." 
        << endl;
   cout << endl;
   cout << "Usage: " << command 
        << " freqEnv [modEnv [inampenv]] insound outsound" 
        << endl;
   cout << endl;
   cout << "   freqEnv  = frequency envelope of modulating oscillator" << endl;
   cout << "   modEnv   = amplitude of modulating oscillator" << endl;
   cout << "              defaults to \"(0 1; 1 1)\" if not specified." << endl;
   cout << "   inampEnv = amplitude of input soundfile mixed into output" 
   << endl;
   cout << "              defaults to \"(0 0; 1 0)\" if not specified." << endl;
   cout << endl;
   cout << "Example envelope strings: pairs of time, freq or amp" << endl;
   cout << "   constant env of 440: \"(0 440; 100 440)\"  " << endl;
   cout << "   triangle env:      \"(0 0; 1 800; 2 0)\" " << endl;
   cout << "   ramp at begin/end: \"(0 0; 1000 100; s; 2000 0)\" " << endl;
   cout << "      with stick points time is given in samples. " << endl;
   cout << endl;

   exit(1);
}



