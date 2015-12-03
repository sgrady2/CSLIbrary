//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Jun 11 08:17:47 GMT-0800 1997
// Last Modified: Wed Jun 11 20:19:08 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/reverba/reverba.cpp
// Syntax:        C++; sig
//
// Description:   4 combfilters and two allpass filters
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {

   if (argc != 3) exitUsage(argv[0]); 

   SoundHeader header;
   header.setHeader(argv[1]);

   double revtime;
   cout << "What reverb time: ";
   cin  >> revtime;
   long numSamples = (long)(header.getSamples() + revtime * header.getSrate());

   // Elements:
   SoundFileIn  insound(argv[1]);
   SoundFileOut outsound(argv[2], header);
   ReverbA      reverba(revtime);

   // Connections:
   outsound.connect(reverba);
   reverba.connect(insound);

   Action action;
   action.tick(outsound, numSamples);

   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << 
      "Four comb filters and two allpass filters for simple reverb." << endl;
   cout << endl;
   cout << "Usage: " << command << " insound outsound" << endl;
   cout << endl;
   cout << "Program input: " << endl;
   cout << "   (a) reverb time in seconds" << endl;
   cout << endl;

   exit(1);
}



