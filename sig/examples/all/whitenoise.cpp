//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Apr 30 22:53:46 GMT-0800 1997
// Last Modified: Fri Apr 17 00:20:59 PDT 1998
// Filename:      ...sig/doc/examples/sig/sigfile/whitenoise/whitenoise.cpp
// Syntax:        C++; sig
//
// Description:   creates a whitenoise soundfile
//

#include "sig.h"
#include <iostream.h>
#include <stdlib.h>

void checkOptions(Options& opts, int argc, char** argv);
void example(void);
void usage(const char* command);


///////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
   Options options;
   checkOptions(options, argc, argv);

   int numSamples;
   if (options.getInt("samples") > 0) {
      numSamples = options.getInt("samples");
   } else {
      numSamples = (int)(options.getDouble("duration") * 44100 + 0.5);  
   }

   // prepare for a monophonic output file at 44.1 kHz sampling rate
   SoundHeader header;
   header.setHighMono();

   // Elements:
   WhiteNoise   noise(1, options.getInt("seed"));
   Envelope     ampenv(options.getString("amp-env"), numSamples);
   Multiply     mul;
   SoundFileOut outsound(options.getArg(1), header);
   Downsample   hold(options.getInt("hold"));

   // Connections:
   hold.connect(noise);
   mul.connect(hold);
   mul.connect(ampenv);
   outsound.connect(mul);

   Action action;
   action.tick(outsound, numSamples);

   return 0;
}


///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// checkOptions -- handle command-line options.
//

void checkOptions(Options& opts, int argc, char** argv) {
   opts.define("a|amp|amp-env=s:0 1 1 1");
   opts.define("d|dur|duration=d:1.0 second");
   opts.define("s|samples=i");
   opts.define("e|seed=i:");
   opts.define("h|hold|=i:1 sample");
   opts.define("author=b");
   opts.define("version=b");
   opts.define("example=b");
   opts.define("help=b");
   opts.process(argc, argv);

   if (opts.getBoolean("author")) {
      cout << "Written by Craig Stuart Sapp, "
           << "craig@ccrma.stanford.edu, April 1997" << endl;
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

   // can only have one output filename
   if (opts.getArgCount() == 0) {
      cout << "Error: need one output file name." << endl;
      usage(opts.getCommand());
      exit(1);
   } else if (opts.getArgCount() > 1) {
      cout << "Error: too many arguments.  Given "
           << opts.getArgCount() << " but need only 1." << endl;
      usage(opts.getCommand());
      exit(1);
   }

   // hold option must be positive
   if (opts.getInt("hold") <= 0) {
      cout << "Error: the random sample hold must be positive" << endl;
      exit(1);
   }
}



//////////////////////////////
//
// example -- gives example calls to the osc program.
//

void example(void) {
   cout <<
   "# whitenoise examples:                                                 \n"
   "#    Here is an example for whitenoise which makes a soundfile with    \n"
   "#    one second of whitenoise with a triangular amplitude envelope.    \n"
   "       whitenoise noise.snd -d 1.0 -a \"0 0 1 1 2 0\"                  \n"
   << endl;
}



//////////////////////////////
//
// usage -- how to run the whitenoise program on the command line.
//

void usage(const char* command) {
   cout << 
   "                                                                         \n"
   "Generates whitenoise or slightly filtered whitenoise.                    \n"
   "                                                                         \n"
   "Usage: " << command 
             << " [-d duration|-s samples][-a amp-env] \n"
             << "                                 [-e seed][-h hold] outsound\n"
   "                                                                         \n"
   "Options:                                                                 \n"
   "   -d = duration of the output in seconds (default 1.0).                 \n"
   "   -s = duration of the output in samples (default is null).             \n"
   "           will override the -d option if specified.                     \n"
   "   -a = amplitude envelope (default \"0 1 1 1\"),                        \n"
   "   -h = number of samples to hold the randomly generated sample          \n"
   "           (default is 1).                                               \n"
   "   -e = random number seed, 0 will generate random seed (default is 0)   \n"
   "   --options = list of all options, aliases and defaults.                \n"
   << endl;
}



