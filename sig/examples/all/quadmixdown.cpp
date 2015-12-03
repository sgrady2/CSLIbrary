//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri May 28 01:26:03 PDT 1999
// Last Modified: Fri May 28 01:26:07 PDT 1999
// Filename:      ...sig/doc/examples/sig/sigfile/quadmixdown/quadmixdown.cpp
// Syntax:        C++; sig
//
// Description:   mix a quad soundfile into a stereo file
//

#include "sig.h"
#include <iostream.h>
#include <stdlib.h>

void checkOptions(Options& opts);
void example(void);
void usage(const char* command);


///////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
   Options options(argc, argv);
   checkOptions(options);

   char* infilename = options.getArg(1);
   char* outfilename = options.getArg(2);
   SoundHeader header(infilename);
   if (header.getChannels() != 4) {
      cout << "Error: input soundfile must be quadraphonic" << endl;
      exit(1);
   } else {
      header.setChannels(2);
   }

   int numSamples = header.getSamples();

   // Elements:
   SoundFileIn  insound(infilename);
   SoundFileOut outsound(outfilename, header);
   Add          addLeft;
   Add          addRight;
   Scale        scaleLeft(0.5);
   Scale        scaleRight(0.5);

   // Connections:
   addLeft.connect(insound, 0, 0);
   addLeft.connect(insound, 2, 1);
   addRight.connect(insound, 1, 0);
   addRight.connect(insound, 3, 1);
   scaleLeft.connect(addLeft);
   scaleRight.connect(addRight);
   outsound.connect(scaleLeft);
   outsound.connect(scaleLeft);

   Action action;
   action.tick(outsound, numSamples);

   return 0;
}


///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// checkOptions -- handle command-line options.
//

void checkOptions(Options& opts) {
//   opts.define("a|amp|amp-env=s:0 1 1 1");
   opts.define("author=b");
   opts.define("version=b");
   opts.define("example=b");
   opts.define("help=b");
   opts.process();

   if (opts.getBoolean("author")) {
      cout << "Written by Craig Stuart Sapp, "
           << "craig@ccrma.stanford.edu, May 1999" << endl;
      exit(0);
   }
   if (opts.getBoolean("version")) {
      cout << "compiled: " << __DATE__ << endl;
      cout << SIG_VERSION << endl;
      exit(0);
   }
   if (opts.getBoolean("help")) {
      usage(opts.getCommand());
      exit(0);
   }
   if (opts.getBoolean("example")) {
      example();
      exit(0);
   }

   if (opts.getArgCount() == 0) {
      cout << "Error: need one output file name." << endl;
      usage(opts.getCommand());
      exit(1);
   } else if (opts.getArgCount() == 1) {
      cout << "Error: need both and input and an output file." << endl;
      exit(1);
   } else if (opts.getArgCount() > 2) {
      cout << "Error: too many arguments.  Given "
           << opts.getArgCount() << " but need only 2." << endl;
      usage(opts.getCommand());
      exit(1);
   }

}
  


//////////////////////////////
//
// example -- gives example calls to the dcblock program.
//

void example(void) {
   cout <<
   "# quadmixdown examples:                                                \n"
   "       quadmixdown input4.wav output2.wav                              \n"
   << endl;
}



//////////////////////////////
//
// usage -- how to run the quadmixdown program on the command line.
//

void usage(const char* command) {
   cout << 
   "\n"
   "Mixes a quad soundfile into a stereo file.\n"
   "Will work on quad soundfiles. " 
   "\n"
   "Usage: " << command << " insound outsound\n" 
   "\n"
   "Options: \n"
   "   none\n"
   "   --options = list of all options, aliases and defaults \n"
   << endl;
}



