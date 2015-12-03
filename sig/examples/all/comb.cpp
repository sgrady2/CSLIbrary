//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 20 19:43:40 GMT-0800 1997
// Last Modified: Mon Apr 20 11:27:58 PDT 1998
// Filename:      ...sig/doc/examples/sig/sigfile/comb/comb.cpp
// Syntax:        C++; sig
//
// Description:   a comb filter.  If no input, then use whitenoise.
//		  Can handle mono or stereo files.
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

   SoundHeader header;
   char* infilename = "";
   char* outfilename = NULL;
   int numSamples;
 
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
   Comb         combLeft;
   Comb         combRight;
   Envelope     fundEnv(options.getString("fundamental-env"));
   Envelope     gainEnv(options.getString("gain-env"));
   WhiteNoise   noise(1);
   Envelope     ampEnv(options.getString("amp-env"));
   Multiply     mul;


   // Connections:
   outsound.connect(combLeft);
   if (header.getChannels() == 2) {
      outsound.connect(combRight, 0, 1);
   }

   mul.connect(noise);
   mul.connect(ampEnv);
   if (strlen(infilename) == 0) {         // use noise
      combLeft.connect(mul, 0);      
      combRight.connect(mul, 0);      
   } else {                               // use input file
      combLeft.connect(insound, 0, 0);
      combRight.connect(insound, 0, 1);
   }
   combLeft.connect(fundEnv, 0, 2);
   combRight.connect(fundEnv, 0, 2);
   combLeft.connect(gainEnv, 0, 1);
   combRight.connect(gainEnv, 0, 1);

   Action action;
   action.tick(outsound, numSamples);
   
   // disconnect signal and add ending reverb
   combLeft.disconnect(0);
   combRight.disconnect(0);
   action.tick(outsound, options.getInt("addsamples"));
 
   return 0;
}


///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// checkOptions -- handle command-line options.
//

void checkOptions(Options& opts) {
   opts.define("a|amp|amp-env=s:0 1 1 1");
   opts.define("f|fundamental|fundamental-env|fund-env=s:0 440 1 440");
   opts.define("g|gain|gain-env=s:0 0.9 1 0.9");
   opts.define("d|dur|duration=d:1.0 second");
   opts.define("s|samples=i");
   opts.define("addsamples=i:44100");
   opts.define("author=b");
   opts.define("version=b");
   opts.define("example=b");
   opts.define("help=b");
   opts.process();

   if (opts.getBoolean("author")) {
      cout << "Written by Craig Stuart Sapp, "
           << "craig@ccrma.stanford.edu, March 1998" << endl;
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
// example -- gives example calls to the comb program.
//

void example(void) {
   cout <<
   "# dcblock examples:                                                    \n"
   "#   Here is an example for dcblock which makes a soundfile with        \n"
   "#   dcblocked whitenoise                                               \n"
   "       dcblock -d 1.0 blocked.snd                                      \n"
   << endl;
}



//////////////////////////////
//
// usage -- how to run the osc program on the command line.
//

void usage(const char* command) {
   cout << endl;
   cout << 
      "A comb filter.  Uses whitenoise if no input sound is specified" << endl;
   cout << endl;
   cout << "Usage: " << command << " [insound] outsound" << endl;
   cout << endl;
   cout << "Program input: " << endl;
   cout << "   (a) amplitude of input noise (if no input sound)" << endl;
   cout << "   (b) duration of noise in samples (if no input sound)" << endl;
   cout << "   (c) fundamental frequency of comb filter in Hz" << endl;
   cout << "   (d) gain factor for feedback [0 .. 1]" << endl;
   cout << endl;
}



