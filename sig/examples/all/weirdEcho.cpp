//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 20 12:34:13 GMT-0800 1997
// Last Modified: Sun Apr 20 12:34:16 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/weirdDelay/weirdDelay.cpp
// Syntax:        C++; sig
//
// Description:   a weird-delay filter.  If no (mono) input, then use whitenoise
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char *command);


int main(int argc, char* argv[]) {
   if (argc == 1) exitUsage(argv[0]);

   SoundHeader header;
   long numSamples = 0;
   long noiseSamples = 0;
   char* infilename = "";
   char* outfilename = NULL;
   float noiseAmp = 0.0;

   if (argc == 2) {            // no input sound
      header.setHighStereo();
      outfilename = argv[1];
      cout << "What amplitude for the noise: [0 .. 1] ";
      cin  >> noiseAmp;
      cout << "How many samples of noise to use: ";
      cin >> noiseSamples;
      numSamples = noiseSamples + 300000;
   } else if (argc == 3) {     // input file specified
      header.setHeader(argv[1]);
      numSamples = header.getSamples() + 300000;
      outfilename = argv[2];
      infilename = argv[1];
   } else {
      exitUsage(argv[0]); 
   }

   header.setChannels(2);

   // Elements:
   SoundFileIn  insound(infilename);
   SoundFileOut outsound(outfilename, header);
   WhiteNoise   noise(noiseAmp);
   Empty        realIn;        


   // Delays
   Delayline   a(17);
   Delayline   b(23);
   Delayline   c(1470);
   Delayline   d(11300);
   Delayline   e(25);
   Delayline   f(700);
   Delayline   g(5000);
   Delayline   h(30000);

   // Summation points
   Add Z;
   Add Y;
   Add X;
   Add W;

   // Normalization for summations
   Scale  gainZ(0.8);
   Scale  gainY(0.7);
   Scale  gainX(0.7);
   Scale  gainW(0.8);

   // Connections:
   outsound.connect(e, 0);           // left channel listening point
   outsound.connect(g, 1);           // right channel listening point
   if (strlen(infilename) == 0) {
      realIn.connect(noise);      
   } else {                       
      realIn.connect(insound);      
   }
  
   a.connect(realIn);
   b.connect(realIn);
   c.connect(realIn);
   d.connect(realIn);
   e.connect(gainZ);
   f.connect(gainX);
   g.connect(gainX);
   h.connect(gainY);
   Z.connect(a);
   Z.connect(gainW);
   Z.connect(f);
   Y.connect(b);
   Y.connect(e);
   X.connect(c);
   X.connect(h);
   W.connect(d);
   W.connect(g);
   gainZ.connect(Z);
   gainY.connect(Y);
   gainX.connect(X);
   gainW.connect(W);

   Action action;

   if (strlen(infilename) == 0) {
      action.tick(outsound, noiseSamples);
      realIn.disconnect(noise);
      action.tick(outsound, numSamples - noiseSamples);
   } else {                       
      action.tick(outsound, numSamples);
   }

 

   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout <<  "Creates an irregular echo pattern. Uses noise if no input\n";
   cout << endl;
   cout << "Usage: " << command << " [insound] outsound" << endl;
   cout << endl;
   cout << "Program input: " << endl;
   cout << "   (a) duration of noise (if no input soundfile)" << endl;
   cout << endl;

   exit(1);
}



