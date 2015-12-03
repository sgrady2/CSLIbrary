//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue May 13 23:58:24 GMT-0800 1997
// Last Modified: Sun May 25 01:21:24 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/onepole/onepole.cpp
// Syntax:        C++; sig
//
// Description:   a one-pole filter.  If no input, then use whitenoise
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   SoundHeader header;
   long numSamples = 0;
   char* infilename = "";
   char* outfilename = NULL;
   float noiseAmp = 0.0;

   if (argc == 3) {            // no input sound
      header.setHighMono();
      outfilename = argv[2];
      cout << "What amplitude for the noise: [0 .. 1] ";
      cin  >> noiseAmp;
      cout << "How many seconds of noise to use: ";
      float temp;
      cin >> temp;
      numSamples = (long)(temp * 44100);
   } else if (argc == 4) {     // input file specified
      header.setHeader(argv[2]);
      numSamples = header.getSamples() + 10;
      outfilename = argv[3];
      infilename = argv[2];
   } else {
      exitUsage(argv[0]); 
   }

   // Elements:
   SoundFileIn  insound(infilename);
   SoundFileOut outsound(outfilename, header);
   OnePole      onepole;
   WhiteNoise   noise(noiseAmp);
   Envelope     feedbackEnv(argv[1], numSamples);

   // Connections:
   outsound.connect(onepole);
   if (strlen(infilename) == 0) {         // use noise
      onepole.connect(noise, 0);      
   } else {                               // use input file
      onepole.connect(insound, 0);
   }
   onepole.connect(feedbackEnv, 1);

   Action action;
   action.tick(outsound, numSamples);

 
   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << "A onepole filter.  Uses white noise If no input sound." << endl;
   cout << "feedback should be in the range (-1 .. 1)." << endl;
   cout << endl;
   cout << "Usage: " << command << " feedbackEnv [insound] outsound"<< endl;
   cout << endl;
   cout << "Program input: " << endl;
   cout << "   (a) amplitude of input noise (if no input sound)" << endl;
   cout << "   (b) duration of noise in samples (if no input sound)" << endl;
   cout << endl;
   cout << "Example filter strings: pairs of time, filter points" << endl;
   cout << "   constant env of 0.5: \"(0 0.5; 100 0.5)\"  " << endl;
   cout << "   triangle env:        \"(0 0; 1 -0.5; 2 0)\" " << endl;
   cout << endl;

   exit(1);
}



