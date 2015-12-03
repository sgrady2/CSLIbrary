//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat May 23 21:08:48 PDT 1998
// Last Modified: Fri Jul  3 14:18:04 PDT 1998
// Filename:      ...sig/doc/examples/all/base40/base40.cpp
// Syntax:        C++ 
// Reference:     http://www.ccarh.org/publications/reprints/base40
//
// Description:   Converts **kern pitches into/from base 40 system.
//

#include "humdrum.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// function declarations
void     checkOptions(Options& opts, int argc, char* argv[]);
void     processDataRecord(HumdrumRecord& line);
void     processExclusiveInterp(HumdrumRecord& line);
void     processTandemRecord(HumdrumRecord& line);
void     splitstring(const char* line, char* front, char* basestring, 
             char* back, const char* markstart, const char* markend);
void     kernsplit(const char* line, char* front, char* back, 
             const char* markstart, const char* markend);

// global variables
Options      options;            // database for command-line arguments
int          octaveQ = 1;        // boolean for keeping or removing octave info
int          transpose = 0;      // transposition value from command-line
int          preserveQ = 0;      // boolean for keeping extra kern info
int          kernQ = 1;          // boolean for converting **base40 to **kern
int          cvalue = 2;         // default pitch class for C
int          noptionQ = 0;       // for -n option: convert to specific key
int          mode = 0;           // for -n option
char         newkey[32] = {0};   // for -n option
const char*  markstart = "";     // start of base40 number marker
const char*  markend   = "";     // end of base40 number marker


///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   // process the command-line options
   checkOptions(options, argc, argv);

   int numinputs = options.getArgCount();
   HumdrumFile input;

   for (int j=0; j<numinputs || j==0; j++) {
      input.clear();

      // if no command-line arguments read data file from standard input
      if (numinputs < 1) {
         input.read(cin);
      } else {
         input.read(options.getArg(j+1));
      }

      for (int i=0; i<input.getNumLines(); i++) {
         switch (input[i].getType()) {
            case E_humrec_none:
            case E_humrec_empty:
            case E_humrec_comment:
            case E_humrec_global_comment:
            case E_humrec_data_comment:
            case E_humrec_data_tandem:
               if (noptionQ) {
                  processTandemRecord(input[i]);
               }
               cout << input[i] << endl;
               break;
            case E_humrec_data_kern_measure:
               cout << input[i] << endl;
               break;
            case E_humrec_data_interpretation:
               processExclusiveInterp(input[i]);
               cout << input[i] << endl;
               break;
            case E_humrec_data:
               processDataRecord(input[i]);
               cout << input[i] << endl;
               break;
            default:
               break;
         }
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
   opts.define("p|pitch-class=b",  "show only base40 pitch classes");
   opts.define("r|rhythm=b",       "preserve **kern rhythms and other info");
   opts.define("c|c-value=i:2",    "base 40 pitch class value for C");
   opts.define("t|transpose=i:0",  "transposition interval");
   opts.define("k|kern=b",         "convert **base40 spines to **kern");
   opts.define("n|newkey=s",       "transpose **kern spines to newkey");
   opts.define("s|start=s:a",      "marker string start for **kern preserving");
   opts.define("e|end=s:a",        "marker string end for **kern preserving");
   opts.define("author=b",         "show the author of program");
   opts.define("version=b",        "compilation info");
   opts.define("example=b",        "example usages");
   opts.define("h|help=b",         "short description");
   opts.process(argc, argv);
   
   // handle basic options:
   if (opts.getBoolean("author")) {
      cout << "Written by Craig Stuart Sapp, "
           << "craig@ccrma.stanford.edu, April 2000" << endl;
      exit(0);
   } else if (opts.getBoolean("version")) {
      cout << argv[0] << ", version: 11 April 2000" << endl;
      cout << "compiled: " << __DATE__ << endl;
      cout << HUMDRUM_VERSION << endl;
      exit(0);
   } else if (opts.getBoolean("help")) {
//      usage(opts.getCommand());
      exit(0);
   } else if (opts.getBoolean("example")) {
//      example();
      exit(0);
   }

   if (opts.getBoolean("pitch-class")) {
      octaveQ = 0;
   } else {
      octaveQ = 1;
   }

   // get the transposition interval
   transpose = opts.getInteger("transpose");   

   // get kern convertion option
   kernQ = opts.getBoolean("kern");   

   // get the base40 marker start/stop strings
   markstart = opts.getString("start");
   markend   = opts.getString("end");

   // get boolean for keeping extra kern data.
   preserveQ = opts.getBoolean("rhythm");

   // get boolean for converting from base40 to kern
   kernQ = opts.getBoolean("kern");

   // get C value offset
   cvalue = opts.getInteger("c-value");
   cvalue -= 2;

   if (opts.getBoolean("newkey")) {
      noptionQ = 1;
      transpose = 0;
      strncpy(newkey, opts.getString("newkey"), 30);
      if (isupper(newkey[0])) {
         mode = 0;  // Major
      } else {
         mode = 1;  // Minor
      }
      int i = 0;
      while (newkey[i] != '\0') {
         if (newkey[i] == 's') {
            newkey[i] = '#';
         }
         i++;
      }
   }


}



//////////////////////////////
//
// example -- example usage of the base40 program
//

void example(void) {
   cout <<
   "                                                                         \n"
   "no examples yet                                                          \n"
   "                                                                         \n"
   << endl;
}



//////////////////////////////
//
// kernsplit -- split up the kern into stuff before and stuff 
//   after a pitch value.
//

void kernsplit(const char* line, char* front, char* back, 
      const char* markstart, const char* markend) {
   int length = strlen(line);

   char *templine;
   templine = new char[length+1];
   strcpy(templine, line);

   char* pointer;
   pointer = strtok(templine, "abcdefgABCDEFG#-n");
   if (pointer != NULL) {
      strcpy(front, pointer);
   } else {
      front[0] = '\0';
   }
   pointer = strtok(NULL, "abcdefgABCDEFG#-n");
   if (pointer != NULL) {
      strcpy(back, pointer);
   } else {
      back[0] = '\0';
   }

   delete [] templine;
}



//////////////////////////////
//
// processDataRecord -- example usage of the base40 program
//

void processDataRecord(HumdrumRecord& line) {
   int k;
   int base40value;
   static char buffer[1024] = {0};
   static char pitchbuffer[128] = {0};
   static char front[128] = {0};
   static char back[128] = {0};
   static char basestring[128] = {0};

   if (kernQ) {
      // converting from **base40 to **kern

      for (k=0; k<line.getFieldCount(); k++) {
         if (strcmp(line.getExIntString(k), "**base40") == 0) {
            if (strcmp(line[k], ".") == 0) {
               continue;
            }
            splitstring(line[k], front, basestring, back, markstart, markend);
            base40value = atoi(basestring);
            base40value += transpose - cvalue;
            Convert::base40ToKern(pitchbuffer, base40value);
            strcpy(buffer, front);
            strcat(buffer, pitchbuffer);
            strcat(buffer, back);

            line.changeField(k, buffer);
         }
      }

   } else  if (noptionQ) {
//////////////////////////////////////////

   } else {

      // converting from **kern to **base40
      for (k=0; k<line.getFieldCount(); k++) {
         if (strcmp(line.getExIntString(k), "**kern") == 0) {
            if (strcmp(line[k], ".") == 0) {
               continue;
            }
            base40value = Convert::kernToBase40(line[k]);
            base40value += transpose + cvalue;
            if (preserveQ) {
               kernsplit(line[k], front, back, markstart, markend);
               if (base40value < -100) {
                  line.changeField(k, "r");
                  pitchbuffer[0] = 'r';
                  pitchbuffer[1] = '\0';
               } else {
                  if (octaveQ == 0) {
                     base40value = base40value % 40;
                  }
                  sprintf(pitchbuffer, "%d", base40value);
               }
               sprintf(buffer, "%s%s%s%s%s", front, markstart, 
                     pitchbuffer, markend, back);
               line.changeField(k, buffer);
            } else {
               if (base40value < -100) {
                  line.changeField(k, "r");
               } else {
                  if (octaveQ == 0) {
                     base40value = base40value % 40;
                  }
                  sprintf(buffer, "%d", base40value);
                  line.changeField(k, buffer);
               }
            }
         }
      }

   }


}



//////////////////////////////
//
// processExclusiveInterp -- adjust the ** fields as necessary
//

void processExclusiveInterp(HumdrumRecord& line) {
   int k;
   if (kernQ) {
      // converting from **base40 to **base40
      for (k=0; k<line.getFieldCount(); k++) {
         if (strcmp(line[k], "**base40") == 0) {
            line.changeField(k, "**kern");
         }
      }
   } else if (noptionQ) {
      // do nothing, will keep **kern spines, but transpose them.
   } else {
      // converting from **kern to **base40
      for (k=0; k<line.getFieldCount(); k++) {
         if (strcmp(line[k], "**kern") == 0) {
            line.changeField(k, "**base40");
         }
      }
   }
}


//////////////////////////////
//
// processTandemRecord -- handle lines starting with *.
//

void processTandemRecord(HumdrumRecord& line) {
   if (transpose != 0) {
      return;              // don't need to change this more than once
   }

   // find the first **kern spine
   char oldkey[32] = {0};
   char oldkeytemp[32] = {0};
   char *tempointer = NULL;
   int length = 0;
   int k;
   for (k=0; k<line.getFieldCount(); k++) {
      if (strcmp("**kern", line.getExIntString(k)) == 0) {
         strncpy(oldkey, line[k], 30);
         break;
      }
   }

   length = strlen(oldkey);
   strcpy(oldkeytemp, oldkey);
   tempointer = strtok(oldkeytemp, "*abcdefgABCDEFG#-:");
   if (tempointer == NULL || strcmp(tempointer, "") == 0) {
      // do nothing
   } else {
      return;     // not the tandem interp that was wanted.
   }

   if (oldkey[length-1] != ':') {
      return;
   }

   int base40note = Convert::kernToBase40(oldkey);

cout << "++++++++ Key value is: " << base40note << endl;


}


//////////////////////////////
//
// splitstring -- split the kern field into three sections:
//   (1) before the pitch, (2) the pitch, and (3) after the pitch.
//

void splitstring(const char* line, char* front, char* basestring, char* back, 
      const char* markstart, const char* markend) {

   int length = strlen(line);
   char *templine;
   templine = new char[length+1];
   strcpy(templine, line);

   int lengths = strlen(markstart);
   int lengthe = strlen(markend);
   
   char *startp, *endp;

   startp = strstr(templine, markstart);
   if (startp != NULL) {
      endp = strstr(&startp[1], markend);
   } else {
      endp = NULL;
   }

   // if there is no marker to preserve kern 
   if (startp == NULL && endp == NULL) {
      front[0] = '\0';
      back[0] = '\0';
      strcpy(basestring, line);
      delete [] templine;
      return;
   }

   // something weird may have happened, send all ouptut to middle string
   if (startp == NULL || endp == NULL) {
      front[0] = '\0';
      back[0] = '\0';
      strcpy(basestring, line);
      delete [] templine;
      return;
   }

   // both the start and the end marker were found so split up the 
   // string into three pieces

   startp[0] = '\0'; 
   endp[0] = '\0'; 
   strcpy(front, templine);
   strcpy(basestring, &startp[lengths]);
   strcpy(back, &endp[lengthe]);

   delete [] templine;
}



//////////////////////////////
//
// usage -- gives the usage statement for the quality program
//

void usage(const char* command) {
   cout <<
   "Extracts pitch information from <tt>**kern</tt> spines and converts them \n"
   "to a base-40 representation which preserves interval qualities between   \n"
   "pitches.                                                                 \n"
   "                                                                         \n"
   "Usage: " << command << " [-r][-p] input                                  \n"
   "                                                                         \n"
   "Options:                                                                 \n"
   "   -r = keep **kern rhythms and other kern information.                  \n"
   "   -p = convert to base40 pitch class, not asolute pitch.                \n"
   "   -k = convert from **base40 to **kern                                  \n"
   "   --options = list of all options, aliases and default values           \n"
   "                                                                         \n"
   << endl;
}




