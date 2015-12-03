//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun May 25 19:11:09 GMT-0800 1997
// Last Modified: Fri May 30 13:23:14 PDT 1997
// Filename:      ...sig/doc/examples/sig/sigfile/pan/pan.cpp
// Syntax:        C++; sig
//
// Description: cross fade a monofile between two stereo channels.
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   if (argc != 4) exitUsage(argv[0]); 

   int method;
   cout << "Enter the panning method: (0 = linear, 1 = constant power): ";
   cin  >> method;

   SoundHeader header(argv[2]);
   header.setChannels(2);


   // Elements:
   SoundFileIn  insound(argv[2]);
   Pan          pan;
   Envelope     panEnv(argv[1], header.getSamples());
   SoundFileOut outsound(argv[3], header);

   // Connections:
   outsound.connect(pan, 0, 0);
   outsound.connect(pan, 1, 1);
   pan.connect(insound);
   pan.connect(panEnv);
   if (method != 0) {
      pan.doPowerPan();
   } else {
      pan.doLinearPan();
   }

   Action action;
   action.tick(outsound, header.getSamples());

   return 0;
}


void exitUsage(const char* command) {
   cout << endl;
   cout << "Takes a mono soundfile and pan envelope and then creates a\n"
        << "stereo file. Pan 0 = all left, pan 1 = all right\n";
   cout << endl;
   cout << "Usage: " << command << " panEnv insound outsound" << endl;
   cout << endl;

   exit(1);
}



