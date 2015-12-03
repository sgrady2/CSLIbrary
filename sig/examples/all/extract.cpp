//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Apr 19 23:33:06 GMT-0800 1997
// Last Modified: Sat Apr 19 23:33:09 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/extract/extract.cpp
// Syntax:        C++; sig
//
// Description:   Extract a channel from a multi channel sound file 
// 		  into a mono file
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   if (argc != 4) exitUsage(argv[0]); 

   SoundHeader header(argv[2]);
   header.setChannels(1);

   // Elements:
   SoundFileIn  insound(argv[2]);
   SoundFileOut outsound(argv[3], header);

   // Connections:
   outsound.connect(insound, 0, argv[1][0] - '0');

   Action action;
   action.tick(outsound, header.getSamples());


   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << 
      "Extracts a channel from a multichannel file into a mono file." << endl;
   cout << 
      "Stereo left=0, stereo right=1." << endl;
   cout << endl;
   cout << "Usage: " << command << " channelNum insound outsound" << endl;
   cout << endl;

   exit(1);
}



