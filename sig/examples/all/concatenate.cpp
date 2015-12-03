//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu May  1 11:57:59 GMT-0800 1997
// Last Modified: Thu Apr 16 21:25:54 PDT 1998
// Filename:      ...sig/doc/examples/sig/sigfile/concatenate/concatenate.cpp
// Syntax:        C++; sig
//
// Description:   combine soundfiles serially into one output file.
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

   // determine how many samples of silence to add between soundfiles
   int numBlankSamples;
   SoundHeader header;
   header.setHeader(options.getArg(1));
   if (options.getInt("time") > 0) {
      numBlankSamples = (int)(options.getDouble("time") * 
         header.getSrate() + 0.5);
   } else if (options.getInt("samples") > 0) {
      numBlankSamples = options.getInt("samples");
   } else {
      numBlankSamples = 0;
   }
 
   int numInSounds = options.getArgCount() - 1;

   // check to see if there is a stereo file in the list of soundfiles
   int i;
   int chan = 1;
   for (i=0; i<numInSounds; i++) {
      header.setHeader(options.getArg(i+1));
      if (header.getChannels() == 2) {
         chan = 2;
         break;
      }
   }
   header.setChannels(chan);

   // Elements
   SoundFileOut outsound(options.getArg(numInSounds + 1), header);
   SoundFileIn  insound;
   Constant     silence(0);


   Action action;

   for (i=0; i<numInSounds; i++) {
      insound.setFile(options.getArg(i+1));
      outsound.connect(insound, 0);
      if (chan == 2) {
         if (insound.getChannels() == 2) {
            outsound.connect(insound, 1, 1);
         } else {  // duplicate mono input channel if a stereo output
            outsound.connect(insound, 1, 0);
         }
      }

      if (!options.getBoolean("quiet")) {
         cout << "Reading sound " << options.getArg(i+1) << " which has " 
              << insound.getSamples() << " samples." << endl;
      }

      // copy the soundfile to the output soundfile
      action.tick(outsound, insound.getSamples());

      if (i < numInSounds-1) {
         outsound.connect(silence, 0);
         if (chan == 2) {
            outsound.connect(silence, 0, 1);   
         }
         action.tick(outsound, numBlankSamples);
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
   opts.define("t|time=d");
   opts.define("s|samples=i:0 samples");
   opts.define("q|quiet=b");
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

   // must have at least two sound filenames
   if (opts.getArgCount() < 2) {
      cout << "Error: need at least one input soundfile." << endl;
      usage(opts.getCommand());
      exit(1);
   }

}



//////////////////////////////
//
// example -- gives example calls to the concatenate program.
//

void example(void) {
   cout <<
   "                                                                         \n"
   "# concatenate examples:                                                  \n"
   "#   Makes a soundfile with all of the system sounds (on a NeXT computer).\n"
   "       concatenate -s 3000 /NextLibrary/Sounds/*.snd all.snd             \n"
   << endl;
}



//////////////////////////////
//
// usage -- how to run the concatenate program on the command line.
//

void usage(const char* command) {
   cout << 
   "                                                                         \n"
   "Strings together any number of soundfiles sequentially into one output   \n"
   "file.  Blank space can be specified between soundfiles in output.  If    \n"
   "any input sound is a stereo file, then the output soundfile will be      \n"
   "stereo, and any mono files will duplicate their channels into both the   \n"
   "left and right of the output soundfile.                                  \n"
   "                                                                         \n"
   "Usage: " << command << " [-q][-s samples | -t time] insound(s) outsound  \n"
   "                                                                         \n"
   "Options:                                                                 \n"
   "   -s = the number of blank samples to add between input soundfiles      \n"
   "   -t = the time in seconds of blank samples to add between input        \n"
   "        soundfiles.  Overrides the -s option.  Sampling rate of the      \n"
   "        first soundfile is used to calculate blank gap.                  \n"
   "   -q = quiet mode.  Do not display message as each soundfile is read.   \n"
   "   --options = list of all options, aliases and defaults                 \n"
   "                                                                         \n"
   "                                                                         \n"
   << endl;
}



