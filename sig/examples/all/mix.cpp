//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu May  1 22:57:20 GMT-0800 1997
// Last Modified: Sun May 25 01:24:54 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/mix/mix.cpp
// Syntax:        C++; sig
//
// Description:   mixes two soundfiles together
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   if (argc != 4) exitUsage(argv[0]); 

   SoundHeader header(argv[1]);

   float s1, s2, offset;
   long offsetSamples;
   cout << "Enter amplitude scaling of file 1: "; cin  >> s1;
   cout << "Enter amplitude scaling of file 2: "; cin  >> s2;
   cout << "Enter the time in seconds of offset of 2nd file: "; cin  >> offset;
   offsetSamples = (long)(offset * header.getSrate() + 0.5);

   // Elements:
   SoundFileIn  insound1(argv[1]);
   SoundFileIn  insound2(argv[2]);
   if (insound2.getChannels() == 2) {
      header.setChannels(2);
   }
   SoundFileOut outsound(argv[3], header);
   Delayline    leftDelay(offsetSamples);
   Delayline    rightDelay;
   Add          addLeft;
   Add          addRight;
   Scale        leftScale1(s1);
   Scale        rightScale1(s1);
   Scale        leftScale2(s2);
   Scale        rightScale2(s2);


   // Connections:
   leftScale1.connect(insound1);
   leftScale2.connect(insound2);
   leftDelay.connect(leftScale2);
   addLeft.connect(leftScale1);
   addLeft.connect(leftDelay);
   outsound.connect(addLeft);

   if (header.getChannels() == 2) {
      rightDelay.setDelay(offsetSamples);
      rightScale1.connect(insound1, 0, 1);
      rightScale2.connect(insound2, 0, 1);
      rightDelay.connect(rightScale2);
      addRight.connect(rightDelay);
      addRight.connect(rightScale1);
      outsound.connect(addRight);
   }

   long samples1, samples2; 
   samples1 = insound1.getSamples();
   samples2 = insound2.getSamples() + offsetSamples;

   int numSamples = samples1 > samples2 ? samples1 : samples2;

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










