//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Nov 19 01:02:28 GMT-0800 1997
// Last Modified: Wed Nov 19 01:02:31 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/am-mod/am-mod.cpp
// Syntax:        C++; sig
//
// Description:   an AM modulator
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   char* freqenv = NULL;
   char* ampenv = NULL;
   char* infile = NULL;
   char* outfile = NULL;

   if (argc == 5) {
      freqenv = argv[1];
      ampenv = argv[2];
      infile = argv[3];
      outfile = argv[4];
   } else if (argc == 4) {
      freqenv = argv[1];
      ampenv = "( 0 1; 1 1)";
      infile = argv[2];
      outfile = argv[3];
   } else {
      exitUsage(argv[0]); 
   }

   SoundHeader header(infile);

   // Elements:
   SoundFileIn  insound(infile);
   Envelope     freqEnv(freqenv, header.getSamples());
   Envelope     ampEnv(ampenv, header.getSamples());
   Osc          oscillator;
   Add          am;
   Constant     zeroHertz(1);
   SoundFileOut outsound(outfile, header);
   Multiply     ringmod;
   Multiply     ringmodRight;

   // Connections:
   outsound.connect(ringmod);
   if (header.getChannels() == 2) {
      outsound.connect(ringmodRight);
   }
   am.connect(zeroHertz);
   am.connect(oscillator);
   ringmod.connect(insound);
   ringmod.connect(am);
   oscillator.connect(freqEnv, 0);
   oscillator.connect(ampEnv, 1);
   ringmodRight.connect(insound, 0, 1);
   ringmodRight.connect(am);

   Action action;
   action.tick(outsound, header.getSamples());


   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << "An AM modulator.  Creates a sinusoid and 0Hz sinusoid which is "
        << endl;
   cout << "multiplied with the input sound.  Keep amp of input sound "
        << "under 50% max vol." << endl;
   cout << endl;
   cout << "Usage: " << command << " freqEnv [ampEnv] insound outsound" 
        << endl;
   cout << endl;
   cout << "   If ampEnv is not specified, then the default is \"(0 1; 1 1)\"."
        << endl << endl;
   cout << "Example envelope strings: pairs of time, freq or amp" << endl;
   cout << "   constant env of 440: \"(0 440; 100 440)\"  " << endl;
   cout << "   triangle env:      \"(0 0; 1 800; 2 0)\" " << endl;
   cout << "   ramp at begin/end: \"(0 0; 1000 100; s; 2000 0)\" " << endl;
   cout << "      with stick points time is given in samples. " << endl;
   cout << endl;

   exit(1);
}



