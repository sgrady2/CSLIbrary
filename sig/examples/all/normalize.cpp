//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Jan 18 20:37:07 GMT-0800 1998
// Last Modified: Sun Jan 18 20:37:13 GMT-0800 1998
// Filename:      ...sig/doc/examples/sig/sigfile/normalize/normalize.cpp
// Syntax:        C++; sig
//
// Description:   Normalize a soundfile to a given maximum, where 1.0 
//                is absolute maximum possible without overloading the
//                sound.
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char* command);


int main(int argc, char* argv[]) {
   char *inputfilename = NULL;
   char *outputfilename = NULL;
   double newmax = 1.0;
   int i, j;
   double max = 0; 

   if (argc == 4) {
      newmax = atof(argv[1]);
      inputfilename = argv[2];
      outputfilename = argv[3];
   } else if (argc == 3) {
      inputfilename = argv[1];
      outputfilename = argv[2];
   } else {
      exitUsage(argv[0]); 
   }

   SoundHeader header(inputfilename);
   SoundFileIn findmax(inputfilename);
   for (i=0; i<header.getSamples(); i++) {
      for (j=0; j<header.getChannels(); j++) {
         findmax.action();
         if (findmax.output(j) < 0) {
            if (max < -findmax.output(j)) {
               max = -findmax.output(j);
            }
         } else {
            if (max < findmax.output(j)) {
               max = findmax.output(j);
            }
         }
      }
   }
   cout << "Maximum amplitude was: " << max << endl;
   cout << "Rescaling to: " << newmax << endl;
   if (max == 0) max = 1;

   // Elements:
   SoundFileIn  insound(inputfilename);
   Scale        scale[insound.getChannels()];
   SoundFileOut outsound(outputfilename, header);

   // Connections:
   for (i=0; i<insound.getChannels(); i++) {
      scale[i].setScale(newmax/max);
      scale[i].connect(insound, 0, i);   
      outsound.connect(scale[i], i, 0);
   }

   Action action;
   action.tick(outsound, header.getSamples());


   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << "Scales a soundfile in relation to its maximum amplitude. Handles an"
        << endl;
   cout << "arbitrary number of channels." << endl;
   cout << endl;
   cout << "Usage: " << command << " [newMax] insound outsound" << endl;
   cout << endl;
   cout << "      newMax = floating point value for new maximum sample "<< endl;
   cout << "               value, where 1.0 is maximum for linear samples." 
        << endl;
   cout << endl;

   exit(1);
}



