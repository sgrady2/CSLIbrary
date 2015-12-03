//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 12 21:54:05 PDT 1998
// Last Modified: Sun Apr 12 21:54:08 PDT 1998
// Filename:      ...sig/doc/examples/sig/sigfile/quantize/quantize.cpp
// Syntax:        C++; sig
//
// Description:   quantizes a sound to a given number of amplitude levels.
//                either integer or floating point quantization is
//                possible.
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
   if (header.getBitsPerSample() != 16) {
      cout << "Error: this program can only process 16-bit samples." << endl;
      exit(1);
   }
   int numSamples = header.getSamples() * header.getChannels();

   // Elements:
   SoundFileIn  insound(options.getArg(1));
   SoundFileOut outsound(options.getArg(2), header);

   int i;
   double currentSample;
   if (options.getBoolean("float")) {
      int scaleBits = options.getInteger("scale");
      int mantissaBits = options.getInteger("mantissa");
      for (i=0; i<numSamples; i++) {
         currentSample = (double) insound.extractSample16Bit() / 0x8000;
         currentSample = DequantizeFloatMidtread(
           QuantizeFloatMidtread(scaleBits, mantissaBits, currentSample));
         outsound.insertSample16Bit((short)(currentSample * 0x7fff + 0.5));
      }
   } else {  // linear quantization
      int bits = options.getInteger("bits");
      for (i=0; i<numSamples; i++) {
         currentSample = (double)insound.extractSample16Bit() / (long)0x8000;
         currentSample = DequantizeMidtread(
           QuantizeMidtread(bits, currentSample));
         outsound.insertSample16Bit((short)(currentSample * 0x7fff + 0.5));
      }
   }

   return 0;
}


///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// checkOptions -- handle command-line options.
//

void checkOptions(Options& opts) {
   opts.define("b|bits|bit=i:8");
   opts.define("s|scale|e|exponent=i:3");
   opts.define("m|mantissa|mant=i:5");
   opts.define("f|float=b");
   opts.define("author=b");
   opts.define("version=b");
   opts.define("example=b");
   opts.define("help=b");
   opts.process();

   if (opts.getBoolean("author")) {
      cout << "Written by Craig Stuart Sapp, "
           << "craig@ccrma.stanford.edu, April 1998" << endl;
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

   if (opts.getBoolean("float")) {
      if (opts.getInt("scale") + opts.getInt("mantissa") > 16) {
         cout << "Error: sum of scale and mantissa must be less than 16." 
              << endl;
         exit(1);
      }
   } else if (opts.getInt("bits") > 16) {
      cout << "Error: number of bits must be less than 16." << endl; 
      exit(1);
   }

   // can only have one input and one output filename
   if (opts.getArgCount() != 2) {
      cout << "Error: need one input and one output file name." << endl;
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
   "# quantize examples:                                                   \n"
   "       quantize -f -s 3 -m 5 input.snd  output.snd                     \n"
   << endl;
}



//////////////////////////////
//
// usage -- how to run the osc program on the command line.
//

void usage(const char* command) {
   cout << endl;
   cout << "Quantizes a 16-bit sound using fewer bits, and then converts\n";
   cout << "to 16 bits samples. Will work on mono or stereo soundfiles.\n";
   cout << endl;
   cout << "Usage: " << command << " [-b bits| -f -s sale -m mantissa] "
           "insound outsound\n";
   cout << endl;
   cout << "Options: " << endl;
   cout << "   -b = The number of bits to use to midtread quantize (default 8)\n";
   cout << "        cannot be greater than 16.\n";
   cout << "   -f = use floating point midtread quantization\n";
   cout << "   -s = number of bits for the scale of floating point "
           "quantization\n";
   cout << "   -m = number of bits for the mantissa of floating point "
           "quantization\n";
   cout << "   --options = list of all options, aliases and defaults \n";
   cout << endl;
}



