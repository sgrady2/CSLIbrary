//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue May 27 18:59:27 GMT-0800 1997
// Last Modified: Tue May 27 18:59:31 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigaudio/.../karplus-strong.cpp
// Syntax:        C++; sig
//
// Description:   example of the Karplus-Strong Plucked String
//

#include "sig.h"

int main(void) {

      Action action;
      int delayLength;
      float duration = 0;
      int numSamples;
   
      // Elements:
      WhiteNoise    noise;
      LinuxSoundOut outsound(44100, 2, 0);  // srate, channels, port
      Delayline     delay(4);
      Switch        noiseGate(2);
      Delayline     unitDelay(2);
      unitDelay.setDelay(1);
      Add           add;
      Add           add2;
      Scale         scale(0.5);
   
      // Connections:
      outsound.connect(delay);
      outsound.connect(delay);    // stereo
      delay.connect(add2);
      add2.connect(noiseGate);
      add2.connect(scale);
      scale.connect(add);
      noiseGate.connect(noise);
      add.connect(delay);
      add.connect(unitDelay);
      unitDelay.connect(delay);
   

   while (1) {
      cout << "How many samples of delay (negative to quit): ";
      cin  >> delayLength;
      if (delayLength <= 1) break;
      delay.setDelay(delayLength);
      noiseGate.setDurationSamples(delayLength);
      noiseGate.trigger();

      cout << "How long for output sound in seconds: ";
      cin  >> duration;
      numSamples = (int)(duration * 44100);
   
      float amplitude;
      cout << "What amplitude for input noise: ";
      cin  >> amplitude;
      noise.setAmplitude(amplitude);

      action.tick(outsound, numSamples);
   }
      return 0;
}


