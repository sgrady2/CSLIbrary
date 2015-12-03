//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Jun 26 23:44:15 PDT 1998
// Last Modified: Fri Jun 26 23:44:19 PDT 1998
// Filename:      ...sig/doc/examples/museinfo/humdrum/harmanal/harmanal.cpp
// Syntax:        C++; humdrum
//
// Description:   Analyzes **kern data and generates a functional
//		  harmony analysis of timeslices (no melodic harmony).
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
Options      options;             // database for command-line arguments
char         unknown[256] = {0};  // space for unknown chord simplification
int          chordinit;           // for initializing chord detection function
int          key = E_root_C;      // base pitch of key to analyze in
int          mode = E_mode_major; // mode of the key


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
      if (options.getBoolean("extract")) {
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
   opts.define("r|riem|riemann=b");             // show **riem analysis
   opts.define("x|e|extract=b");                // display analysis only
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
           << "craig@ccrma.stanford.edu, June 1998" << endl;
      exit(0);
   } else if (opts.getBoolean("version")) {
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

   ChordQuality::setDisplay(opts.getString("format"));
   Convert::chordType.setNullName(opts.getString("unknown"));
   Convert::chordInversion.setNullName(opts.getString("unknown"));
   Convert::chordRoot.setNullName(opts.getString("unknown"));

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
// determineFunctionalAnalysis -- mode 0 = major, mode 1 = minor
//	default value: strlen = 32
//

void determineFunctionalAnalysis(char* output, const ChordQuality& aQuality, 
   int aKey, int aMode, int strlen = 32) {

   if (aQuality.getType() == E_unknown) {
      strncpy(output, "X", strlen);
      return;
   } else if (aQuality.getType() == E_chord_note) {
      strncpy(output, "X", strlen);
      return;
   }

   int rootInKeyQ = 0;
   int rightKeyQualityQ = 1;
   int degreeLocation = 0;
   Collection<int> chordNotes;
   Collection<int> scaleDegrees;

   chordNotes = Convert::chordQualityToNoteSet(aQuality);
   scaleDegrees = Convert::keyToScaleDegrees(aKey, aMode);

   // question: is root in key?
   int i;
   for (i=0; i<scaleDegrees.getSize(); i++) {
      if (chordNotes[0] == scaleDegrees[i]) {
         rootInKeyQ = 1;
         degreeLocation = i;
         break;
      }
   }

   // question: are chord notes in the key?
   for (i=0; i<chordNotes.getSize(); i++) {
      if (chordNotes[i] != scaleDegrees[(2*i+degreeLocation) %
            scaleDegrees.getSize()] ) {
         rightKeyQualityQ = 0;
         break;
      }
   }
 

   int chordThird = 0;
   int chordFifth = 0;
   if (rightKeyQualityQ) {
      if (chordNotes.getSize() > 1) {
         if (chordNotes[0] < chordNotes[1]) {
            chordThird = chordNotes[1] - chordNotes[0];
         } else {
            chordThird = chordNotes[0] - chordNotes[1];
         }
      } else {
         strncpy(output, "X", strlen);
         return;
      }
      if (chordNotes.getSize() > 2) {
         if (chordNotes[0] < chordNotes[2]) {
            chordFifth = chordNotes[2] - chordNotes[0];
         } else {
            chordFifth = chordNotes[0] - chordNotes[2];
         }
      }
      
      // can now build a chord in the key
      switch (degreeLocation) {
         case 0:
            strcpy(output, "I");
            break;
         case 1:
            strcpy(output, "II");
            break;
         case 2:
            strcpy(output, "III");
            break;
         case 3:
            strcpy(output, "IV");
            break;
         case 4:
            strcpy(output, "V");
            break;
         case 5:
            strcpy(output, "VI");
            break;
         case 6:
            strcpy(output, "VII");
            break;
         default:
            strncpy(output, "X", strlen);
            return;
      }
    
      // adjust for major/minor third
      i = 0;
      while (output[i] != '\0') {
         output[i] = tolower(output[i]);
      }
   
      if (chordFifth == E_base40_dim5 || (chordFifth == 0 &&
           aQuality.getType() == E_chord_incmin)) {
        strcat(output, "o");
      } else if (chordFifth == E_base40_aug5) {
        strcat(output, "+");
      }

      if (chordNotes.getSize() == 4) {
         if (degreeLocation == 6) {
            strcat(output, "0");
         }
         strcat(output, "7");
      }

      switch (aQuality.getInversion()) {
         case 0:
            break;
         case 1:
            strcat(output, "a");
            break;
         case 2:
            strcat(output, "b");
            break;
         case 3:
            strcat(output, "c");
            break;
         case 4:
            strcat(output, "d");
            break;
         default:
            strcat(output, "x");
      }
      return;
   }

   else {
      strncpy(output, "X", strlen);
      return;
   }

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
   "# display only the chord analysis:                                       \n"
   "     quality -x chor217.krn                                              \n"
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
            quality = determineChordQuality(infile[i]);
            determineFunctionalAnalysis(aString, quality, key, mode);
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
// usage -- gives the usage statement for the harmanal program
//

void usage(const char* command) {
   cout <<
   "                                                                         \n"
   "Places a **qual spine into humdrum data which analyzes the chord quality \n"
   "of **kern spines.  Currently, input spines cannot split or join.         \n"
   "                                                                         \n"
   "                                                                         \n"
   "Usage: " << command << "[-x][-t|-r|-i] [input1 [input2 ...]][output]     \n"
   "                                                                         \n"
   "Options:                                                                 \n"
   "   -x = extract the **qual analysis spine to output only                 \n"
   "   --options = list of all options, aliases and default values           \n"
   "                                                                         \n"
   << endl;
}



