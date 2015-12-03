//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Jul  9 13:20:28 PDT 1998
// Last Modified: Thu Jul  9 13:20:32 PDT 1998
// Filename:      ...sig/doc/examples/museinfo/humdrum/sample/sample.cpp
// Syntax:        C++; humdrum
//
// Description:   Samples pitches according to the given pattern
//

#include "humdrum.h"
#include <string.h>
#include <ctype.h>


// function declarations
void         checkOptions(Options& opts, int argc, char* argv[]);
void         example(void);
void         processRecords(HumdrumFile& infile, HumdrumFile& outfile);
void         usage(const char* command);


// global variables
Options       options;            // database for command-line arguments

double* sampleDuration;
int     sdSize;

///////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[]) {
   HumdrumFile infile, outfile;

   // process the command-line options
   checkOptions(options, argc, argv);

   // figure out the number of input files to process
   int numinputs = options.getArgCount();

   for (int i=0; i<numinputs || i==0; i++) {
      infile.clear();
      outfile.clear();

      // if no command-line arguments read data file from standard
      // input
      if (numinputs < 1) {
         infile.read(cin);
      } else {
         infile.read(options.getArg(i+1));
      }
      // analyze the input file according to command-line options
      processRecords(infile, outfile);

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
   opts.define("r|rhythm|rhythms|=s:4");  // rhythm to sample notes at
   opts.define("d|divide=b");             // determine if to split input notes
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
      cout << argv[0] << ", version: 9 July 1998" << endl;
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

   // handle specific program options
   Array<double> sDur;    // duration to take note sample at
   sDur.allowGrowth(1);
   sDur.setSize(0);
   char* aDuration;
   char* aString;
   aString = new char[strlen(opts.getString("rhythms")) + 1];
   strcpy(aString, opts.getString("rhythms"));
   aDuration = strtok(aString, " ,;\t\n:");
   double duration;
   while (aDuration != NULL) {
      duration = Convert::kernToDuration(aDuration);
      sDur.append(duration);
      aDuration = strtok(NULL, " ,;\t\n:");
   }
   sDur.allowGrowth(0);
   sdSize = sDur.getSize();
   sampleDuration = new double[sDur.getSize()];

   for (int i=0; i<sDur.getSize(); i++) {
      sampleDuration[i] = sDur[i];
      cout << sDur[i] << " ";
   }
   cout << endl;
}



//////////////////////////////
//
// example -- example usage of the sample program
//

void example(void) {
   cout <<
   "                                                                         \n"
   "# example usage of the sample program.                                   \n"
   "                                                                         \n"
   << endl;
}



//////////////////////////////
//
// processRecords -- 
//

void processRecords(HumdrumFile& infile, HumdrumFile& outfile) {
   char aString[128] = {0};
   char outputString[128] = {0};
   int pitch;
   double timetop = 0.0;
   double timebottom = 0.0;
   double duration = 0.0;
   double measureDuration = 0;
   double meterDuration = 0;
   double outputDuration = 0;
   int durationIndex = 0;
   double currDuration = 0;
   double noteDuration = 0;
   int phraseStart = 0;
   int phraseEnd = 0;
   int doPhraseStart = 1;
   int doPhraseEnd = 1;
   int phraseToggle = 0;

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
            outfile.appendLine(currRecord);
            break;
         case E_humrec_data_tandem:
            if (Identify::kernTimeSignature(currRecord[0])) {
               timetop = Convert::kernTimeSignatureTop(currRecord[0]);
               timebottom = Convert::kernTimeSignatureBottomToDuration(
                     currRecord[0]);
               meterDuration = timetop * timebottom;
            }
            outfile.appendLine(currRecord);
            break;
         case E_humrec_data_kern_measure:
            if (timetop == 0) {
               timetop = 4.0;
               timebottom = 1.0;
            }
            measureDuration = 0;
            outfile.appendLine(currRecord[0]);
            break;
         case E_humrec_data:
            if (Identify::humdrumNullField(currRecord[0])) {
               break;
            }
            duration = Convert::kernToDuration(currRecord[0]);
            currDuration += duration;
            pitch = Convert::kernToBase40(currRecord[0]);
            noteDuration = 0;
            doPhraseEnd = 1;
            doPhraseStart = 1;
            if (outputDuration < currDuration) {
               while (outputDuration < currDuration) {
                  noteDuration += sampleDuration[durationIndex % sdSize];
                  outputDuration += sampleDuration[durationIndex % sdSize];
                  if (strchr(currRecord[0], '{') != NULL) {
                     phraseStart = 1;
                  }
                  if (strchr(currRecord[0], '}') != NULL) {
                     phraseEnd = 1;
                  }
                  durationIndex++;
                  if (options.getBoolean("divide")) {
                     strcpy(outputString, "");
                     if (phraseToggle == 0 && phraseStart && doPhraseStart) {
                        strcat(outputString, "{");
                        phraseStart = 0;
                        doPhraseStart = 0;
                        phraseToggle = 1;
                     }
                     strcat(outputString, Convert::durationToKernRhythm(
                           aString, sampleDuration[durationIndex % sdSize]));
                     strcat(outputString, Convert::base40ToKern(aString, 
                           pitch));
                     if (outputDuration >= currDuration && phraseEnd && 
                           doPhraseEnd && phraseToggle == 1) {
                        strcat(outputString, "}");
                        phraseEnd = 0;
                        doPhraseEnd = 0;
                        phraseToggle = 0;
                     }
                     outfile.appendLine(outputString);
                  }
               }
               if (!options.getBoolean("divide")) {
                  strcpy(outputString, "");
                  if (phraseStart && phraseToggle == 0) {
                     strcat(outputString, "{");
                     phraseStart = 0;
                     phraseToggle = 1;
                  }
                  strcat(outputString, Convert::durationToKernRhythm(aString, 
                        noteDuration));
                  strcat(outputString, Convert::base40ToKern(aString, pitch));
                  if (phraseEnd && phraseToggle == 1) {
                     strcat(outputString, "}");
                     phraseEnd = 0;
                     phraseToggle = 0;
                  }
                  outfile.appendLine(outputString);
               }
            } else {
               if (strchr(currRecord[0], '{') != NULL) {
                  phraseStart = 1;
               }
               if (strchr(currRecord[0], '}') != NULL) {
                  phraseEnd = 1;
               }
            }
            break;
         case E_humrec_data_interpretation:
            outfile.appendLine(currRecord[0]);
            break;
         default:
            cerr << "Error on line " << (i+1) << " of input" << endl;
            exit(1);
      }
   }
}



//////////////////////////////
//
// usage -- gives the usage statement for the sample program
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
   "   -b = set the base rhythm for analysis to specified kern rhythm value. \n"
   "   -d = gives the duration of each kern record in beat measurements.     \n"
   "   -s = sum the beat count in each measure.                              \n"
   "   --options = list of all options, aliases and default values           \n"
   "                                                                         \n"
   << endl;
}


