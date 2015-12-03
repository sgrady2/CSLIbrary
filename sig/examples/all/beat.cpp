//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Jun 26 13:02:06 PDT 1998
// Last Modified: Fri Jul  3 14:17:58 PDT 1998
// Last Modified: Mon Apr 17 14:46:51 PDT 2000 (process incomplete beats)
// Filename:      ...sig/doc/examples/museinfo/humdrum/beat/beat.cpp
// Syntax:        C++; humdrum
//
// Description:   Generates metrical location data for **kern entries
//
// There are two cases when an incomplete measure needs to
//    be counted backwards.  These cases will be handled by
//    the beat program:
//    (1) an initial pickup beat
//    (2) a repeat sign breaks a true measure
//

#include "humdrum.h"
#include <stdlib.h>
#include <iostream.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#ifdef VISUAL
   #include <strstrea.h>    /* for windows 95 */
#else
   #include <strstream.h>
#endif


// function declarations
void     checkOptions(Options& opts, int argc, char* argv[]);
double   determineDuration(const HumdrumRecord& aRecord);
void     example(void);
void     fixIncompleteBars(HumdrumFile& outfile);
void     numberToString(double number, char* string);
void     processRecords(HumdrumFile& infile, HumdrumFile& outfile);
void     usage(const char* command);


// global variables
Options      options;            // database for command-line arguments
double       meterposition = 1;  // position of current event in meter
double       scalevalue = 1;     // for changing duration size (not used yet)
int          init;               // for initializing duration function
int          zero = 0;           // zero offset instead of 1 for first beat


// incomplete measure checking variables
Array<int>     lineC;            // file line number of the current item
Array<double>  sumC;             // current summation of the beat
Array<double>  durationC;        // duration of the current item
Array<double>  beatlocationC;    // beat position of the current item
Array<double>  meterbeatsC;      // number of beats in current meter
Array<double>  timebaseC;        // duration of a beat
Array<double>  errorsC;          // errors in the measure length
				 // note: errorsC is not used yet
int lineOffset = 0;

///////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[]) {
   HumdrumFile infile, outfile;

   // process the command-line options
   checkOptions(options, argc, argv);

   // figure out the number of input files to process
   int numinputs = options.getArgCount();

   for (int i=0; i<numinputs || i==0; i++) {
      init = 1;
      infile.clear();
      outfile.clear();

      // incomplete measure checking variables

      lineC.setSize(10000);
      sumC.setSize(10000);
      durationC.setSize(10000);
      beatlocationC.setSize(10000);
      meterbeatsC.setSize(10000);
      timebaseC.setSize(10000);
      errorsC.setSize(10000);

      lineC.setAllocSize(10000);
      sumC.setAllocSize(10000);
      durationC.setAllocSize(10000);
      beatlocationC.setAllocSize(10000);
      meterbeatsC.setAllocSize(10000);
      timebaseC.setAllocSize(10000);
      errorsC.setAllocSize(10000);

      lineC.setSize(0);
      sumC.setSize(0);
      durationC.setSize(0);
      beatlocationC.setSize(0);
      meterbeatsC.setSize(0);
      timebaseC.setSize(0);
      errorsC.setSize(0);

      lineC.allowGrowth();
      sumC.allowGrowth();
      durationC.allowGrowth();
      beatlocationC.allowGrowth();
      meterbeatsC.allowGrowth();
      timebaseC.allowGrowth();
      errorsC.allowGrowth();


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
         if (!options.getBoolean("keep-nulls")) {
            outfile = outfile.removeNullRecords();
         }
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
   opts.define("a|assemble=b");           // assemble analysis with input
   opts.define("b|beat|base|t|timebase=s:"); // rhythmic unit of one beat
   opts.define("d|dur|duration=b");       // display rhymic duration of records
   opts.define("s|sum=b");                // sum the duration of each measure
   opts.define("z|zero|zero-offset=b");   // first beat is represented by a 0
   opts.define("n|nulls|keep-nulls=b");   // doesn't remove nulls with -s option
   opts.define("debug=b");                // determine bad input line num
   opts.define("author=b");               // author of program
   opts.define("version=b");              // compilation info
   opts.define("example=b");              // example usages
   opts.define("h|help=b");               // short description
   opts.process(argc, argv);
   
   // handle basic options:
   if (opts.getBoolean("author")) {
      cout << "Written by Craig Stuart Sapp, "
           << "craig@ccrma.stanford.edu, May 1998" << endl;
      exit(0);
   } else if (opts.getBoolean("version")) {
      cout << argv[0] << ", version: 17 April 2000" << endl;
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

   if (options.getBoolean("zero")) {
      zero = 1;
   }
}
  


//////////////////////////////
//
// determineDuration -- determines the duration of the **kern
//	entries before a new **kern entry.
//

double determineDuration(const HumdrumRecord& aRecord) {
   static Collection<double> lastdurations;  // for keeping track of durations
   static Collection<double> runningstatus;  // for keeping track current dur

   int i;
   // initialization:
   if (init) {
      init = 0;
      lastdurations.setSize(aRecord.getFieldCount());
      runningstatus.setSize(aRecord.getFieldCount());
      for (i=0; i<aRecord.getFieldCount(); i++) {
         lastdurations[i] = 0;
         runningstatus[i] = 0;
      }
      // remove non-kern spines from note list
      for (i=0; i<aRecord.getFieldCount(); i++) {
         if (aRecord.getExInt(i) != E_KERN_EXINT) {
            lastdurations.setSize(lastdurations.getSize() - 1);
            runningstatus.setSize(runningstatus.getSize() - 1);
         }
      }
   }


   // Step (1): if lastdurations == runningstatus, then zero running
   // status.
   for (i=0; i<runningstatus.getSize(); i++) {
      if (runningstatus[i] == lastdurations[i]) {
         runningstatus[i] = 0;
      }
   }

 
   // Step (2): input new durations into the lastdurations array
   int count = 0;
   for (i=0; i<aRecord.getFieldCount(); i++) {
      if (aRecord.getExInt(i) == E_KERN_EXINT) {
         if (strcmp(aRecord[i], ".") == 0) {
            ; // don't do anything
         } else {
            lastdurations[i] = Convert::kernToDuration(aRecord[i]);
         }
         count++;
      }
   }
   if (count != runningstatus.getSize()) {
      cerr << "Error: spine count has changed" << endl;
   }


   // Step (3): find minimum duration by subtracting last from running
   double min = 99999999;
   double test;
   for (i=0; i<lastdurations.getSize(); i++) {
      test = lastdurations[i] - runningstatus[i];
      if (test < min) {
         min = test;
      }
   }
   

   // Step (4): add the duration to the running values and to meter position
   for (i=0; i<runningstatus.getSize(); i++) {
      runningstatus[i] += min;
   }
   meterposition += min;

   return min;
}


//////////////////////////////
//
// numberToString --
//

void numberToString(double number, char* string) {
   strstream temp;
   temp << number << ends;
   strncpy(string, temp.str(), 64);
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
   "# display the metrical location spine with original data:                \n"
   "     meter -a chor217.krn                                                \n"
   "                                                                         \n"
   << endl;
}



//////////////////////////////
//
// processRecords -- looks at humdrum records and determines the 
//      beat location.
//

void processRecords(HumdrumFile& infile, HumdrumFile& outfile) {
   char   aString[256] = {0};      // for creating strings
   char   aString2[256] = {0};     // for creating strings
   double summation = 0;           // for summing measure duration
   int summationLine = -1;         // for locating summation line in output
   double duration;
   HumdrumRecord tempRecord;       // for *beat: interpretation
   char* slash;                    // for metronome marking
   double measureBeats = 0.0;
   double mmarker = -1.0;
   double temp = 0.0;

   double timebase = 4.0;
   if (strcmp(options.getString("base"), "") != 0) {
      timebase = options.getInteger("base");
 
      // check for prolongation dot.  Ignore any double dots
      if (strchr(options.getString("base"), '.') != NULL) {
         timebase = timebase * 2.0/3.0;
      }
   }

   HumdrumRecord currRecord;
   int value = 0;
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
            // check for time signature
            if (currRecord[0][1] == 'M' && isdigit(currRecord[0][2])) {
               measureBeats = atof(&currRecord[0][2]);
               slash = strchr(currRecord[0], '/');
               if (slash != NULL && 
                     strcmp(options.getString("base"), "") == 0) {
                  timebase = atof(&slash[1]);
               }
               
               // create a beat interpretation if manually specified
               if (strcmp(options.getString("base"), "") != 0 &&
                     tempRecord.getFieldCount() == 0) {
                     tempRecord = currRecord;
                  for (int j=0; j<currRecord.getFieldCount(); j++) {
                     tempRecord.changeField(j, "*");
                  }
                  strcpy(aString, "*base:");
                  Convert::durationToKernRhythm(aString2, 4.0/timebase);
                  strcat(aString, aString2);
                  tempRecord.appendField(aString);
                  outfile.appendLine(tempRecord);
                  lineOffset++;
               }
            }

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

            value = i + lineOffset;
            lineC.append(value);
            sumC.append(mmarker);
            durationC.append(mmarker);
            beatlocationC.append(mmarker);
            meterbeatsC.append(mmarker);
            timebaseC.append(mmarker);

            outfile.appendLine(currRecord);
            meterposition = 1;
            if (summationLine != -1 && options.getBoolean("sum")) {
               numberToString(summation, aString);               
               outfile[summationLine].changeField(
                  outfile[summationLine].getFieldCount()-1,
                  aString);
               summationLine = outfile.getNumLines();
            }
            summation = 0;
            break;
         case E_humrec_data_interpretation:
            if (options.getBoolean("duration")) {
               currRecord.appendField("**duration");
            } else if (options.getBoolean("sum")) {
               currRecord.appendField("**beatsum");
            } else {
               currRecord.appendField("**beat");
            }
            outfile.appendLine(currRecord);
            break;
         case E_humrec_data:
            if (summationLine == -1) {
                summationLine = outfile.getNumLines();
            }
            // handle null fields
            if (currRecord.equalFieldsQ("**kern", ".")) {
               currRecord.appendField(".");
               outfile.appendLine(currRecord);
               break;
            }
            if (options.getBoolean("duration")) {
               duration = determineDuration(infile[i]) * timebase / 4.0;
               numberToString(duration, aString);
               currRecord.appendField(aString);
               outfile.appendLine(currRecord);
            } else if (options.getBoolean("sum")) {
               currRecord.appendField(".");
               outfile.appendLine(currRecord);
               duration = determineDuration(infile[i]);
               summation += duration * timebase/4.0;
            } else {
               numberToString(meterposition*timebase/4.0 - 
                  timebase/4.0 + 1 - zero, aString);
               currRecord.appendField(aString);
               outfile.appendLine(currRecord);
               duration = determineDuration(infile[i]);
               summation += duration * timebase/4.0;

               value = i+lineOffset;
               lineC.append(value);
               sumC.append(summation);
               durationC.append(duration);
               temp = meterposition*timebase/4.0 - timebase/4.0;
               beatlocationC.append(temp);
               temp = measureBeats * 4.0 / timebase;
               meterbeatsC.append(measureBeats);
               timebaseC.append(timebase);

            }
            break;
         default:
            cerr << "Error on line " << (i+1) << " of input" << endl;
            exit(1);
      }
   }

   if (!options.getBoolean("sum") && !options.getBoolean("duration")) {
      fixIncompleteBars(outfile);
   }
}



//////////////////////////////
//
// fixIncompleteBars -- resolve when incomplete bars are
//    supposed to be the ends of measures rather than the
//    beginnings of measures.
//

void fixIncompleteBars(HumdrumFile& outfile) {
   int i, k;
   int lastk = 0;
   double lastsum = 0;
   double newsum = 0;
   char newentry[1024] = {0};

   for (i=0; i<lineC.getSize(); i++) { 
      // at each measure line determine one of three
      // cases: (1) all ok (2) partial measure (3) incorrect measure
      if (durationC[i] == -1) {       
        
         // case 1: measure sum is complete
         if (i==0) {
            continue;
         }
         if (sumC[i-1] == meterbeatsC[i-1]) {
            lastsum = meterbeatsC[i-1];
            lastk = i-1;
            continue;
         }

         // incomplete measure encountered determine if it
         // is an error or not.

         // case 2a: start of piece
         if (lastsum == 0) {
            k = i-1;
            newsum = 0;
            while (k >= 0 && sumC[k] != -1) {
               newsum += durationC[k];
               numberToString((meterbeatsC[k] - newsum) 
                   * timebaseC[k]/4.0 + 1 - zero, newentry);
               outfile[lineC[k]].changeField(outfile[lineC[k]].getFieldCount()
                  -1, newentry);
               k--;
            }
            lastk = i-1;
            lastsum = sumC[i-1];
            continue;
         }
         // case 2b: repeat bar or something splitting up
         // a regular bar
         if ((lastsum + sumC[i-1]) == timebaseC[i-1]/4.0 * meterbeatsC[i-1]) {
            k = i-1;
            newsum = 0;
            while (k >= 0 && sumC[k] != -1) {
               newsum += durationC[k];
               numberToString((meterbeatsC[k] - newsum) 
                   * timebaseC[k]/4.0 + 1 - zero, newentry);
               outfile[lineC[k]].changeField(outfile[lineC[k]].getFieldCount()
                  -1, newentry);
               k--;
            }
            lastk = i-1;
            lastsum = sumC[i-1];
         } else {
            lastk = i-1;
            lastsum = sumC[i-1];
         }

          // don't worry about incorrect bars for now
      }
   }
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



