//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Apr 24 01:54:14 PDT 1998
// Last Modified: Fri Apr 24 01:54:20 PDT 1998
// Filename:      ...sig/doc/examples/sig/sigfile/sinebark/sinebark.cpp
// Syntax:        C++; sig
//
// Description:   makes a soundfile with the specified number
//		  of sinewaves per critical band.
//

#include "sig.h"
#include <iostream.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void    checkOptions(Options& opts);
void    example(void);
void    usage(const char* command);
double* createFrequencyArray(const char* method, int count);
double  freq2bark(double freqValue);
double  bark2freq(double barkValue);


double Bark[26] = {
   0, 100, 200, 300, 400, 500, 630, 770, 920, 1080, 1270, 1480,
   1720, 2000, 2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700,
   9500, 12000, 15500, 20000};


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

   // determine if the random number generator needs to be seeded:
   if (options.getInt("seed") != 0) {
      srand(options.getInt("seed"));
   } else {
      srand(time(NULL));
   }
  
   // prepare for a monophonic output file
   SoundHeader header;
   header.setHighMono();

   // determine how many sines per critical band
   int count = options.getInt("number");

   double *frequencies;
   frequencies = createFrequencyArray(options.getString("method"), count);

   // Elements:
   Osc         *oscillators = new Osc[count*25];
   Add          add;
   Scale        scale(options.getDouble("amplitude")/(count*25.0));
   SoundFileOut outsound(options.getArg(1), header);

   int print = options.getBoolean("print");

   // Connections:
   for (int i=0; i<count*25; i++) {
      oscillators[i].setPhase((double)rand()/RAND_MAX);
      oscillators[i].connect(frequencies[i], 0);
      add.connect(oscillators[i]);
      if (print) {
         cout << frequencies[i] << '\n';
      }
   }
   scale.connect(add);
   outsound.connect(scale);

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
   opts.define("a|amp|amplitude=d:1");
   opts.define("d|dur|duration=d:1.0 second");
   opts.define("s|samples=i");
   opts.define("e|seed=i");
   opts.define("m|method=s:equal");
   opts.define("n|number=i:1");
   opts.define("p|print=b");
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
// example -- gives example calls to the sinebark program.
//

void example(void) {
   cout << 
   "# sinebark examples:                                                     \n"
   "#   10 randomly distributed sines in each critical band region for 3 sec:\n"
   "       sinebark -m random -n -d 3 band10.snd                             \n"
   << endl;
}



//////////////////////////////
//
// usage -- how to run the osc program on the command line.
//

void usage(const char* command) {
   cout << 
   "                                                                         \n"
   "Creates sinewaves spaced equally on the basilar membrane or uniform      \n"
   "randomly distributed in each of 25 adjacent critical band regions.       \n"
   "                                                                         \n"
   "Usage: " << command << " [-d duration| -s samples][-a amp][-m method]    \n"
   "         [-n numuber][-p] outsound                                       \n"
   "                                                                         \n"
   "Options:                                                                 \n"
   "   -d = duration of the oscillator in seconds (default 1.0).             \n"
   "   -s = duration in samples.  Overrides the -d option (default null)     \n"
   "   -a = amplitude of output (default 0.5).                               \n"
   "   -n = number of sines to generate in each adjacent critical band       \n"
   "           (default 1)                                                   \n"
   "   -m = method for distributing sines in critical band:                  \n"
   "           equal = equally spaced, random = ramdomly spaced              \n"
   "           (default equal)                                               \n"
   "   -p = print the frequencies of the sinewaves being created.            \n"
   "   --options = list of all options, aliases and default values.          \n"
   "                                                                         \n"
   "                                                                         \n"
   << endl;
}



//////////////////////////////
//
// createFrequencyArray -- creates a list of the frequencies
// 	from bark values based on spacing method and number
//	of sinewaves per critical band.
//

double* createFrequencyArray(const char* method, int count) {
   double* output;
   output = new double[count*25];
   double step;
   int i, j;

   if (strcmp(method, "random") == 0) {
      for (i=0; i<25; i++) {
         for (j=0; j<count; j++) {
            output[i*count+j] = bark2freq(i + (double)rand()/RAND_MAX);
         }
      }
   } else if (strcmp(method, "equal") == 0) {
      step = 1.0/count;
      for (i=0; i<25*count; i++) {
         output[i] = bark2freq((i+0.5)*step);
      }
   } else if (strcmp(method, "harmonic") == 0) {
      step = 22050/(25*count);
      for (i=0; i<25*count; i++) {
         output[i] = (i+1)*step;
      }
   } else {
      cout << "Error: unknown distribution method: " << method << endl;
      exit(1);
   }

   return output;
}



//////////////////////////////
//
// freq2bark -- converts frequency in Hertz to the Bark scale.
//

double freq2bark(double freqValue) {
   if (freqValue <= 500.0) {           // linear bark region
      return freqValue/100.0;   
   } else if (freqValue >= 20000.0) {  // out of audible range
      return 25.0;
   } else {                            // logarithmic bark region
      int index = 6;      
      while (freqValue >= Bark[index]) {
         index++;
      }
      // index now points to bark value just higher than freqValue.
      // Use linear interpolation on the logarighmic scale to
      // find intermediate bark value.
      return index - fabs((log(Bark[index])-log(freqValue)) /
               (log(Bark[index])-log(Bark[index-1])));
   }
}



//////////////////////////////
//
// bark2freq -- converts Bark scale values to frequency in Hertz.
//

double bark2freq(double barkValue) {
   if (barkValue <= 5.0) {             // linear bark region
      return barkValue * 100.0;
   } else if (barkValue >= 25.0) {     // out of audible range
      return 20000.0;
   } else {                            // logarithmic bark region
      int index = (int)barkValue;
      double fraction = barkValue - index;
      return exp((log(Bark[index+1])-log(Bark[index]))*fraction +
         log(Bark[index]));
   }   
}



