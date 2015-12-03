//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue May 27 18:59:27 GMT-0800 1997
// Last Modified: Tue May 27 18:59:31 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/karplus/karplus.cpp
// Syntax:        C++; sig
//
// Description:   example of the Karplus-String Plucked String
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   if (argc != 2) exitUsage(argv[0]); 

   int delayLength;
   cout << "How many samples of delay (44.1 kHz sample rate): ";
   cin  >> delayLength;

   float duration = 0;
   int numSamples;
   cout << "How long for output sound in seconds: ";
   cin  >> duration;
   numSamples = (int)(duration * 44100);

   float amplitude;
   cout << "What amplitude for input noise: [0 .. 1] ";
   cin  >> amplitude;

   SoundHeader header;
   header.setHighMono();

   // Elements:
   WhiteNoise   noise(amplitude);
   SoundFileOut outsound(argv[1], header);
   Delayline    delay(delayLength);
   Switch       noiseGate(delayLength);
   Delayline    unitDelay(2);
   unitDelay.setDelay(1);
   Add          add;
   Add          add2;
   Scale        scale(0.5);

   // Connections:
   outsound.connect(delay);
   delay.connect(add2);
   add2.connect(noiseGate);
   add2.connect(scale);
   scale.connect(add);
   noiseGate.connect(noise);
   add.connect(delay);
   add.connect(unitDelay);
   unitDelay.connect(delay);

   Action action;
   action.tick(outsound, numSamples);

   return 0;
}




void exitUsage(const char* command) {
   cout << endl;
   cout << "Generates a plucked string sound." << endl;
   cout << endl;
   cout << "Usage: " << command << " outsound" << endl;
   cout << endl;
   cout << "Program input: " << endl;
   cout << "   (a) sample length of delayline" << endl;
   cout << "   (b) length of output soundfile in seconds" << endl;
   cout << "   (c) amplitude of input noise [0 .. 1]" << endl;
   cout << endl;

   exit(1);
}



