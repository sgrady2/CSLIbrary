//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Jul  7 22:29:32 PDT 1998
// Last Modified: Tue Jul  7 22:29:36 PDT 1998
// Filename:      ...sig/doc/examples/museinfo/humdrum/ana/ana.cpp
// Syntax:        C++; humdrum
//
// Description:   Generates and ESAC-like analysis summary
//

#include "humdrum.h"
#include <stdlib.h>
#include <iostream.h>
#include <ctype.h>
#include <string.h>

#ifdef VISUAL
   #include <strstrea.h>    /* for windows 95 */
#else
   #include <strstream.h>
#endif


// function declarations
void         checkOptions(Options& opts, int argc, char* argv[]);
void         example(void);
void         processRecords(HumdrumFile& infile);
void         usage(const char* command);


// global variables
Options      options;            // database for command-line arguments


///////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[]) {
   HumdrumFile infile;

   // process the command-line options
   checkOptions(options, argc, argv);

   // figure out the number of input files to process
   int numinputs = options.getArgCount();

   for (int i=0; i<numinputs || i==0; i++) {
      infile.clear();

      // if no command-line arguments read data file from standard input
      if (numinputs < 1) {
         infile.read(cin);
      } else {
         infile.read(options.getArg(i+1));
         cout << "\n\nFilename: " << options.getArg(i+1) << endl;
      }
      // analyze the input file according to command-line options
      processRecords(infile);
   }

   return 0;
}


///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// checkOptions -- validate and process command-line options.
//

void checkOptions(Options& opts, int argc, char* argv[]) {
   opts.define("s|spine|c|column=i:0");   // spine to analyze
   opts.define("debug=b");                // determine bad input line num
   opts.define("author=b");               // author of program
   opts.define("version=b");              // compilation info
   opts.define("example=b");              // example usages
   opts.define("h|help=b");               // short description
   opts.process(argc, argv);
   
   // handle basic options:
   if (opts.getBoolean("author")) {
      cout << "Written by Craig Stuart Sapp, "
           << "craig@ccrma.stanford.edu, July 1998" << endl;
      exit(0);
   } else if (opts.getBoolean("version")) {
      cout << argv[0] << ", version: 7 July 1998" << endl;
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

}
  

//////////////////////////////
//
// example -- example usage of the quality program
//

void example(void) {
   cout <<
   "                                                                         \n"
   "# example usage of the meter program.                                    \n"
   "# analyze a Bach chorale for meter position:                             \n"
   "     meter chor217.krn                                                   \n"
   "                                                                         \n"
   "                                                                         \n"
   << endl;
}



//////////////////////////////
//
// processRecords -- looks at humdrum records and determines chord
//	quality
//

void processRecords(HumdrumFile& infile) {
   char aString[128] = {0};
   int lastBase40Pitch = 0x7fffffff;
   int base40Pitch = 0x7fffffff;
   int tempNote = 0x7fffffff;
   int lastNote = 0x7fffffff;   
   int phraseCount = 0;
   int phraseQ = 0;
   int phraseBoundary = 1;
   int barCount = -1;
   int spine = options.getInteger("spine");    // the melodic spine to analyze
   int key = 0x7fffffff;
   int mode = 0x7fffffff;


   HumdrumRecord currRecord;

   Array<int> pitches(300);
   pitches.allowGrowth(0);
   pitches.zero();

   Array<int> intervals(300);
   Array<int> breakIntervals(300);
   intervals.allowGrowth(0);
   intervals.zero();
   breakIntervals.allowGrowth(0);
   breakIntervals.zero();

   Array<int> phraseEndings;
   phraseEndings.allowGrowth();
   phraseEndings.setSize(0);


   for (int i=0; i<infile.getNumLines(); i++) {
      if (options.getBoolean("debug")) {
         cout << "processing line " << (i+1) << " of input ..." << endl;
      }
      currRecord = infile[i]; 
      switch (currRecord.getType()) {
         case E_humrec_none:
         case E_humrec_empty:
         case E_humrec_comment:
             if (strncmp(currRecord[0], "!!!OTL:", 7) == 0) {
                cout << "Title:" << &currRecord[0][7] << endl;
             }
         case E_humrec_global_comment:
            break;
         case E_humrec_data_comment:
            break;
         case E_humrec_data_tandem:
            if (currRecord[spine][strlen(currRecord[spine])-1] == ':') {
               if (strchr("abcdefg", tolower(currRecord[spine][1])) != NULL) {
                  key = Convert::kernToBase40Class(currRecord[spine]);
                  if (islower(currRecord[spine][1])) {
                     mode = 2;
                  } else if (isupper(currRecord[spine][1])) {
                     mode = 1;
                  } else {
                     mode = 0;
                  }
               }
            }
            break;
         case E_humrec_data_kern_measure:
            barCount++;
            break;
         case E_humrec_data_interpretation:
            if (strcmp(currRecord[spine], "**kern") != 0) {
               cerr << "Error: input spine must be in **kern format.  "
                    << "Format is: " << currRecord[spine] << endl;
               exit(1);
            }
            break;
         case E_humrec_data:

            // ignore null records
            if (strcmp(currRecord[spine], ".") == 0) {
               break;
            }

            // extract the pitch
            lastBase40Pitch = base40Pitch;
            lastNote = tempNote;
            tempNote = Convert::kernToBase40(currRecord[spine]);
            if (tempNote != E_base40_rest) {
               base40Pitch = tempNote;
            }
            if (tempNote == E_base40_rest && lastNote != E_base40_rest) {
               pitches[0]++;
            } else if (tempNote == base40Pitch) {
               pitches[base40Pitch]++;
            }
  
            // check for phrase markings
            if (strchr(currRecord[spine], '{') != NULL) {
               if (phraseQ == 0) {
                  phraseQ = 1;
                  phraseBoundary = 1;
                  phraseCount++;
               } else {
                  cerr << "Error: starting a phrase before ending a "
                       << "phrase on line " << i << " of input" << endl;
                  exit(1);
               }
            } else if (strchr(currRecord[spine], '}') != NULL) {
               phraseBoundary = 0;
               if (phraseQ == 1) {
                  phraseQ = 0;
                  phraseEndings.append(base40Pitch);
               } else {
                  cerr << "Error: ending a phrase before starting a "
                       << "phrase on line " << i << " of input" << endl;
                  exit(1);
               }
            } else {
               phraseBoundary = 0;
            }
 
            // check for intervals
            if (lastNote != 0x7fffffff) {
            if (tempNote != E_base40_rest && lastNote != E_base40_rest) {
               if (phraseBoundary) {
                  breakIntervals[tempNote - lastNote + 80]++; 
               } else if (!phraseBoundary) {
                  intervals[tempNote - lastNote + 80]++; 
               }
            } else if (tempNote == E_base40_rest) {
               // do nothing
            } else if (lastNote == E_base40_rest) {
               if (phraseBoundary) {
                  breakIntervals[tempNote - lastNote + 80]++; 
               } else if (!phraseBoundary) {
                  intervals[tempNote - lastNote + 80]++; 
               }
            } else if (lastNote == tempNote && lastNote == E_base40_rest) {
               tempNote = base40Pitch;
            }
            }

            break;
         default:
            cerr << "Error on line " << (i+1) << " of input" << endl;
            exit(1);
      }
   }


   ///// analysis summary //////////////////////////////////////////////////

   /// key information

   cout << "key signature: ";
   switch (mode) {
      case 0:
         cout << " unknown" << endl;
         break;
      case 1:
         cout << Convert::kernPitchClass.getName(key) << " major" 
              << endl;
         break;
      case 2:
         cout << Convert::kernPitchClass.getName(key) << " minor" 
              << endl;
         break;
      default:
         cout << "Error (" << mode << ")" << endl;
   }

   /// pitch information

   int lowindex = 1;
   int highindex = 295;
   while (pitches[lowindex] == 0) {
      lowindex++;
   }
   while (pitches[highindex] == 0) {
      highindex--;
   }
   cout << "Range: " << Convert::base40ToKern(aString, lowindex);
   cout << " to "    << Convert::base40ToKern(aString, highindex) << endl;
   cout << "Pitch frequency: ";
   for (int j=lowindex; j<=highindex; j++) {
      if (pitches[j] != 0) {
         cout << Convert::base40ToKern(aString, j) << ":" 
              << pitches[j];
         if (j != highindex) {
            cout << ", ";
         }
      }
   }
   cout << endl;
   cout << "Rest count: " << pitches[0] << endl;


   /// interval information

   cout << "phrase intervals: " << intervals.sum() <<  endl;
   cout << "   pi-down: " << intervals.sum(0, 79) << " ";
   for (int j=79; j>=0; j--) {
      if (intervals[j] != 0) {
         cout << Convert::intervalNames.getName(-(j-80)) << ":" 
              << intervals[j] << " ";
      } 
   }
   cout << "\n   pi-unisons: " << intervals[80];
   cout << "\n   pi-up: " << intervals.sum(81, 200) << " ";
   for (int j=81; j<80+80; j++) {
      if (intervals[j] != 0) {
         cout << Convert::intervalNames.getName(j-80) << ":" 
              << intervals[j] << " ";
      } 
   }
   cout << endl;

   cout << "phrase boundary intervals : " << breakIntervals.sum() << endl;
   cout << "   pbi-down: " << breakIntervals.sum(1, 79) << " ";
   for (int j=79; j>=0; j--) {
      if (breakIntervals[j] != 0) {
         cout << Convert::intervalNames.getName(-(j-80)) << ":" 
              << breakIntervals[j] << " ";
      } 
   }
   cout << "\n   pbi-unisons: " << breakIntervals[80];
   cout << "\n   pbi-up: " << breakIntervals.sum(81, 200) << " ";
   for (int j=81; j<80+80; j++) {
      if (breakIntervals[j] != 0) {
         cout << Convert::intervalNames.getName(j-80) << ":" 
              << breakIntervals[j] << " ";
      } 
   }
   cout << endl;

   /// phrase information

   cout << "Bar count: " << barCount << endl;
   cout << "Phrase count: " << phraseCount << endl;
   cout << "Phrase endings: ";
   for (int j=0; j<phraseEndings.getSize(); j++) {
      cout << Convert::base40ToKern(aString, phraseEndings[j]);
      if (j != phraseEndings.getSize()-1) {
         cout << ", ";
      }
   }
   cout << endl;


}



//////////////////////////////
//
// usage -- gives the usage statement for the meter program
//

void usage(const char* command) {
   cout <<
   "                                                                         \n"
   "Analyzes **kern data and generates a rhythmic analysis which gives       \n"
   "the beat location of **kern data records in the measure.  Currently,     \n"
   "input spines cannot split or join.                                       \n"
   "                                                                         \n"
   "Usage: " << command << " [-a][-b base-rhythm][-s|-d][input1 [input2 ...]]\n"
   "                                                                         \n"
   "Options:                                                                 \n"
   "   -a = assemble the analysis spine with the input data.                 \n"
   "   -b = set the base rhythm for analysis to specified kern rhythm value. \n"
   "   -d = gives the duration of each kern record in beat measurements.     \n"
   "   -s = sum the beat count in each measure.                              \n"
   "   --options = list of all options, aliases and default values           \n"
   "                                                                         \n"
   << endl;
}



