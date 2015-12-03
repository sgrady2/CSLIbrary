//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 20 18:14:44 GMT-0800 1997
// Last Modified: Wed Apr  8 22:23:46 PDT 1998
// Filename:      ...sig/doc/examples/sig/sigfile/periods/periods.cpp
// Syntax:        C++; sig
//
// Description:   creates a soundfile with specified frequency/cycles 
//		  under a cos^2 window
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

   // determine how many sample to produce for soundfile
   int numSamples = (int)(1.0 / options.getDouble("freq") *
                          options.getDouble("periods") * 44100 + 0.5);

   // prepare for a monophonic output file at 44.1 kHz sampling rate
   SoundHeader header;
   header.setHighMono();

   // Elements:
   FixedSine     sinewave(options.getDouble("freq"), options.getDouble("amp"));
   FixedSine     window(options.getDouble("freq")/options.getDouble("periods")/
                       2.0);
   Multiply      multiply;
   SoundFileOut  outsound(options.getArg(1), header);

   // Connections:
   multiply.connect(sinewave);
   multiply.connect(window);
   multiply.connect(window);
   outsound.connect(multiply);

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
   opts.define("a|amp=d:1.0");
   opts.define("f|freq=d:440.0");
   opts.define("p|periods=i:1");
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

}



//////////////////////////////
//
// example -- gives example calls to the osc program.
//

void example(void) {
   cout <<
   "# periods examples:                                                    \n"
   "#    Here is an example for periods which makes a soundfile with       \n"
   "#    7 periods of a 1000 Hz sinewave with a cos^2 amplitude env:       \n"
   "        periods -p 7 -f 1000 -a 0.5 1000-7.snd                         \n"
   << endl;
}



//////////////////////////////
//
// usage -- how to run the osc program on the command line.
//

void usage(const char* command) {
   cout << endl;
   cout 
   << "Creates a specified number of periods of a certain frequency under\n"
   << "a cos^2 amplitude window. Can be used to determine how many periods \n"
   << "of a sound are needed to accurately detect a frequency.\n";
   cout << endl;
   cout << "Usage: " << command << " [-f freq][-p periods][-a amp] "
        << "outsound" << endl;
   cout << endl;
   cout << "Options:" << endl;
   cout << "   -f = frequency of sinesave in Hertz (default 440.0)\n";
   cout << "   -p = number of periods of the sinwave to create (default 1)\n";
   cout << "   -a = maximum amplitude of sinewave (default 1.0)\n";
   cout << "   --options = list of all options, aliases and defaults \n";
   cout << endl;
}



