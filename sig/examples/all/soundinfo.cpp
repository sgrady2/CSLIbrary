//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Nov 14 21:41:18 PST 1996
// Last Modified: Sat May 24 23:33:28 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/soundinfo/soundinfo.cpp
// Syntax:        C++; sig
//
// Description:   display the sound data format information for a soundfile
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   if (argc == 1) exitUsage(argv[0]);
   for (int i=1; i<argc; i++) {
      SoundHeader header(argv[i]);
      header.setOutputType(TYPE_TEXT);
      cout << "Filename:        " << argv[i] << endl;
      cout << header;
      cout << endl;
   }

   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout <<  "Example of soundfile header extaction." << endl;
   cout << endl;
   cout << "Usage: " << command << " insound [...] " << endl;
   cout << endl;

   exit(1);
}



