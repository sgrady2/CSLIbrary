//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Apr 30 21:50:42 GMT-0800 1997
// Last Modified: Wed Apr 30 21:50:45 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/flanger/flanger.cpp
// Syntax:        C++; sig
//
// Description:   a flanger
//

#include "sig.h"
#include <iostream.h>
#include <math.h>

void exitUsage(const char* command);


Delayline *delayptr;

static sampleType flangeFunction(double input) {
   // frequency 0.2 Hz
   return delayptr->output(-200*(1+sin(0.000142475 * input * 0.2 )) - 50);
}


int main(int argc, char* argv[]) {
   if (argc == 1) exitUsage(argv[0]); 
 
   SoundHeader header(argv[1]);
   char* infilename = "";
   char* outfilename;
   float noiseAmp = 0.0;
   long numSamples = 0;

   if (argc == 2) {            // no input sound
      header.setHighMono();
      outfilename = argv[1];
      cout << "What amplitude for the noise: [0 .. 1] ";
      cin >> noiseAmp;
      cout << "How many seconds of noise to use: ";
      float temp;
      cin >> temp;
      numSamples = (long)(temp * 44100);
   } else if (argc == 3) {     // input file specified
      header.setHeader(argv[1]);
      numSamples = header.getSamples() + 5;
      outfilename = argv[2];
      infilename = argv[1];
   } else {
      exitUsage(argv[0]);
   }


   // Elements:
   SoundFileIn  insound(argv[1]);
   SoundFileOut outsound(argv[2], header);
   Function     oscdelay(flangeFunction);
   WhiteNoise   noise(noiseAmp);
   Add          add;
   Scale        scale(0.5);
   Delayline    delay(1000);
   Delayline    delay2(500);
   delayptr = &delay;

   // Connections:
   if (strlen(infilename) == 0) {         // use noise
      delay.connect(noise);
      delay2.connect(noise);
   } else {                               // use input file
      delay.connect(insound);
      delay2.connect(insound);
   }
   add.connect(delay2);
   add.connect(oscdelay);
   scale.connect(add);
   outsound.connect(scale);


   for (int i=0; i<numSamples; i++) {
      Tick(outsound);
      delay.action();
   }

   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << "A flanger. Fixed frequency at 0.2 Hz. Use whitenoise if no" << endl;
   cout << "input sound is specified." << endl;
   cout << endl;
   cout << "Usage: " << command << " [insound] outsound" << endl;
   cout << endl;
   cout << "Program input: " << endl;
   cout << "   (a) amplitude of input noise (if no input sound)" << endl;
   cout << "   (b) duration of noise in samples (if no input sound)" << endl;
   cout << endl;

   exit(1);
}



