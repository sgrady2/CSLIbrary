//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Apr 30 11:18:14 GMT-0800 1997
// Last Modified: Sun May 25 19:00:05 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/echos/echos.cpp
// Syntax:        C++; sig
//
// Description:   a soundfile echo program
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   if (argc != 3) exitUsage(argv[0]); 

   SoundHeader header(argv[1]);

   int sampleDelay = 0;
   double timeIn;
   cout << "Enter the time in seconds for echo: " << endl;
   cin  >> timeIn;
   sampleDelay = (int)(timeIn * header.getSrate());

   // Elements:
   SoundFileIn  insound(argv[1]);
   SoundFileOut outsound(argv[2], header);
   Add          add;
   Add          addRight;
   Delayline    delay(sampleDelay);
   Delayline    delayRight(sampleDelay);

   // Connections:
   delay.connect(insound);
   add.connect(delay);
   add.connect(insound);
   delayRight.connect(insound, 0, 1);
   addRight.connect(delayRight);
   addRight.connect(delayRight, 1, 1);
   outsound.connect(add, 0);
   if (header.getChannels() == 2) {
      outsound.connect(addRight, 1);
   }

   int numSamples = header.getSamples() + sampleDelay;

   Action action;
   action.tick(outsound, numSamples);


   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout <<"A program which takes a mono input file (or the left channel of a\n";
   cout << "stereo file) and places that channel in the left channel of the\n";
   cout << "output. The output right channel contains a delayed version of\n";
   cout << "the input. It is delayed by the user specified time in seconds.\n";
   cout << endl;
   cout << "Usage: " << command << " insound outsound" << endl;
   cout << endl;
   cout << "Program input: " << endl;
   cout << "   (a) start time of echos in seconds" << endl;
   cout << endl;

   exit(1);
}



