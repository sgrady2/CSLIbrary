//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Jun  7 08:17:09 GMT-0800 1997
// Last Modified: Sat Jun  7 08:17:13 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/precEnv/precEnv.cpp
// Syntax:        C++; sig
//
// Description:   precedence according to input envelope
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   if (argc != 4) exitUsage(argv[0]); 

   SoundHeader header(argv[2]);
   header.setChannels(2);

   Envelope     panEnv(argv[1]);
   SoundFileIn  insound(argv[2]);
   SoundFileOut outsound(argv[3], header);
   Delayline    rightDelay(44100);
   Delayline    leftDelay(44100);

   panEnv.setDurationSamples(header.getSamples());

   // Connections:
   leftDelay.connect(insound);
   rightDelay.connect(insound);
   outsound.connect(leftDelay);
   outsound.connect(rightDelay);
   
   for (unsigned int i=0; i<(unsigned int)header.getSamples(); i++) {
      static sampleType leftValue;
      static sampleType rightValue;
      panEnv.action();
     
      if (panEnv.output(0) < 0) {
         rightValue = -panEnv.output(0) + 2;
         leftValue = 2;
      } else {
         rightValue = 2;
         leftValue = panEnv.output(0) + 2;
      }

      leftDelay.setDelay((int)(leftValue));
      rightDelay.setDelay((int)(rightValue));

      Tick(outsound);
   }


   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout 
   << "You supply a sample delay envelope. Negative values mean that the left\n"
   << "channel is heard before the right channel. Useful to generate motion\n"
   << "between left and right channels when delay is in the range [-30..30].\n";
   cout << endl;
   cout << "Usage: " << command << " delayEnv insound outsound" << endl;
   cout << endl;

   exit(1);
}



