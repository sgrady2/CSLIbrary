//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed May 14 23:44:12 GMT-0800 1997
// Last Modified: Sun Apr 12 12:16:49 PDT 1998
// Filename:      ...sig/doc/examples/sig/sigfile/resonator/resonator.cpp
// Syntax:        C++; sig
//
// Description:   a resonance filter example.  If no input soundfile, 
//		  then use whitenoise
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

   SoundHeader header;
   char* infilename = "";
   char* outfilename = NULL;

   long numSamples = 0;

   if (options.getArgCount() == 1) {   // use whitenoise
      outfilename = options.getArg(1);
      header.setHighMono();
      if (options.getInt("samples") > 0) {
         numSamples = options.getInt("samples");
      } else {
         numSamples = (int)(options.getDouble("duration") * 44100 + 0.5);
      }
   } else {                            // use input soundfile
      infilename = options.getArg(1);
      header.setHeader(infilename);
      numSamples = header.getSamples();
      outfilename = options.getArg(2);
   }


   // Elements:
   SoundFileIn  insound(infilename);
   SoundFileOut outsound(outfilename, header);
   Resonator    resonatorLeft;
   Resonator    resonatorRight;
   WhiteNoise   noise(1);
   Envelope     noiseAmpEnv(options.getString("amp-env"));
   Multiply     noiseMul;
   Envelope     frequencyEnv(options.getString("freq-env"), numSamples);
   Envelope     bandwidthEnv(options.getString("bandwidth-env"), numSamples);

   // Connections:
   outsound.connect(resonatorLeft,0);
   if (header.getChannels() > 1) {
      outsound.connect(resonatorRight, 1);
   }
   if (strlen(infilename) == 0) {         // use noise
      resonatorLeft.connect(noise, 0);      
      resonatorRight.connect(noise, 0);      
   } else {                               // use input file
      resonatorLeft.connect(insound, 0, 0);
      resonatorRight.connect(insound, 0, 1);
   }
   resonatorLeft.connect(frequencyEnv, 1);
   resonatorLeft.connect(bandwidthEnv, 2);
   resonatorRight.connect(frequencyEnv, 1);
   resonatorRight.connect(bandwidthEnv, 2);

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
   opts.define("b|band|band-env|bandwidth|bandwidth-env=s:0 0.995 1 0.995");
   opts.define("f|freq|freq-env=s:0 440 1 440");
   opts.define("author=b");
   opts.define("version=b");
   opts.define("example=b");
   opts.define("help=b");
   opts.process(argc, argv);

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

   // can only have one input and one output filename
   if (opts.getArgCount() == 0) {
      cout << "Error: need one output file name." << endl;
      usage(opts.getCommand());
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
// example -- gives example calls to the resonator program.
//

void example(void) {
   cout <<
   "# resonator examples:                                                  \n"
   "#    Keeping a constant bandwidth, sweeping frequency linearly:        \n"
   "        resonator -f \"0 100 1 5000\" -b \"0 100 1 100\"               \\\n"
   "            -a \"0 3 1 3\" -d 5 resonator1.snd                         \n"
   "#    Constant center frequency, widening and narrowing bandwidth:      \n"
   "        resonator -f \"0 800, 45 800, 55 1200, 100 1200\"            \\\n"
   "   -b \"0 0.01 10 1 50 100 100 1000 150 100 190 1 195 0.2 200 0.01\" \\\n"
   "        -d 7 -a \"0 0.5 1 0.5\" resonator2.snd                         \n"
   "#    Changing both bandwidth and center frequency:                     \n"
   "         resonator -f \"0 1000 1 900 2 100 3 100\" -d 10             \\\n"
   "            -b \"0 0.01 1 100 2 10 4 0.1 5 1000 6 100 7 0.01\"       \\\n"
   "            -a \"0 3 1 3\" resonator3.snd                              \n"
   << endl;
}



//////////////////////////////
//
// usage -- how to run the osc program on the command line.
//

void usage(const char* command) {
   cout << endl;
   cout << 
      "A frequency resonator.  Uses whitenoise if no input soundfile.\n" 
      << "Can handle mono or stero soundfiles.\n";
   cout << endl;
   cout << "Usage: " << command << " [-f freq][-b bandwidth]"
           "[[-a amp][-d duration|-s samples]|insound] outsound" << endl;
   cout << endl;
   cout << "Options: " << endl;
   cout << "   -f = center frequency envelope (default is \"0 440 1 440\")\n";
   cout << "   -b = bandwidth envelope (default is \"0 100 1 100\")\n";
   cout << "   -d = duration in seconds of whitenoise if used (default 1.0)\n";
   cout << "   -s = duration in samples of whitenoise if used (default null)\n";
   cout << "        overrides -d option if specified.\n";
   cout << "   -a = amplitude envelope of whitenoise if used "
        << "(default is \"0 1 1 1\")\n";
   cout << "   --options = list of all options, aliases and defaults \n";
   cout << endl;
}



