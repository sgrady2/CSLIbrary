//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Jun  8 10:52:24 GMT-0800 1997
// Last Modified: Sun Jun  8 10:52:27 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/pan2/pan2.cpp
// Syntax:        C++; sig
//
// Description:   cross fade a monofile between two stereo channels
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   if (argc != 5) {
      cerr << "Usage: " << argv[0] 
           << " XpositionString YpositionString input.snd output.snd\n";
      exitUsage(argv[0]); 
   }

   SoundHeader header(argv[3]);
   header.setChannels(2);

   // Elements:
   SoundFileIn  insound(argv[3]);
   Pan2         pan;
   Envelope     xEnvelope(argv[1], header.getSamples());
   Envelope     yEnvelope(argv[2], header.getSamples());
   SoundFileOut outsound(argv[4], header);


   // Connections:
   outsound.connect(pan, 0, 0);
   outsound.connect(pan, 1, 1);
   pan.connect(insound);
   pan.connect(xEnvelope);
   pan.connect(yEnvelope);

   Action action;
   action.tick(outsound, header.getSamples());


   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << 
      "Mixes two soundfiles together. You can adjust the amplitude" << endl;
   cout << 
      "of each input and the starting time of the second file in" << endl; 
   cout << 
      "the output. If either of the soundfiles is stereo, then the" << endl;
   cout << 
      "the output will be stereo, and the mono channel will become" << endl;
   cout << "the left channel." << endl;
   cout << endl;
   cout << "Usage: " << command << " insound1 insound2 outsound" << endl;
   cout << endl;
   cout << "Program input: " << endl;
   cout << "   (a) amplitude scaling of file 1" << endl;
   cout << "   (b) amplitude scaling of file 2" << endl;
   cout << "   (c) offset time in seconds of second sound file" << endl;
   cout << endl;

   exit(1);
}



