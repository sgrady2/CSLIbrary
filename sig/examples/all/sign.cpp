//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Mar 25 17:25:01 GMT-0800 1998
// Last Modified: Wed Mar 25 17:25:04 GMT-0800 1998
// Filename:      ...sig/doc/examples/sig/sigfile/sign/sign.cpp
// Syntax:        C++; sig
//
// Description:   extracts the sign of a sound.  If no input soundfile
//		  is given, then use whitenoise.  The result is a one-
//                bit mid-rise quantization of the soundfile.
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   if (argc == 1) exitUsage(argv[0]);

   SoundHeader header;
   char* infilename = "";
   char* outfilename = NULL;
   float noiseAmp = 0.0;
   long numSamples = 0;

   if (argc == 2) {              // only output file specified
      header.setHighMono();
      outfilename = argv[1];
      cout << "What amplitude for the noise: [0 .. 1] ";
      cin >> noiseAmp;
      cout << "How many seconds of noise to use: ";
      float temp;
      cin >> temp;
      numSamples = (long)(temp * 44100);
   } else if (argc == 3) {      // input and output files specified
      infilename = argv[1];
      outfilename = argv[2];
      header.setHeader(infilename);
      numSamples = header.getSamples();
   } else {
      exitUsage(argv[0]); 
   }


   // Elements:
   SoundFileIn  insound(infilename);
   SoundFileOut outsound(outfilename, header);
   Sign         leftSign;
   Sign         rightSign;
   WhiteNoise   noise(noiseAmp);

   // Connections:
   if (strlen(infilename) == 0) {         // use noise
      leftSign.connect(noise, 0);      
      rightSign.connect(noise, 0);      
   } else {                               // use input file
      leftSign.connect(insound, 0);
      rightSign.connect(insound, 0, 1);
   }

   outsound.connect(leftSign, 0);
   if (header.getChannels() == 2) {
      outsound.connect(rightSign, 1);
   }

   Action action;
   action.tick(outsound, numSamples);

   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << "Extracts the sign of a soundfile.  "
        << "Uses whitenoise if no input soundfile." << endl;
   cout << endl;
   cout << "Usage: " << command << " [insound] outsound" << endl;
   cout << endl;

   exit(1);
}


