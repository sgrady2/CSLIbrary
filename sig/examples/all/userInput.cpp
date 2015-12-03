//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Apr 29 22:44:16 GMT-0800 1997
// Last Modified: Sat May 24 23:22:52 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/userInput/userInput.cpp
// Syntax:        C++; sig
//
// Description:   an example of user input/output signal
//

#include "sig.h"
#include <iostream.h>

void exitUsage(const char *command);


int main(int argc, char* argv[]) {
   if (argc != 1) exitUsage(argv[0]);

   // Elements:
   UserIn     input;
   Delayline  delay;
   UserOut    output;

   // Connections:
   delay.connect(input);
   output.connect(delay);

   int delayamount;
   cout << "Enter a delay length for the delayline: ";
   cin  >> delayamount;
   delay.setDelay(delayamount);

   int numSamples;
   cout << "Enter the number of samples to calculate: ";
   cin  >> numSamples;

   Action action;
   action.tick(output, numSamples);

   return 0;
}



void exitUsage(const char* command) {
   cout << endl;
   cout << 
   "A demonstratin of generating a signal from user input from the keyboard.\n";
   cout << endl;
   cout << "Usage: " << command << " " << endl;
   cout << endl;
   cout << "Program input: " << endl;
   cout << "   (a) delay length in samples" << endl;
   cout << "   (b) number of samples to run through" << endl;
   cout << endl;

   exit(1);
}



