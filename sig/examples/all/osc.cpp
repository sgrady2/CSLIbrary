//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun May 11 22:45:14 GMT-0800 1997
// Last Modified: Tue Apr  7 23:59:27 PDT 1998
// Filename:      ...sig/doc/examples/sig/sigfile/osc/osc.cpp
// Syntax:        C++; sig
//
// Description:   make a soundfile with the specified moveable 
//                frequency/amplitude sinewave
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

   // determine how many samples in the output based on the duration
   int numSamples;
   if (options.getInt("samples") > 0) {
      numSamples = options.getInt("samples");
   } else {
      numSamples = (int)(options.getDouble("duration") * 44100 + 0.5);
   }
  
   // prepare for a monophonic output file
   SoundHeader header;
   header.setHighMono();

   // Elements:
   Osc          oscillator;
   SoundFileOut outsound(options.getArg(1), header);
   Envelope     freqEnv(options.getString("freq-env"), numSamples);
   Envelope     ampEnv(options.getString("amp-env"), numSamples);

   // Connections:
   oscillator.connect(freqEnv, 0);
   oscillator.connect(ampEnv, 1);
   outsound.connect(oscillator);

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
   opts.define("a|amp|amp-env=s:0 1 1 1");
   opts.define("d|dur|duration=d:1.0 second");
   opts.define("s|samples=i");
   opts.define("f|freq|freq-env=s:0 440 1 440");
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
   "# osc examples:                                                          \n"
   "#   a quick sweep in frequency from 0 to 22050 Hz with a saw-like        \n"
   "#   amplitude envelope:                                                  \n"
   "       osc chirp.snd -d 0.5 -f \"0 0 1 22050\" -a \"0 0 1 1 10 0\"       \n"
   << endl;
}



//////////////////////////////
//
// usage -- how to run the osc program on the command line.
//

void usage(const char* command) {
   cout << 
   "                                                                         \n"
   "Creates a frequency and amplitude varying sinusoid.                      \n"
   "                                                                         \n"
   "Usage: " << command << " [-d duration][-f freqEnv][-a ampEnv] outsound   \n"
   "                                                                         \n"
   "Options:                                                                 \n"
   "   -d = duration of the oscillator in seconds (default 1.0).             \n"
   "   -s = duration in samples.  Overrides the -d option (default null)     \n"
   "   -a = amplitude envelope (default \"0 1, 1 1\").                       \n"
   "   -f = frequency envelope (default \"0 440, 1 440\").                   \n"
   "   --options = list of all options, aliases and default values.          \n"
   "                                                                         \n"
   "                                                                         \n"
   << endl;
}



