//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Mar 24 23:52:42 GMT-0800 1998
// Last Modified: Wed Mar 25 00:02:42 GMT-0800 1998
// Filename:      ...sig/doc/examples/sig/sigfile/negate/negate.cpp
// Syntax:        C++; sig
//
// Description:   Flips the sign of sound.  Parts that were positive
//                amplitude become negative amplitude and vice-versa.
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char *command);


int main(int argc, char* argv[]) {
   if (argc != 3) exitUsage(argv[0]);

   SoundHeader header(argv[1]);

   // Elements:
   SoundFileIn  insound(argv[1]);
   SoundFileOut outsound(argv[2], header);
   Scale        leftScale(-1);      
   Scale        rightScale(-1);      

   // Connections:
   leftScale.connect(insound);
   rightScale.connect(insound);

   outsound.connect(leftScale);
   if (header.getChannels() == 2) {
      outsound.connect(rightScale, 0, 1);
   }

   Action action;
   action.tick(outsound, header.getSamples());

   return 0;
}
   


void exitUsage(const char* command) {
   cout << endl;
   cout << "Inverts the input soundfile." << endl;
   cout << endl;
   cout << "Usage: " << command << " insound outsound" << endl;
   cout << endl;

   exit(1);
}



