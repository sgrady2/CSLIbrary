//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 27 19:11:17 GMT-0800 1997
// Last Modified: Sat May 24 23:26:23 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/subtract/subtract.cpp
// Syntax:        C++; sig
//
// Description:   subtract two soundfiles from each other

#include "sig.h"
#include <iostream.h>

void exitUsage(const char *command);


int main(int argc, char* argv[]) {
   if (argc != 4) exitUsage(argv[0]);

   SoundHeader header(argv[1]);

   // Elements:
   SoundFileIn  insound1(argv[1]);
   SoundFileIn  insound2(argv[2]);
   SoundFileOut outsound(argv[3], header);
   Add          add;
   Scale        scale(-1);      

   // Connections:
   outsound.connect(add);
   add.connect(insound1);
   add.connect(scale);
   scale.connect(insound2);

   Action action;
   action.tick(outsound, header.getSamples());

   return 0;
}
   


void exitUsage(const char* command) {
   cout << endl;
   cout << 
      "Subtracts input2 from input1.  For example, if you subtract two" << endl;
   cout << "identical soundfiles, then the output will be silence." << endl;
   cout << endl;
   cout << "Usage: " << command << " insound1 insound2 outsound" << endl;
   cout << endl;

   exit(1);
}



