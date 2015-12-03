//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Mar 25 16:47:34 GMT-0800 1998
// Last Modified: Wed Mar 25 16:52:35 GMT-0800 1998
// Filename:      ...sig/doc/examples/sig/sigfile/offset/offset.cpp
// Syntax:        C++; sig
//
// Description:   adds a DC component to a soundfile.  If no
//		  soundfile is given, then uses whitenoise
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   if (argc == 1) exitUsage(argv[0]);

   SoundHeader header;
   char* infilename = "";
   char* outfilename = NULL;
   char* offsetEnv = NULL;
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
   } else if (argc == 4) {     // poleEnv file specified
      outfilename = argv[3];
      infilename = argv[2];
      offsetEnv = argv[1];
      header.setHeader(infilename);
      numSamples = header.getSamples();
   } else {
      exitUsage(argv[0]); 
   }

   if (offsetEnv == NULL) {
      offsetEnv = "c(0 0.5; 1 0.5)";
   }

   // Elements:
   SoundFileIn  insound(infilename);
   SoundFileOut outsound(outfilename, header);
   Add          addLeft;
   Add          addRight;
   WhiteNoise   noise(noiseAmp);
   Envelope     envelope(offsetEnv, numSamples);

   // Connections:
   if (strlen(infilename) == 0) {         // use noise
      addLeft.connect(noise, 0);      
      addRight.connect(noise, 0);      
   } else {                               // use input file
      addLeft.connect(insound, 0);
      addRight.connect(insound, 0, 1);
   }

   addLeft.connect(envelope, 1);
   addRight.connect(envelope, 1);
       
   outsound.connect(addLeft, 0);
   if (header.getChannels() == 2) {
      outsound.connect(addRight, 1);
   }

   Action action;
   action.tick(outsound, numSamples);

   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << 
      "Adds a DC signal.  Uses whitenoise if no input soundfile." << endl;
   cout << endl;
   cout << "Usage: " << command << " [[offsetEnv] insound] outsound" << endl;
   cout << endl;
   cout << "   offsetEnv = offset position envelope [default is 0.5]" << endl;
   cout << endl;
   cout << "Example offsetEnv string: pairs of time, offset positions" << endl;
   cout << "   \"(0 0.5; 1 0.5)\" " << endl;
   cout << endl;

   exit(1);
}



