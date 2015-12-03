//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Apr 25 21:19:21 PDT 1998
// Last Modified: Sat Apr 25 21:19:24 PDT 1998
// Filename:      ...sig/doc/examples/sig/sigfile/sndcomment/sndcomment.cpp
// Syntax:        C++; sig
//
// Description:   extracts or replaces a comment in a soundfile.
//

#include "sig.h"
#include <iostream.h>
#include <stdlib.h>

#ifdef VISUAL
   #include <strstrea.h>
#else 
   #include <strstream.h>
#endif

void  checkOptions(Options& opts);
void  example(void);
void  usage(const char* command);
char* getcomment(const char* filename);


///////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
   Options options(argc, argv);
   checkOptions(options);
  
   SoundHeader header(options.getArg(1));
   char* comment;
 
   if (strlen(options.getString("comment-file")) != 0) {
      comment = getcomment(options.getString("comment-file"));
   } else {
      comment = new char[strlen(options.getString("comment")) + 1];
      strcpy(comment, options.getString("comment"));
   }
      
   header.setComment(comment);

   int numSamples = header.getSamples() * header.getChannels();

   // Elements:
   SoundFileIn  insound(options.getArg(1));
   SoundFileOut outsound(options.getArg(2), header);

   // Connections:

   for (int i=0; i<header.getChannels(); i++) {
      outsound.connect(insound, i, i);
   }

   Action action;
   action.tick(outsound, numSamples);

   delete [] comment;
   return 0;
}


///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// checkOptions -- handle command-line options.
//

void checkOptions(Options& opts) {
   opts.define("c|comment=s");
   opts.define("f|file|comment-file=s");
   opts.define("e|extract=b");
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

   if (opts.getBoolean("extract")) {
      SoundHeader header(opts.getArg(1));
      cout << header.getComment() << endl;
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
// example -- gives example calls to the dcblock program.
//

void example(void) {
   cout <<
   "# sndcomment examples:                                                   \n"
   "       sndcomment -c \"This is a comment\" in.snd out.snd                \n"
   << endl;
}



//////////////////////////////
//
// usage -- how to run the osc program on the command line.
//

void usage(const char* command) {
   cout << 
   "                                                                         \n"
   "Sets a comment in the soundfile header.  Doesn't work with               \n"
   "                                                                         \n"
   "Usage: " << command << " [-c comment][-f file][-e] insound outsound      \n"
   "                                                                         \n"
   "Options:                                                                 \n"
   "   -c = comment string                                                   \n"
   "   -f = comment file.  overrides the -c option                           \n"
   "   -e = extracts the comment from the input soundfile                    \n"
   "   --options = list of all options, aliases and defaults \n"
   << endl;
}



//////////////////////////////
//
// getcomment -- gets a comment from a text file.
//

char* getcomment(const char* filename) {
   fstream  infile(filename, ios::nocreate | ios::in);
   
   // figure out how many bytes in file:
   infile.seekg(0, ios::end);
   int size = infile.tellg();
   infile.seekg(0, ios::beg);

   char* output = new char[size + 1];
   output[size] = '\0';

   infile.read(output, sizeof(char) * size);
   return output;
}



