//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Dec  9 12:16:21 PST 1997
// Last Modified: Tue Jan 27 20:54:13 GMT-0800 1998
// Filename:      ...sig/doc/examples/improv/improv/moff/moff.cpp
// Syntax:        C++; improv
//
// Description:   Turns off all MIDI notes on a specified channel.
//

#include "sigControl.h"
#include <iostream.h>
#include <stdlib.h>
#include <ctype.h>

int atohd(const char* aNumber);
void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   int port = 0;
   int channel = 0;

   if (argc == 3) {
      port = atohd(argv[1]);
      channel = atohd(argv[2]);
   } else if (argc == 2) {
      port = atohd(argv[1]);
      channel = 0;
   } else {
      exitUsage(argv[0]);
   }

   if (channel < 0 || channel > 15) exitUsage(argv[0]);

   MidiOutput midi;
   if (midi.getNumPorts() <= port) {
      cout << "Error: highest available port is: " << midi.getNumPorts()-1
           << endl;
      exit(1);
   }
   midi.setPort(port);
   midi.open();
   cout << "Turning off all MIDI sound on channel " << channel << endl;
   midi.silence(channel);
   midi.close();
   
   return 0;
}



int atohd(const char* aNumber) {
   if (aNumber[0] == '0' && tolower(aNumber[1]) == 'x') {
      return (int)strtol(aNumber, (char**)NULL, 16);
   } else {
      return atoi(aNumber);
   }
}
      


void exitUsage(const char* command) {
      cout << "Usage: " << command << " port [channel] " << endl;
      cout << endl;
      cout << "   port       = MIDI output port (usually 0)\n";
      cout << "   channel   = [0..15] the MIDI channel to play note on.\n";
      cout << "      default is 0.\n";
      cout << endl;
      exit(1);
}


