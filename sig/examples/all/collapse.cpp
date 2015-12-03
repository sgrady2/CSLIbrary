//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed May 28 20:07:37 GMT-0800 1997
// Last Modified: Fri Apr 17 10:15:02 PDT 1998
// Filename:      ...sig/doc/examples/sig/sigfile/collapse/collapse.cpp
// Syntax:        C++; sig 
//
// Description:   mixes stereo channels into a mono output soundfile
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

   SoundHeader header(options.getArg(1));
   header.setChannels(1);

   // Elements:
   SoundFileIn  insound(options.getArg(1));
   SoundFileOut outsound(options.getArg(2), header);
   Envelope     ampEnv(options.getString("amp-env"));
   Add          add;
   Multiply     mul;

   // Connections:
   outsound.connect(mul);
   mul.connect(ampEnv);
   mul.connect(add);
   for (int i=0; i<insound.getChannels(); i++) {
      add.connect(insound, i, i);
   }

   Action action;
   action.tick(outsound, insound.getSamples());

   return 0;
}


///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// checkOptions -- handle command-line options.
//

void checkOptions(Options& opts) {
   opts.define("a|amp|amp-env=s:0 1 1 1");
   opts.define("author=b");
   opts.define("version=b");
   opts.define("example=b");
   opts.define("help=b");
   opts.process();

   if (opts.getBoolean("author")) {
      cout << "Written by Craig Stuart Sapp, "
           << "craig@ccrma.stanford.edu, May 1997" << endl;
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

   // must have one input and one output filename
   if (opts.getArgCount() != 2) {
      cout << "Error: need one input sound and one output sound name" << endl;
      usage(opts.getCommand());
      exit(1);
   }

}




//////////////////////////////
//
// example -- gives example calls to the osc program.
//

void example(void) {
   cout <<
   "# collapse examples:                                                     \n"
   "#   mix channels from a stereo file into a mono file.                    \n"
   "       collapse stereo.snd mono.snd                                      \n"
   << endl;
}



//////////////////////////////
//
// usage -- how to run the osc program on the command line.
//

void usage(const char* command) {
   cout <<
   "                                                                         \n"
   "Adds input file channels into a monophonic output soundfile. Input       \n"
   "soundfile can have any number of channels.                               \n"
   "                                                                         \n"
   "Usage: " << command << " [-a ampEnv] insound outsound                    \n"
   "                                                                         \n"
   "Options:                                                                 \n"
   "   -a = amplitude envelope applied to output channel                     \n"
   "           (default \"0 1, 1 1\").                                       \n"
   "   --options = list of all options, aliases and default values.          \n"
   "                                                                         \n"
   "                                                                         \n"
   << endl;
}


