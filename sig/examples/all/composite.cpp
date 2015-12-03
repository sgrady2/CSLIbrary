//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Jul  3 22:16:30 PDT 1998
// Last Modified: Sat Jul  4 11:59:48 PDT 1998
// Filename:      ...sig/doc/examples/museinfo/humdrum/composite/composite.cpp
// Syntax:        C++; humdrum
//
// Description:   Generates composite rhythm data spine from **kern input
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
double       determineDuration(const HumdrumRecord& aRecord);
int          determineRest(const HumdrumRecord& aRecord);
int          determineTie(HumdrumRecord& aRecord);
void         example(void);
void         generateMupOutput(HumdrumFile& aFile);
void         modifyRecordTie(HumdrumRecord& aRecord, int state);
void         numberToString(double number, char* string);
void         processRecords(HumdrumFile& infile, HumdrumFile& outfile);
void         usage(const char* command);


// global variables
Options      options;            // database for command-line arguments
double       meterposition = 1;  // position of current event in meter
double       scalevalue = 1;     // for changing duration size (not used yet)
int          durinit;            // for initializing duration function
int          restinit;           // for initializing rests function
int          tieinit;            // for initializing composite tie detector
int          lastTie = 0;        // for determining tie start/stop
char*        title = "";         // for MUP title


///////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[]) {
   HumdrumFile infile, outfile;

   // process the command-line options
   checkOptions(options, argc, argv);

   // figure out the number of input files to process
   int numinputs = options.getArgCount();

   for (int i=0; i<numinputs || i==0; i++) {
      durinit = restinit = tieinit = 1;
      lastTie = 0;
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

      if (!options.getBoolean("mup")) {
         outfile.write(cout);
      } else {
         generateMupOutput(outfile);
      }
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
   opts.define("r|rest|rests|no-rests=b");// don't indicate rests
   opts.define("m|mup=b");                // determine if to output mup
   opts.define("t|title=s:");             // sets the MUP title
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
   if (durinit) {
      durinit = 0;
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
// determineRest -- extracts notes from humdrum data record
//      and determines if all notes are rests or not.  Notes from previous
//      records are remembered, and replace any null records in the
//      data.  Rests are represented by some negative value.
//

int determineRest(const HumdrumRecord& aRecord) {
   static Collection<int> lastnotes;  // for keeping track of null record notes

   int output = 1;
   int i;
   if (restinit) {
      restinit = 0;
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
         if (currentNotes[count] != E_root_rest) {
            output = 0;
         }
         count++;
      }
   }

   return output;
}

//////////////////////////////
//
// determineTie -- determines if notes in record are all tied.
//

int determineTie(HumdrumRecord& aRecord) {
   static Collection<int> ties;         // for keeping track of notes states
   static Collection<int> sustains;     // for keeping track of note sustains
   static Collection<int> rests;        // for keeping track of notes states

   int i;
   if (tieinit) {
      tieinit = 0;
      ties.setSize(aRecord.getFieldCount());
      sustains.setSize(aRecord.getFieldCount());
      rests.setSize(aRecord.getFieldCount());
      for (i=0; i<aRecord.getFieldCount(); i++) {
         ties[i] = 0;
         sustains[i] = 0;
         rests[i] = 0;
      }
      // remove non-kern spines from note list
      for (i=0; i<aRecord.getFieldCount(); i++) {
         if (aRecord.getExInt(i) != E_KERN_EXINT) {
            ties.setSize(ties.getSize() - 1);
            sustains.setSize(sustains.getSize() - 1);
            rests.setSize(rests.getSize() - 1);
         }
      }
   }

   // determine note states 
   Collection<int> currentTies(ties.getSize());
   Collection<int> currentSustains(sustains.getSize());
   Collection<int> currentRests(rests.getSize());
   int count = 0;
   for (i=0; i<aRecord.getFieldCount(); i++) {
      if (aRecord.getExInt(i) == E_KERN_EXINT) {

         // sustain: check if a continuing note
         if (strcmp(aRecord[i], ".") == 0) {
            currentSustains[count] = 1;
         } else {
            currentSustains[count] = 0;
         }
    
         // tie: check for a tie marking
         if (strchr(aRecord[i], '[') != NULL) {
            currentTies[count] = 1;
         } else if (strchr(aRecord[i], ']') != NULL) {
            currentTies[count] = 0;
         } else {
            currentTies[count] = -1;
         }
         
         // rest: check for a rest
         if (strchr(aRecord[i], 'r') != NULL) {
            currentRests[count] = 1;
         } else {
            currentRests[count] = -1;
         }

         count++;
      }
   }

   // process the findings
   for (i=0; i<ties.getSize(); i++) {
      // resolve rests:
      if (currentRests[i] == -1 && currentSustains[i] == 1) {
         currentRests[i] = rests[i];
      } else if (currentRests[i] == -1) {
         currentRests[i] = 0;
      }

      // resolve ties:
      if (currentTies[i] == -1 && ties[i] == 1) {
         currentTies[i] = 1;
      } else if (currentTies[i] == -1) {
         currentTies[i] = 0;
      }
   }


   // determine output
   Collection<int> results(ties.getSize());
   int tiechange;
   for (i=0; i<ties.getSize(); i++) {
      if (ties[i] == 1 && currentTies[i] == 1) {
         tiechange = 1;
      } else if (ties[i] == 1 && currentTies[i] == 0) {
         tiechange = 1;
      } else {
         tiechange = 0;
      }
      if (tiechange || currentSustains[i] || currentRests[i]) {
         results[i] = 1;
      } else {
         results[i] = 0;
      }
   }
      
   // copy new values onto old values
   for (i=0; i<ties.getSize(); i++) {
      ties[i] = currentTies[i];
      rests[i] = currentRests[i];
      sustains[i] = currentSustains[i];
   }

   int output = 1;
   for (i=0; i<results.getSize(); i++) {
      if (results[i] == 0) {
         output = 0;
         break;
      }
   }

   int testrests = 1;
   for (i=0; i<results.getSize(); i++) {
      if (currentRests[i] == 0) {
         testrests = 0;
         break;
      }
   }

   if (testrests) {
      output = 0;
   }

   return output;
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
   "# example usage of the composite program.                                \n"
   "# analyze a Bach chorale for composite rhythm:                           \n"
   "     composite chor217.krn                                               \n"
   "                                                                         \n"
   "# display the composite spine with original data:                        \n"
   "     composite -a chor217.krn                                            \n"
   "                                                                         \n"
   << endl;
}



////////////////////
//
// generateMupOutput
//

void generateMupOutput(HumdrumFile& aFile) {
   int tietrace = 0;
   char aString[128] = {0};
   int starttest = -1;
   int measurestart = 1;

   cout << "// beat output\n\n";
   cout << "header\n   title (28) \" \"\n   title (14) \"\\f(TI) "
        << "composite rhythm\"\n";
   cout << "   title (18) \"\\f(TB)"; 
   if (strcmp(options.getString("title"), "") != 0) {
      cout << options.getString("title");
   } else if (strcmp(title, "") != 0) {
      cout << title;
   }
   cout << "\"\n\n";
   cout << "score\n   stafflines=1\n   scale=0.75\n   measnum=y\n\n";

   HumdrumFile analysis = aFile.extract(-1);
   HumdrumRecord currRecord;
   for (int i=0; i<analysis.getNumLines(); i++) {
      currRecord = analysis[i]; 
      switch (currRecord.getType()) {
         case E_humrec_none:
         case E_humrec_empty:
         case E_humrec_global_comment:
         case E_humrec_data_comment:
            break;
         case E_humrec_comment:
            break;
         case E_humrec_data_tandem:
            if (currRecord[0][1] == 'M' && isdigit(currRecord[0][2])) {
               cout << "score\n   time=";
               strcpy(aString, &currRecord[0][2]);
               cout << aString << '\n';
               if (strcmp(aString, "1/4") == 0) {
                  cout << "   beamstyle=4\n";
               } else if (strcmp(aString, "2/4") == 0) {
                  cout << "   beamstyle=4,4\n";
               } else if (strcmp(aString, "3/4") == 0) {
                  cout << "   beamstyle=4,4,4\n";
               } else if (strcmp(aString, "4/4") == 0) {
                  cout << "   beamstyle=4,4,4,4\n";
               }
               cout << "\nmusic\n\n";
            }
            break;
         case E_humrec_data_kern_measure:
            measurestart = 1;
            cout << '\n';
            if (starttest != -1) {
            if (currRecord[0][strlen(currRecord[0])-1] == '-') {
               cout << "\ninvisbar\n\n";
            } else if (isdigit(currRecord[0][1])) {
               cout << "\nbar\n\n";
            } else if (currRecord[0][1] == '=') {
               cout << "\nendbar\n\n";
            } else if (strcmp(currRecord[0], "=||") == 0) {
               cout << "\ndblbar\n\n";
            } else if (strcmp(currRecord[0], "=|!") == 0) {
               cout << "\nendbar\n\n";
            } else if (strcmp(currRecord[0], "=!|:") == 0) {
               cout << "\nrepeatstart\n\n";
            } else if (strcmp(currRecord[0], "=:|!") == 0) {
               cout << "\nrepeatend\n\n";
            } else if (strcmp(currRecord[0], "=:!!:") == 0) {
               cout << "\nrepeatboth\n\n";
            } else {
               cout << "\nbar\n\n";
            }
            cout << "music\n\n";
            break;
         case E_humrec_data_interpretation:
            break;
         case E_humrec_data:
            starttest = 1;
            // ignore null fields
            if (strcmp(currRecord[0], ".") == 0) {
               break;
            }

            if (measurestart) {
               measurestart = 0;
               cout << "\n1: ";
            }
 
            if (strchr(currRecord[0], '[') != NULL) {
               tietrace = 1;
            } else if (strchr(currRecord[0], ']') != NULL) {
               tietrace = 0;
            }

            if (strchr(currRecord[0], 'r') != NULL) {
               cout << currRecord[0];
            } else if (strchr(currRecord[0], '[') != NULL) {
               for (unsigned int j=0; j<strlen(currRecord[0]); j++) {
                  if (currRecord[0][j] != '[') {
                     cout << currRecord[0][j];
                  }
               }
               cout << 'e';
            } else if (strchr(currRecord[0], ']') != NULL) {
               for (unsigned int j=0; j<strlen(currRecord[0]); j++) {
                  if (currRecord[0][j] != ']') {
                     cout << currRecord[0][j];
                  }
               }
               cout << 'e';
            } else {
               cout << currRecord[0] << 'e';
            }
            
            if (tietrace) {
               cout << '~';
            }
          
            cout << "; ";

            break;
         default:
            cerr << "Error on line " << (i+1) << " of output" << endl;
            exit(1);
      }
   }
}
}



////////////////////
//
// modifyRecordTie -- adjust the tie for the last data in list
//	state = 1 --> turn on a tie
//	state = 0 --> turn off a tie
//

void modifyRecordTie(HumdrumRecord& aRecord, int state) {
   char temp[128];

   if (state) {                  // starting a tie
      strcpy(temp, "[");
      strcat(temp, aRecord[aRecord.getFieldCount() - 1]);
   } else {
      strcpy(temp, aRecord[aRecord.getFieldCount() - 1]);
      strcat(temp, "]");
   }

   aRecord.changeField(aRecord.getFieldCount()-1, temp);
}



//////////////////////////////
//
// processRecords -- looks at humdrum records and determines chord
//	quality
//

void processRecords(HumdrumFile& infile, HumdrumFile& outfile) {
   char   aString[256] = {0};
   double duration;
   int rest;
   int tie;
   int lastDataRecord = 0;

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
            if (strncmp(currRecord[0], "!!!OTL: ", 8) == 0) {
               title = new char[strlen(currRecord[0])];
               strcpy(title, &currRecord[0][8]);
            } else if (strncmp(currRecord[0], "!!!OTL:", 7) == 0) {
               title = new char[strlen(currRecord[0])];
               strcpy(title, &currRecord[0][7]);
            }
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
            meterposition = 1;
            break;
         case E_humrec_data_interpretation:
            currRecord.appendField("**composite");
            outfile.appendLine(currRecord);
            break;
         case E_humrec_data:

            // handle null fields
            if (currRecord.equalFieldsQ("**kern", ".")) {
               currRecord.appendField(".");
               outfile.appendLine(currRecord);
               break;
            }

            duration = determineDuration(infile[i]);
            Convert::durationToKernRhythm(aString, duration);
            if (!options.getBoolean("no-rests")) {
               rest = determineRest(infile[i]);
               if (rest) {
                  strcat(aString, "r");
               }
            }

            tie = determineTie(infile[i]);
            if (tie == 1 && lastTie == 0) {
               modifyRecordTie(outfile[lastDataRecord], 1);
            } else if (tie == 0 && lastTie == 1) {
               modifyRecordTie(outfile[lastDataRecord], 0);
            }
            lastTie = tie;

            currRecord.appendField(aString);
            outfile.appendLine(currRecord);
          
            lastDataRecord = i;
            break;
         default:
            cerr << "Error on line " << (i+1) << " of input" << endl;
            exit(1);
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
   "Analyzes **kern data and generates a composite rhythm spine from the     \n"
   "input **kern data records.  Currently, input spines cannot split or join.\n"
   "                                                                         \n"
   "Usage: " << command << " [-a][-c][input1 [input2 ...]]                   \n"
   "                                                                         \n"
   "Options:                                                                 \n"
   "   -a = assemble the composite analysis spine with the input data.       \n"
   "   -r = don't indicate rests in composite rhythm analysis.               \n"
   "   -m = generate mup output instead of humdrum output.                   \n"
   "   --options = list of all options, aliases and default values           \n"
   "                                                                         \n"
   << endl;
}



