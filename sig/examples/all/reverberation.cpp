//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Jun 12 17:12:36 GMT-0800 1997
// Last Modified: Thu Jun 12 17:12:41 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/.../reverberation.cpp
// Syntax:        C++; sig
//
// Description:
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {

   if (argc != 3) {
      cerr << "Usage: " << argv[0] << " input.snd output.snd\n";
      exitUsage(argv[0]); 
   }

   SoundHeader header;
   header.setHeader(argv[1]);

   double revtime;
   cout << "What reverb time: ";
   cin  >> revtime;
   long numSamples = (long)(header.getSamples() + revtime * header.getSrate());

   int revtype;
   cout << "What reverb type: (1 = ReverbA) : ";
   cin  >> revtype;
 
   double revamount;
   cout << "What reverb amount: [0..1]: ";
   cin  >> revamount;


   // Elements:
   SoundFileIn  insound(argv[1]);
   SoundFileOut outsound(argv[2], header);
   Reverberator reverb;

   // Connections:
   outsound.connect(reverb);
   reverb.connect(insound);
   reverb.connect(insound);
   reverb.connect(insound);
   reverb.setReverbTime(revtime);
   reverb.setReverbAmount(revamount);
   reverb.setReverbType(revtype);

   Action action;
   action.tick(outsound, numSamples);

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



