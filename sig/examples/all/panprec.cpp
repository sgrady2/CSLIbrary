//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Jun  7 08:17:09 GMT-0800 1997
// Last Modified: Sat Jun  7 08:17:13 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/panprec/panprec.cpp
// Syntax:        C++; sig
//
// Description:   cross fade a monofile between two stereo channels
//

#include "sig.h"
#include <stdlib.h>

void exitUsage(const char *command);


// Elements:
Envelope     panEnv;
Resonator    leftEar(1000);
Resonator    rightEar(1000);
Delayline    leftDelay(30);
Delayline    rightDelay(30);

static sampleType updatingFunction(double input) {
   static sampleType leftValue;
   static sampleType rightValue;
   panEnv.action();
   rightValue = panEnv.output(0);
   leftValue = 1.0 - rightValue;

   leftEar.setBandwidth(leftValue*1000+1000);
   rightEar.setBandwidth(rightValue*1000+1000);

   leftDelay.setDelay((int)(leftValue*30));
   rightDelay.setDelay((int)(rightValue*30));

   return 0.0;
}


int main(int argc, char* argv[]) {

   if (argc != 4) {
      cerr << "Usage: " << argv[0] 
           << " ChannelPositionString input.snd output.snd\n";
      exitUsage(argv[0]); 
   }


   SoundHeader header(argv[2]);
   header.setChannels(2);

   SoundFileIn  insound(argv[2]);
   SoundFileOut outsound(argv[3], header);
   Function     processEnvelope(updatingFunction);

   panEnv.setEnvelope(argv[1]);
   panEnv.setDurationSamples(header.getSamples());

   // Connections:
   processEnvelope.setFunction(updatingFunction);
   leftDelay.connect(leftEar);
   rightDelay.connect(rightEar);
   leftEar.connect(insound);
   rightEar.connect(insound);
   outsound.connect(leftDelay);
   outsound.connect(rightDelay);
   
   for (int i=0; i<header.getSamples(); i++) {
      Tick(processEnvelope);
      Tick(outsound);
   }

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



