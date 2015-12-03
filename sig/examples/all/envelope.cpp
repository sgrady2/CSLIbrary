//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Apr 19 23:52:50 GMT-0800 1997
// Last Modified: Sun May 25 01:46:38 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/envelope/envelope.cpp
// Syntax:        C++; sig
//
// Description:   apply an amplitude envelope to a sound file
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   if (argc != 4) exitUsage(argv[0]); 

   SoundHeader header(argv[2]);

   // Elements:
   SoundFileIn  insound(argv[2]);
   Envelope     envelope(argv[1], header.getSamples());
   SoundFileOut outsound(argv[3], header);
   Multiply     multiply[insound.getChannels()];

   // Connections:
   for (int i=0; i<insound.getChannels(); i++) {
      multiply[i].connect(insound, 0, i);   
      multiply[i].connect(envelope);
      outsound.connect(multiply, i);
   }

//   envelope.printState();

   Action action;
   action.tick(outsound, header.getSamples());


   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << 
      "Applies an amplitude envelope to a soundfile.  Handles an" << endl;
   cout << "arbitrary number of channels." << endl;
   cout << endl;
   cout << "Usage: " << command << " ampString insound outsound" << endl;
   cout << endl;
   cout << "Example amp strings: pairs of time, amp points" << endl;
   cout << "   constant env of 1: \"(0 1; 100 1)\"  " << endl;
   cout << "   triangle env:      \"(0 0; 1 1; 2 0)\" " << endl;
   cout << "   ramp at begin/end: \"(0 0; 1000 1; s; 2000 0)\" " << endl;
   cout << "      with stick points time is given in samples. " << endl;
   cout << endl;

   exit(1);
}



