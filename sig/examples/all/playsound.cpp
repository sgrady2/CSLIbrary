//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat May 10 12:13:42 PDT 1997
// Last Modified: Sun May 25 01:19:06 GMT-0800 1997
// Filename:      .../sig/doc/examples/sigaudio/playsound/playsound.cpp
// Syntax:        C++; sig
// $Smake:        cc -Wall -DLINUX -O -o %b %f -I../include %
//                -L../lib -lg++ -lsig && strip %b
//
// Description:   plays a soundfile on a LINUX computer.
//


#include "sig.h"


int main(int argc, char* argv[]) {
   if (argc != 2) {
      cerr << "Usage: " << argv[0] << " input.snd\n";
      exit(1); 
   }

   SoundHeader header(argv[1]);

   // Elements:
   SoundFileIn    insound(argv[1]);
   LinuxSoundOut  outsound(44100, 2, 0);        // srate, channels, port

   // Connections:
   outsound.connect(insound, 0, 0);
   outsound.connect(insound, 1, 1);

   Action action;
   action.tick(outsound, insound.getSamples());

   return 0;
}


