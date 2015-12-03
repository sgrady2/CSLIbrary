//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Apr 30 13:01:29 GMT-0800 1997
// Last Modified: Sun May 25 00:09:05 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/silence/silence.cpp
// Syntax:        C++; sig
//
// Description: make a blank soundfile.
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   if (argc != 2) exitUsage(argv[0]); 

   int numSamples;
   cout << "How many samples in the blank soundfile (44.1 kHz sample rate): ";
   cin >> numSamples;

   SoundHeader header;
   header.setHighMono("This is a blank soundfile.");

   // Elements:
   Constant     constant(0);
   SoundFileOut outsound(argv[1], header);

   // Connections:
   outsound.connect(constant);

   Action action;
   action.tick(outsound, numSamples);


   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << "Generates a blank soundfile with specified length." << endl;
   cout << endl;
   cout << "Usage: " << command << " outsound" << endl;
   cout << endl;
   cout << "Program input: " << endl;
   cout << "   (a) number of samples in output sound (44.1 kHz sampling rate)"
        << endl;
   cout << endl;

   exit(1);
}



