//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed May 14 23:44:12 GMT-0800 1997
// Last Modified: Sun Apr 12 12:16:49 PDT 1998
// Filename:      ...sig/doc/examples/sig/sigfile/logistic/logistic.cpp
// Syntax:        C++; sig
//
// Description:   a non-linear limiter based on the logistic function
//                which is 1/(1+e^-x).  Any real-number will be mapped
//                to the range from -1 to 1.  Basically, this program
//                is useful to prevent overloading of the range -1 to 1.
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
   SoundFileIn     insound(infilename);
   SoundFileOut    outsound(outfilename, header);
   LogisticLimiter limitLeft;
   LogisticLimiter limitRight;
   WhiteNoise      noise(1);
   Envelope        ampEnv(options.getString("amp-env"), numSamples);
   Multiply        mulLeft;
   Multiply        mulRight;

   // Connections:
   mulLeft.connect(ampEnv);
   mulRight.connect(ampEnv);
   outsound.connect(limitLeft,0);
   if (header.getChannels() > 1) {
      outsound.connect(limitRight, 1);
   }
   if (strlen(infilename) == 0) {         // use noise
      mulLeft.connect(noise);
      mulRight.connect(noise);

      limitLeft.connect(mulLeft);      
      limitRight.connect(mulRight);      
   } else {                               // use input file
      mulLeft.connect(insound, 1, 0);
      mulRight.connect(insound, 1, 1);

      limitLeft.connect(mulLeft);
      limitRight.connect(mulRight);
   }

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
   opts.define("author=b");
   opts.define("version=b");
   opts.define("example=b");
   opts.define("help=b");
   opts.process(argc, argv);

   if (opts.getBoolean("author")) {
      cout << "Written by Craig Stuart Sapp, "
           << "craig@ccrma.stanford.edu, March 1999" << endl;
      exit(0);
   }
   if (opts.getBoolean("version")) {
      cout << "sigfile: logistic 7 March 1999" << endl;
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
   "# logistic examples:                                                 \n"
   "        logistic -a \"0 0 1 10 2 0\" out.snd                         \n"
   << endl;
}



//////////////////////////////
//
// usage -- how to run the osc program on the command line.
//

void usage(const char* command) {
   cout << endl;
   cout << 
      "A Logistic function amplitude limiter.  Uses noise if no input "
      "soundfile.\n" 
      << "Can handle mono or stero soundfiles.\n";
   cout << endl;
   cout << "Usage: " << command << 
           "[[-a amp][-d duration|-s samples]|insound] outsound" << endl;
   cout << endl;
   cout << "Options: " << endl;
   cout << "   -d = duration in seconds of whitenoise if used (default 1.0)\n";
   cout << "   -s = duration in samples of whitenoise if used (default null)\n";
   cout << "        overrides -d option if specified.\n";
   cout << "   -a = amplitude envelope of whitenoise if used "
        << "(default is \"0 1 1 1\")\n";
   cout << "   --options = list of all options, aliases and defaults \n";
   cout << endl;
}



