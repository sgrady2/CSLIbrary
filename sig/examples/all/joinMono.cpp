//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed May  7 00:48:18 GMT-0800 1997
// Last Modified: Sun May 25 01:34:18 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/join/join.cpp
// Syntax:        C++; sig
//
// Description:   join two mono files into a stereo file
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   if (argc != 4) exitUsage(argv[0]); 

   SoundHeader header(argv[1]);
   header.setChannels(2);
  
   unsigned long delayAmount;
   cout << "Enter number of samples to delay sound 2 by: ";
   cin  >> delayAmount;

   // Elements:
   SoundFileIn  insound1(argv[1]);
   SoundFileIn  insound2(argv[2]);
   Delayline    delay(delayAmount+1);   // have to avoid a 0 delay for now
   SoundFileOut outsound(argv[3], header);

   // Connections:
   delay.connect(insound2);
   outsound.connect(insound1);
   outsound.connect(insound2);

   int samples1 = insound1.getSamples();
   int samples2 = delayAmount + insound2.getSamples();
   int numSamples = samples1 > samples2 ? samples1 : samples2;

   Action action;
   action.tick(outsound, numSamples);


   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << 
      "Joins two mono files into a single stereo file.  The first input" <<endl;
   cout << 
      "file goes to the left channel, second to right channel." << endl;
   cout << endl;
   cout << "Usage: " << command << " inLeft inRight outsound" << endl;
   cout << endl;
   cout << "Program input: " << endl;
   cout << "   (a) offset delay in seconds of right channel" << endl;
   cout << endl;

   exit(1);
}



