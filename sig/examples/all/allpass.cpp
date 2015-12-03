//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 20 21:29:16 GMT-0800 1997
// Last Modified: Sun Apr 20 21:29:19 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/allpass/allpass.cpp
// Syntax:        C++; sig
//
// Description:   an allpass filter.  If no input, then use whitenoise
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

   if (argc == 2) {            // no input sound
      header.setHighMono();
      outfilename = argv[1];
      cout << "What amplitude for the noise: [0 .. 1] ";
      cin  >> noiseAmp;
      cout << "How many seconds of noise to use: ";
      float temp;
      cin >> temp;
      numSamples = (long)(temp * 44100);
   } else if (argc == 3) {     // input file specified
      header.setHeader(argv[1]);
      numSamples = header.getSamples() + 10;
      outfilename = argv[2];
      infilename = argv[1];
   } else {
      exitUsage(argv[0]); 
   }

   // Elements:
   SoundFileIn  insound(infilename);
   SoundFileOut outsound(outfilename, header);
   Allpass      allpass;
   WhiteNoise   noise(noiseAmp);

   // Connections:
   outsound.connect(allpass);
   if (strlen(infilename) == 0) {         // use noise
      allpass.setInput(noise);      
   } else {                               // use input file
      allpass.setInput(insound);
   }

   
   double delay;
   cout << "What delay to use for the allpass filter: ";
   cin  >> delay;
   allpass.setDelay(delay);

   double scale;
   cout << "What scale factor for [0..1]: ";
   cin  >> scale;
   allpass.setGain(scale);

   numSamples += 44100;
   Action action;
   action.tick(outsound, numSamples);

 
   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << "An N-th order allpass filter.  "
        << "Uses whitenoise if no input sound." << endl;
   cout << endl;
   cout << "Usage: " << command << " [insound] outsound" << endl;
   cout << endl;
   cout << "Program input: " << endl;
   cout << "   (a) amplitude of input noise (if no input sound)" << endl;
   cout << "   (b) duration of noise in samples (if no input sound)" << endl;
   cout << "   (c) fundamental frequency of allpass filter" << endl;
   cout << "   (d) scale factor for feedback [0 .. 1]" << endl;
   cout << endl;

   exit(1);
}



