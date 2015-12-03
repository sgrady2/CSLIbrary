//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Jul 26 16:39:36 GMT-0800 1997
// Last Modified: Sat Jul 26 16:39:39 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/disperse/disperse.cpp
// Syntax:        C++; sig
//
// Description:   Disperses frequencies with a cascade of Allpass1 filters
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
      header.setChannels(1);
      numSamples = header.getSamples() + 10;
      outfilename = argv[2];
      infilename = argv[1];
   } else {
      exitUsage(argv[0]); 
   }

   double gain;
   cout << "Enter the gain of the allpass filters [-1 .. 1]: ";
   cin  >> gain;

   int numFilters;
   cout << "Enter the number of first-order allpass filters to use: ";
   cin  >> numFilters;
   if (numFilters <= 0) {
      cerr << "Error: must use a positive number of filters." << endl;
      exitUsage(argv[0]);
   }

   // Elements:
   SoundFileIn  insound(infilename);
   SoundFileOut outsound(outfilename, header);
   WhiteNoise   noise(noiseAmp);
   Allpass1     *filters;
   filters = new Allpass1[numFilters];

   // Connections:
   if (strlen(infilename) == 0) {         // use noise
      filters[0].setInput(noise);      
   } else {                               // use input file
      filters[0].setInput(insound);
   }
   for (int i=1; i<numFilters; i++) {
      filters[i].setInput(filters[i-1]);
      filters[i].setGain(gain);
   }
   outsound.connect(filters[numFilters-1]);
   
   int addSamples;
   cout << "How many samples to add to at end of input: ";
   cin  >> addSamples;
   int disconnectSample = numSamples;
   numSamples += addSamples;

   for (int j=0; j<numSamples; j++) {
      Tick(outsound);
      if (disconnectSample == j) {
         filters[0].disconnect(noise);
      }
   }

 
   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << 
      "A cascade of first-order allpass filters.  Uses whitenoise if no" <<endl;
   cout << "input soundfile is specified." << endl;
   cout << endl;
   cout << "Usage: " << command << " [insound] outsound" << endl;
   cout << endl;
   cout << "Program input: " << endl;
   cout << "   (a) amplitude of input noise (if no input sound)" << endl;
   cout << "   (b) duration of noise in samples (if no input sound)" << endl;
   cout << "   (c) gain of the first-order allpass filters" << endl;
   cout << "   (d) number of filters to use" << endl;
   cout << "   (e) samples to add after input for ringing" << endl;
   cout << endl;

   exit(1);
}



