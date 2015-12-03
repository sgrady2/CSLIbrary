//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Mar 25 15:46:22 GMT-0800 1998
// Last Modified: Wed Apr  8 23:22:49 PDT 1998
// Filename:      ...sig/doc/examples/sig/sigfile/dcblock/dcblock.cpp
// Syntax:        C++; sig
//
// Description:   A filter which removes DC (0 Hertz).  If no input soundfile, 
//		  then use whitenoise
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
   DCBlock      leftBlock;
   DCBlock      rightBlock;
   WhiteNoise   noise(1);
   Envelope     noiseAmpEnv(options.getString("amp-env"));
   Multiply     mulLeft;
   Multiply     mulRight;
   Envelope     polePosition(options.getString("pole-env"), numSamples);

   // Connections:

   if (strlen(infilename) == 0) {         // use noise
      mulLeft.connect(noise);      
      mulRight.connect(noise);      
   } else {                               // use input file
      mulLeft.connect(insound, 0, 0);
      mulRight.connect(insound, 0, 1);
   }

   mulLeft.connect(noiseAmpEnv);
   mulRight.connect(noiseAmpEnv);

   leftBlock.connect(mulLeft, 0);
   rightBlock.connect(mulRight, 0);

   leftBlock.connect(polePosition, 1);
   rightBlock.connect(polePosition, 1);
       
   outsound.connect(leftBlock, 0);
   if (header.getChannels() == 2) {
      outsound.connect(rightBlock, 1);
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

void checkOptions(Options& opts) {
   opts.define("a|amp|amp-env=s:0 1 1 1");
   opts.define("d|dur|duration=d:1.0 second");
   opts.define("s|samples=i");
   opts.define("p|pole|pole-env=s:0 0.995 1 0.995");
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
// example -- gives example calls to the dcblock program.
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
   cout << "Removes DC signal.  Uses whitenoise if no input soundfile." << endl;
   cout << "Will work on mono or stereo soundfiles. " << endl;
   cout << endl;
   cout << "Usage: " << command << " [-p pole][[-a amp][-d duration|-s samples]"
        << "|insound] outsound" << endl;
   cout << endl;
   cout << "Options: " << endl;
   cout << "   -p = pole position envelope (default is \"0 0.995 1 0.995\")\n";
   cout << "   -d = duration in seconds of whitenoise if used (default 1.0)\n";
   cout << "   -s = duration in samples of whitenoise if used (default null)\n";
   cout << "        overrides -d option if specified.\n";
   cout << "   -a = amplitude envelope of whitenoise if used "
        << "(default is \"0 1 1 1\")\n";
   cout << "   --options = list of all options, aliases and defaults \n";
   cout << endl;
}



