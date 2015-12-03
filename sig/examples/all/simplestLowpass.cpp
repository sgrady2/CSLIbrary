//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 27 12:17:57 GMT-0800 1997
// Last Modified: Sun May 25 00:06:31 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/.../simplestLowpass.cpp
// Syntax:        C++; sig
//
// Description:   Adds the current sample and the previous sample together
//                to create a slow-attenuation lowpass filter.
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   if (argc == 1) exitUsage(argv[0]);

   SoundHeader header(argv[1]);
   char* infilename = "";
   char* outfilename = NULL;
   float noiseAmp = 0.0;
   long numSamples = 0;

   if (argc == 2) {            // no input sound
      header.setHighMono();
      outfilename = argv[1];
      cout << "What amplitude for the noise: ";
      cin >> noiseAmp;
      cout << "How many seconds of noise to use: [0 .. 1] ";
      float temp;
      cin >> temp;
      numSamples = (long)(temp * 44100);
   } else if (argc == 3) {     // input file specified
      header.setHeader(argv[1]);
      numSamples = header.getSamples() + 2;
      outfilename = argv[2];
      infilename = argv[1];
   } else {
      exitUsage(argv[0]);
   }


   // Elements:
   SoundFileIn  insound(infilename);
   SoundFileOut outsound(outfilename, header);
   WhiteNoise   noise(noiseAmp);
   Add          add;
   Scale        scale(0.5);            // scale by 50%
   Delayline    delay(2);              // 1 sample delay 
   delay.setDelay(1);

   // Connections:
   outsound.connect(scale);
   if (strlen(infilename) == 0) {         // use noise
      delay.connect(noise);
      add.connect(noise);
   } else {                               // use input file
      delay.connect(insound);
      add.connect(insound);
   }
   add.connect(delay);
   scale.connect(add);

   Action action;
   action.tick(outsound, header.getSamples() + 1);


   return 0;
}
   


void exitUsage(const char* command) {
   cout << "Averages adjacent samples together to give a low-quality lowpass "
        << endl;
   cout << "filter.  If no input sound, then use whitenoise. " << endl;
   cout << endl;
   cout << "Usage: " << command << " [insound] outsound" << endl;
   cout << endl;
   cout << "Program input: " << endl;
   cout << "   (a) amplitude of input noise (if no input sound)" << endl;
   cout << "   (b) duration of noise in samples (if no input sound)" << endl;
   cout << endl;

   exit(1);
}



