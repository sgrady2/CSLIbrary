//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu May  6 17:15:41 PDT 2000
// Last Modified: Fri May  5 11:45:28 PDT 2000
// Filename:      ...sig/doc/examples/all/hplay/hplay.cpp
// Syntax:        C++; humdrum
//
// Description:   Play **kern entries in a Humdrum file through MIDI.
//

#include "synthImprov.h"
#include "HumdrumFile.h"
#include "Convert.h"
#include "string.h"

/*----------------- beginning of improvization algorithms ---------------*/

double tempo = 120.0;     // current metronome tempo
SigTimer timer;           // for playing the humdrum file
EventBuffer eventBuffer;  // for storing note offs when playing notes

HumdrumFile data;         // humdrum file to play
int linenum = 0;          // for keeping track of current line in file
double tempoScale = 1.0;  // for global adjustment of tempo
int velocity = 64;        // default velocity to play MIDI notes
int echoTextQ = 1;        // boolean for displaying input file
int fileNumber = 1;       // current file number being played


// non-synthImprov function declarations:

void   checkOptions(void);
void   inputNewFile(void);
void   playdata(HumdrumFile& data, int& linenum, SigTimer& timer);
void   printInputLine(HumdrumRecord& record);
double processNotes(HumdrumRecord& record, int linenum);


/*--------------------- maintenance algorithms --------------------------*/


void description(void) { 
   printboxtop();      
   psl(" HPLAY -- by Craig Stuart Sapp <craig@ccrma.stanford.edu> "
                                                        "-- 4 May 2000\n");
   printboxbottom();
}


void initialization(void) { 
   checkOptions();
   timer.setPeriod(500); 
   timer.reset();
   eventBuffer.setPollPeriod(10);
}


void finishup(void) { }

/*-------------------- main loop algorithms -----------------------------*/

void mainloopalgorithms(void) { 
   eventBuffer.checkPoll();
   if (timer.expired()) {
      playdata(data, linenum, timer);
      if (linenum >= data.getNumLines()) {
         inputNewFile();
      }
   }
}

/*-------------------- triggered algorithms -----------------------------*/

void keyboardchar(int key) { 
   switch (key) {
      case ' ':               // toggle display of file while playing
         echoTextQ = !echoTextQ;
         if (echoTextQ) {
            cout << "!! FILE DISPLAY TURNED ON" << endl;
         } else {
            cout << "!! FILE DISPLAY TURNED OFF" << endl; 
         }
         break;
      case ',':    // slow down tempo
      case '<':
         tempoScale *= 0.95;
         cout << "!! TEMPO SET TO " << (int)(tempo * tempoScale) << endl;
         break;
      case '.':    // speed up tempo
      case '>':
         tempoScale *= 1.05;
         cout << "!! TEMPO SET TO " << (int)(tempo * tempoScale) << endl;
         break;
      case 's':    // silence notes
         eventBuffer.off();
         break;
   }
}


/*------------------ end improvization algorithms -----------------------*/


//////////////////////////////
//
// checkOptions --  process command-line options and setup the
//   humdrum data to play.
//

void checkOptions(void) {
   options.define("t|tempo=d:120.0", "Base tempo");
   options.define("s|tempo-scale=d:1.0", "Tempo scaling factor");
   options.define("p|pause=d:1.0", "pause time in seconds between files");
   options.define("q|quiet=b", "Turn off data echoing while playing");
   options.define("v|velocity=i:64", "Default MIDI key velocity");
   options.process();

   velocity = options.getInteger("velocity");
   tempoScale = options.getDouble("tempo-scale");
   tempo = options.getDouble("tempo");
   if (options.getBoolean("quiet")) {
      echoTextQ = 0;
   }

   if (options.getArgCount() < 1) {
      data.read(cin);
   } else {
      inputNewFile();
   }
}



//////////////////////////////
//
// inputNewFile -- load in a new Humdrum file.
//

void inputNewFile(void) {
   data.clear();
   linenum = 0;    

   int count = options.getArgCount();
   if (fileNumber > count) {
      exit(0);
   }

   data.read(options.getArg(fileNumber));
   data.appendCompositeDuration();

   if (fileNumber > 1) {
      millisleep((float)(1000 * options.getDouble("pause")));
   }
   fileNumber++;
}



//////////////////////////////
//
// playdata -- play the next line of the humdrum file, update the 
//     line number and the time for the next events to be read 
//     from the file.
//

void playdata(HumdrumFile& data, int& linenum, SigTimer& timer) {
   double duration = 0;     // duration of the current line;
   int type = data[linenum].getType();
   while (linenum < data.getNumLines() && duration == 0.0) {
      switch (type) {
         case E_humrec_data:
            duration = processNotes(data[linenum], linenum);
            timer.setPeriod(60000 / tempo / tempoScale * duration);
            timer.reset();
            break;
         case E_humrec_data_tandem:
            // look for a tempo marking
            if (strncmp(data[linenum][0], "*MM", 3) == 0) {
               tempo = atoi(&data[linenum][0][3]);
            }
            break;
         default:
            break;
      }
      if (echoTextQ) {
         printInputLine(data[linenum]);
      }
      linenum++;
      if (linenum < data.getNumLines()) {
         type = data[linenum].getType();
      }
   }
}



//////////////////////////////
//
// printInputLine -- print the current line of the file,
//  omitting the duration field at the end of the line
//

void printInputLine(HumdrumRecord& record) {
   int count = 0;
   int i;
   switch (record.getType()) {
      case E_humrec_none:
      case E_humrec_empty:
      case E_humrec_comment:
      case E_humrec_global_comment:
         cout << record << endl;
         break;
      case E_humrec_data_comment:
      case E_humrec_data_tandem:
      case E_humrec_data_kern_measure:
      case E_humrec_data_interpretation:
      case E_humrec_data:
         count = record.getFieldCount();
         for(i=0; i<count-1; i++) {
            cout << record[i];
            if (i < count-2) {
               cout << '\t';
            }
         }
         cout << endl;
         break;
      default:
         cout << record << endl;
   }
}



//////////////////////////////
//
// processNotes -- play all kern notes in the current record and
//    return the shortest note duration.
//

double processNotes(HumdrumRecord& record, int linenum) {
   NoteEvent note;
   int pitch = 0;
   double duration = 0.0;
   int staccatoQ = 0;
   int accentQ = 0;
   int sforzandoQ = 0;
   int i, j;
   int notecount = 0;
   char buffer[128] = {0};
   for (i=0; i<record.getFieldCount(); i++) {
      if (strcmp(record.getExIntString(i), "**kern") == 0) {
         notecount = record.getTokenCount(i);
         if (strcmp(record[i], ".") == 0) {
            continue;
         }
         for (j=0; j<notecount; j++) {
            record.getToken(buffer, i, j);
            if (strchr(buffer, '[')) {
               // total tied note durations
               duration = data.getTiedDuration(linenum, i, j);
            } else {
               duration = Convert::kernToDuration(buffer);
            }
            pitch = Convert::kernToMidiNoteNumber(buffer); 
            // skip rests
            if (pitch < -100) {
               continue;
            }

            // skip tied notes
            if (strchr(buffer, '_') || strchr(buffer, ']')) {
               continue;
            }

            accentQ = (int)strchr(buffer, '^');
            sforzandoQ = (int)strchr(buffer, 'z');
            staccatoQ = (int)strchr(buffer, '\'');
            note.setChannel(0);
            note.setKey(pitch);
            note.setOnTime(t_time);
            note.setDur(duration * 60000 / tempo / tempoScale);
            if (staccatoQ) {
               note.setDur(0.5 * note.getDur());
            }
            note.setVelocity(velocity);
            if (accentQ) {
               note.setVelocity(note.getVelocity() * 1.3);
            }
            if (sforzandoQ) {
               note.setVelocity(note.getVelocity() * 1.5);
            }
 
            note.activate();
            note.action(eventBuffer);
            eventBuffer.insert(note);
         }
      }
   }

   return strtod(record[record.getFieldCount()-1], NULL);
}


