//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat May 23 21:08:48 PDT 1998
// Last Modified: Fri Jul  3 14:18:04 PDT 1998
// Filename:      ...sig/doc/exmaples/museinfo/humdrum/quality/quality.cpp
// Syntax:        C++; humdrum
//
// Description:   Analyzes **kern data for timeslice chord qualities
//

#include "humdrum.h"
#include <stdlib.h>
#include <iostream.h>
#include <ctype.h>
#include <string.h>


// function declarations
void         checkOptions(Options& opts, int argc, char* argv[]);
ChordQuality determineChordQuality(const HumdrumRecord& aRecord);
void         example(void);
void         processRecords(HumdrumFile& infile, HumdrumFile& outfile);
void         usage(const char* command);


// global variables
Options      options;            // database for command-line arguments
char         unknown[256] = {0}; // space for unknown chord simplification
int          chordinit;          // for initializing chord detection function


///////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[]) {
   HumdrumFile infile, outfile;

   // process the command-line options
   checkOptions(options, argc, argv);

   // figure out the number of input files to process
   int numinputs = options.getArgCount();

   for (int i=0; i<numinputs || i==0; i++) {
      chordinit = 1;
      infile.clear();
      outfile.clear();

      // if no command-line arguments read data file from standard input
      if (numinputs < 1) {
         infile.read(cin);
      } else {
         infile.read(options.getArg(i+1));
      }

      // analyze the input file according to command-line options
      processRecords(infile, outfile);

      // check to see if only the analysis spine is required
      if (!options.getBoolean("assemble")) {
         outfile = outfile.extract(-1);
      }

      outfile.write(cout);
   }

   return 0;
}


///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// checkOptions -- validate and process command-line options.
//

void checkOptions(Options& opts, int argc, char* argv[]) {
   opts.define("t|type=b");                     // show only chord type
   opts.define("i|inversion=b");                // show only chord inversion
   opts.define("r|root=b");                     // show only chord root
   opts.define("a|assemble=b");                 // assemble analysis with input
   opts.define("f|format=s:t:i:r");             // control display style
   opts.define("u|unknown=s:X");                // control display of unknowns
   opts.define("d|debug=b");                    // determine bad input line num
   opts.define("author=b");                     // author of program
   opts.define("version=b");                    // compilation info
   opts.define("example=b");                    // example usages
   opts.define("h|help=b");                     // short description
   opts.process(argc, argv);
   
   // handle basic options:
   if (opts.getBoolean("author")) {
      cout << "Written by Craig Stuart Sapp, "
           << "craig@ccrma.stanford.edu, May 1998" << endl;
      exit(0);
   } else if (opts.getBoolean("version")) {
      cout << argv[0] << ", version: 3 July 1998" << endl;
      cout << "compiled: " << __DATE__ << endl;
      cout << HUMDRUM_VERSION << endl;
      exit(0);
   } else if (opts.getBoolean("help")) {
      usage(opts.getCommand());
      exit(0);
   } else if (opts.getBoolean("example")) {
      example();
      exit(0);
   }

   if (opts.getBoolean("root")) {
      ChordQuality::setDisplay("r");
   } else if (opts.getBoolean("type")) {
      ChordQuality::setDisplay("t");
   } else if (opts.getBoolean("inversion")) {
      ChordQuality::setDisplay("i");
   } else {
      ChordQuality::setDisplay(opts.getString("format"));
   }

   Convert::chordType.setNullName(opts.getString("unknown"));
   Convert::chordInversion.setNullName(opts.getString("unknown"));
   Convert::kernPitchClass.setNullName(opts.getString("unknown"));

   strncpy(unknown, opts.getString("unknown"), 64);
   strcat(unknown, ":");
   strncat(unknown, opts.getString("unknown"), 64);
   strcat(unknown, ":");
   strncat(unknown, opts.getString("unknown"), 64);

}



//////////////////////////////
//
// determineChordQuality -- extracts notes from humdrum data record
//	and sends them to a chord identifier.  Notes from previous
//	records are remembered, and replace any null records in the
//	data.  Rests are represented by some negative value 
//	and will be ignored by the chord identifier.
//

ChordQuality determineChordQuality(const HumdrumRecord& aRecord) {
   static Collection<int> lastnotes;  // for keeping track of null record notes
   int i;
   if (chordinit) {
      chordinit = 0;
      lastnotes.setSize(aRecord.getFieldCount());
      for (i=0; i<aRecord.getFieldCount(); i++) {
         lastnotes[i] = E_root_rest;
      }
      // remove non-kern spines from note list
      for (i=0; i<aRecord.getFieldCount(); i++) {
         if (aRecord.getExInt(i) != E_KERN_EXINT) {
            lastnotes.setSize(lastnotes.getSize() - 1);
         }
      }
   }

   // determine chord notes and send to chord identifier
   Collection<int> currentNotes(lastnotes.getSize());
   int count = 0;
   for (i=0; i<aRecord.getFieldCount(); i++) {
      if (aRecord.getExInt(i) == E_KERN_EXINT) {
         if (strcmp(aRecord[i], ".") == 0) {
            currentNotes[count] = lastnotes[count];
         } else {
            currentNotes[count] = Convert::kernToBase40(aRecord[i]);
            lastnotes[count] = currentNotes[count];
         }
         count++;
      }
   }

   return Convert::noteSetToChordQuality(currentNotes); 
}



//////////////////////////////
//
// example -- example usage of the quality program
//

void example(void) {
   cout <<
   "                                                                         \n"
   "# example usage of the quality program.                                  \n"
   "# analyze a Bach chorale for chord qualities:                            \n"
   "     quality chor217.krn                                                 \n"
   "                                                                         \n"
   "# display the chord analysis with original data:                         \n"
   "     quality -a chor217.krn                                              \n"
   "                                                                         \n"
   "# display only the roots of chords:                                      \n"
   "     quality -r chor217.krn                                              \n"
   "                                                                         \n"
   << endl;
}



//////////////////////////////
//
// processRecords -- looks at humdrum records and determines chord
//	quality
//

void processRecords(HumdrumFile& infile, HumdrumFile& outfile) {
   ChordQuality quality;

   char aString[256] = {0};
   HumdrumRecord currRecord;
   for (int i=0; i<infile.getNumLines(); i++) {
      if (options.getBoolean("debug")) {
         cout << "processing line " << (i+1) << " of input ..." << endl;
      }
      currRecord = infile[i]; 
      switch (currRecord.getType()) {
         case E_humrec_none:
         case E_humrec_empty:
         case E_humrec_comment:
         case E_humrec_global_comment:
            outfile.appendLine(currRecord);
            break;
         case E_humrec_data_comment:
            if (currRecord.equalFieldsQ("**kern")) {
               currRecord.appendField(currRecord[0]);
            } else {
               currRecord.appendField("!");
            }
            outfile.appendLine(currRecord);
            break;
         case E_humrec_data_tandem:
            if (currRecord.equalFieldsQ("**kern")) {
               currRecord.appendField(currRecord[0]);
            } else {
               currRecord.appendField("*");
            }
            outfile.appendLine(currRecord);
            break;
         case E_humrec_data_kern_measure:
            if (currRecord.equalFieldsQ("**kern")) {
               currRecord.appendField(currRecord[0]);
            } else {
               currRecord.appendField("=");
            }
            outfile.appendLine(currRecord);
            break;
         case E_humrec_data_interpretation:
            currRecord.appendField("**qual");
            outfile.appendLine(currRecord);
            break;
         case E_humrec_data:
            // handle null fields
            if (currRecord.equalFieldsQ("**kern", ".")) {
               currRecord.appendField(".");
               outfile.appendLine(currRecord);
               break;
            }
            quality = determineChordQuality(infile[i]);
            quality.makeString(aString);
            if (strcmp(aString, unknown) == 0 || (
                  quality.getType()==E_chord_note && options.getBoolean("root"))
                  ) {
               strcpy(aString, options.getString("unknown"));
            }
            currRecord.appendField(aString);
            outfile.appendLine(currRecord);
            break;
         default:
            cerr << "Error on line " << (i+1) << " of input" << endl;
            exit(1);
      }
   }
}



//////////////////////////////
//
// usage -- gives the usage statement for the quality program
//

void usage(const char* command) {
   cout <<
   "                                                                         \n"
   "Analyzes **kern data and generates a **qual spine which gives the chord  \n"
   "quality of the **kern spines.  Currently, input spines cannot split or   \n"
   "join.                                                                    \n"
   "                                                                         \n"
   "Usage: " << command << " [-a][-i|-r|-t] [input1 [input2 ...]]            \n"
   "                                                                         \n"
   "Options:                                                                 \n"
   "   -a = assemble the **qual analysis spine with input data               \n"
   "   -i = displays the **qual chord inversion only                         \n"
   "   -r = displays the **qual chord root only                              \n"
   "   -t = displays the **qual chord type only                              \n"
   "   --options = list of all options, aliases and default values           \n"
   "                                                                         \n"
   << endl;
}



