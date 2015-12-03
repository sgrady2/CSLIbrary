//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun May 25 17:08:06 PDT 1997
// Last Modified: Sun May 25 17:08:13 PDT 1997
// Filename:      ...sig/doc/examples/sig/sigfile/.../flipChannels.cpp
// Syntax:        C++; sig
//
// Description:   flip the left and right channels of a stereo sound file 
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   if (argc != 3) exitUsage(argv[0]); 

   SoundHeader header(argv[1]);
   if (header.getChannels() != 2) {
      cerr << "Can't flip channels in a mono soundfile." << endl;
      exitUsage(argv[0]);
   }

   // Elements:
   SoundFileIn  insound(argv[1]);
   SoundFileOut outsound(argv[2], header);

   // Connections:
   outsound.connect(insound, 0, 1);
   outsound.connect(insound, 1, 0);

   Action action;
   action.tick(outsound, header.getSamples());


   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << "Switches the left and right channels in a stereo file." << endl;
   cout << endl;
   cout << "Usage: " << command << " insound outsound" << endl;
   cout << endl;

   exit(1);
}



