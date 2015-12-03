//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Mar  5 13:02:45 PST 2000
// Last Modified: Sun Mar  5 13:02:49 PST 2000
// Filename:      ...sig/doc/examples/sig/sigfile/sndsplit/sndsplit.cpp
// Syntax:        C++; sig
//
// Description:   split a large file into several smaller versions.
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
   int i;

   // Elements
   SoundFileOut *outsound;
   SoundFileIn   insound(options.getArg(1));
   SoundHeader   header(options.getArg(1));

   int channels    = header.getChannels();
   int sampleCount = header.getSamples();
   double srate    = header.getSrate();

   int filelength = 0;
   // determine the sample length of each output sound file
   if (options.getBoolean("duration")) {
      filelength = (int)(options.getDouble("duration") * srate);
   } else {
      filelength = options.getInteger("samples");
   }


   double fileCount = (double) sampleCount / filelength;
   char buffer[1024] = {0};
   char nbuffer[1024] = {0};
   char basename[1024] = {0};
   char *extension = NULL;

   strncpy(basename, options.getArg(1), 1000);
   int length = strlen(basename);
   for (i=0; i<length; i++) {
      if (basename[length-i-1] == '.') {
         basename[length-i-1] = '\0';
         extension = &basename[length-i];
         break;
      }
   }
   if (extension == NULL) {
      cout << "Error: invalid soundfile name: " << options.getArg(1) << endl;
      exit(1);
   }
   

   Action action;
   int k;
   for (i=0; i<(int)fileCount; i++) {
      strcpy(buffer, basename);
      strcat(buffer, "-");
      if (fileCount >= 10 && i < 10) {
         strcat(buffer, "0");
      }
      if (fileCount >= 100 && i < 100) {
         strcat(buffer, "0");
      }
      if (fileCount >= 1000 && i < 1000) {
         strcat(buffer, "0");
      }
      sprintf(nbuffer, "%d", i);
      strcat(buffer, nbuffer);
      strcat(buffer, ".");
      strcat(buffer, extension);
      
      outsound = new SoundFileOut(buffer, header);
      for (k=0; k<channels; k++) {
         outsound->connect(insound, k, k);
      }

      if (!options.getBoolean("quiet")) {
         cout << "writing file " << buffer << endl;
      }

      // copy the soundfile to the output soundfile
      action.tick(*outsound, filelength);
      delete outsound;
   }

   int leftover = sampleCount - (int)fileCount * (int)filelength;
   if (leftover > 0) {
      strcpy(buffer, basename);
      strcat(buffer, "-");
      if (fileCount >= 10 && i < 10) {
         strcat(buffer, "0");
      }
      if (fileCount >= 100 && i < 100) {
         strcat(buffer, "0");
      }
      if (fileCount >= 1000 && i < 1000) {
         strcat(buffer, "0");
      }
      sprintf(nbuffer, "%d", i);
      strcat(buffer, nbuffer);
      strcat(buffer, ".");
      strcat(buffer, extension);
      
      outsound = new SoundFileOut(buffer, header);
      for (k=0; k<channels; k++) {
         outsound->connect(insound, k, k);
      }

      if (!options.getBoolean("quiet")) {
         cout << "writing file " << buffer << endl;
      }

      // copy the soundfile to the output soundfile
      action.tick(*outsound, leftover);
      delete outsound;
   }

   return 0;
}



///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// checkOptions -- handle command-line options.
//

void checkOptions(Options& opts) {
   opts.define("d|dur|duration=d");
   opts.define("s|samples=i:44100 samples");
   opts.define("o|overlap=d:0");
   opts.define("q|quiet=b");
   opts.define("author=b");
   opts.define("version=b");
   opts.define("example=b");
   opts.define("help=b");
   opts.process();

   if (opts.getBoolean("author")) {
      cout << "Written by Craig Stuart Sapp, "
           << "craig@ccrma.stanford.edu, 5 March 2000" << endl;
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

   // must have exactly one sound filename
   if (opts.getArgCount() != 1) {
      cout << "Error: need exactly one input soundfile." << endl;
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
   "# sndsplit examples:                                                     \n"
   "#   Splits a soundfile into one second pieces                            \n"
   "       sndsplit -d 1 test.wav                                            \n"
   << endl;
}



//////////////////////////////
//
// usage -- how to run the sndsplit program on the command line.
//

void usage(const char* command) {
   cout << 
   "                                                                         \n"
   "Splits the given soundfile into smaller files.  The output file format   \n"
   "is identical to the given input sound.                                   \n"
   "                                                                         \n"
   "Usage: " << command << " [-d duration][-s samples] insound\n"
   "                                                                         \n"
   "Options:                                                                 \n"
   "   -s = length of the output soundfiles specified in samples             \n"
   "   -d = the time in seconds of output files.  Overrides the -s option.   \n"
   "   -q = quiet mode.  Do not display message as each soundfile is written.\n"
   "   --options = list of all options, aliases and defaults                 \n"
   "                                                                         \n"
   "                                                                         \n"
   << endl;
}



