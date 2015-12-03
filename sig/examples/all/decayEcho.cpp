//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon May 26 09:32:04 GMT-0800 1997
// Last Modified: Mon May 26 09:32:07 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/decayEcho/decayEcho.cpp
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

   float scaleFactor;
   cout << "Enter the decay rate factor: ";
   cin  >> scaleFactor;

   int numEchos;
   cout << "Enter the number of echos: ";
   cin  >> numEchos;

   // Elements:
   SoundFileIn  insound(argv[1]);
   SoundFileOut outsound(argv[2], header);
   Add          addLeft;
   Add          addRight;
   Scale        scaleLeft(scaleFactor);
   Scale        scaleRight(scaleFactor);
   Delayline    delayLeft(sampleDelay-1);
   Delayline    delayRight(sampleDelay-1);

   // Connections:
   addLeft.connect(insound);
   addLeft.connect(scaleLeft);
   delayLeft.connect(addLeft);
   addRight.connect(insound, 0, 1);
   addRight.connect(scaleRight);
   delayRight.connect(addRight);
   outsound.connect(delayLeft);
   scaleRight.connect(delayRight);
   scaleLeft.connect(delayLeft);
   if (header.getChannels() == 2) {
      outsound.connect(delayRight);
   }

   int numSamples = header.getSamples() + (numEchos + 1) * sampleDelay;

   Action action;
   action.tick(outsound, numSamples);


   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << "Creates echos which decay at a specified rate." << endl;
   cout << endl;
   cout << "Usage: " << command << " insound outsound" << endl;
   cout << endl;
   cout << "Program input: " << endl;
   cout << "   (a) echo time in seconds" << endl;
   cout << "   (b) decay factor for each echo" << endl;
   cout << "   (c) number of echos to create" << endl;
   cout << endl;

   exit(1);
}



