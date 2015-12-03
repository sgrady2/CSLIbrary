//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon May 19 22:45:43 GMT-0800 1997
// Last Modified: Mon May 19 22:45:47 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/precedence/precedence.cpp
// Syntax:        C++; sig
//
// Description:   for demonstration of the precedence effect.
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   if (argc == 1) exitUsage(argv[0]);

   SoundHeader header;
   char* infilename = "";
   char* infilename2 = "";
   char* outfilename = NULL;
   float impulseAmp = 0.9;
   long numSamples = 0;

   if (argc == 2) {            // no input sound
      header.setHighStereo();
      outfilename = argv[1];
      cout << "What is the duration of the output file in seconds: ";
      float temp;
      cin >> temp;
      numSamples = (long)(temp * 44100);
   } else if (argc == 3) {     // input file specified
      header.setHeader(argv[1]);
      numSamples = header.getSamples();
      outfilename = argv[2];
      infilename = argv[1];
   } else if (argc == 4) {     // two input files specified
      header.setHeader(argv[1]);
      numSamples = header.getSamples();
      outfilename = argv[3];
      infilename = argv[1];
      infilename2 = argv[2];
   } else {
      exitUsage(argv[0]); 
   }

   int sampleDelay;
   cout << "Enter the number of samples to offset the sound by: ";
   cin  >> sampleDelay;
   cout << "That is a delay of " << (double)sampleDelay / 
           header.getSrate() * 1000 << " milliseconds." << endl;
   numSamples += sampleDelay;

   header.setChannels(2);
   // Elements:
   SoundFileIn  insound(infilename);
   SoundFileIn  insound2(infilename2);
   SoundFileOut outsound(outfilename, header);
   Delayline    delay(sampleDelay);
   Impulse      impulse(20000, impulseAmp);

   // Connections:
   if (strlen(infilename2) != 0) {               // use two input files
      outsound.connect(insound, 0);
      outsound.connect(delay, 1);
      delay.connect(insound2, 0);
   } else if (strlen(infilename) == 0) {         // use impulse
      outsound.connect(impulse, 0);      
      outsound.connect(delay, 1);
      delay.connect(impulse);
   } else {                                      // use input file
      outsound.connect(insound, 0);
      outsound.connect(delay, 1);
      delay.connect(insound, 0);
   }

   Action action;
   action.tick(outsound, numSamples);

   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << 
      "Generates precedence effect examples by delaying a sound by the" << endl;
   cout <<
      "specified number of samples. Try 1-30 samples at 44.1 kHz." << endl;
   cout << endl;
   cout << "Usage: " << command << " [insound1] [insound2] outsound"<< endl;
   cout << endl;
   cout << "Program input: " << endl;
   cout << "   (a) duration of the output sound (if no input soundfile)"<< endl;
   cout << "   (b) sample delay between channels" << endl;
   cout << endl;

   exit(1);
}



