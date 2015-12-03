//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Feb 18 10:12:41 PST 1999
// Last Modified: Thu Jul  8 15:12:29 PDT 1999
// Filename:      ...sig/doc/examples/all/perform/perform.cpp
// Syntax:        C++; Improv 2.1
//
// Description:   The original header from the C program is in the comment 
//                below.
//
//		  This version of the program with work in both
//		  Linux (gnu C++) and Windows 95/NT/98 (Microsoft
//		  Visual C++) without alterations.
//

/************************************
  PERFORM: the perform command. Takes input from stdin and translates 
  it into MIDI track-ready data which it then sends to a MIDI device.
  Playback is interactive, as the user can change tempi, search for strings
  in the file, pause the playback, or goto individual measures using key
  presses.

   Revision history:

   Created: Summer 1993 - Kyle Dawkins
   Altered: June 10th, 1994 - KLD
      - Ported to Borland C++ 3.0 from
        Microsoft's Quick C.
      - Removed memory management code
   Altered: June 13th, 1994 - KLD
      - Rewrote command-line parser
      - Added help screen for 
        interactive mode
   Altered: February 1999 - Craig Sapp
      - Ported to Linux operating system using
        Gnu C 2.7.2 or later, with Intel Pentium 75 MHz CPU or better.
      - Ported to Windows 95/NT operating system 
        using Microsoft Visual C++ ver. 5/6,
        with Intel Pentium 75 MHz CPU or better.
      - Modified command line options slightly for
        new operating systems (-p, -l added -i removed).
   Altered: July 1999 - Craig Sapp
      - Changed internal data structure for performance data.
      - Converted to C++ program flow control style.

************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "Performance.h"
#include "Options.h"
#include "KeyboardInput.h"
#include "LineDisplay.h"

typedef unsigned char uchar;
typedef unsigned long ulong;

// defines
#define DEFAULT_TEMPO    80    /* initial tempo if none specified          */
#define MAX_TEMPO       999    /* fastest that a quarter note can go       */
#define MIN_TEMPO         4    /* slowest that a quarter note can go       */
#define FORWARD_SEARCH    1
#define BACKWARD_SEARCH  -1

// defines for computer keyboard input modes
#define BUFF_STATUS_NOT_USED        0
#define BUFF_STATUS_SEARCH_BACKWARD 1
#define BUFF_STATUS_SEARCH_FORWARD  2

// function declarations
void          checkOptions          (Options& opts);
void          eraseline             (int count);
void          example               (void);
void          help                  (void);
void          help_screen           (void);
int           keyboardchar          (int key);
void          runloop               (Performance& data);
void          processBufferStatus   (int keybufferstatus);

// global variables
Performance   data;                  // where the MIDI data resides
KeyboardInput keyboard;              // the computer keyboard interface.
Options       options;               // the command-line interface.
LineDisplay   display;               // for displaying and erasing lines.
int           measure_flag = 0;      // for returning to a measure

#define KEY_BUFFER_SIZE 1024
char          keybuffer[KEY_BUFFER_SIZE] = {0}; // computer keyboard commands
int           keybufferindex  = 0;   // location in computer keyboard buffer
int           keybufferstatus = BUFF_STATUS_NOT_USED; 


// global variables associated with command-line options
double    default_tempo = DEFAULT_TEMPO; // for command line tempo setting
int       default_vel   = 64;            // velocity of note if none given
int       echo          = 1;             // true of echoing text or not.
istream*  input         = NULL;          // for input datafile to perform.


///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   options.setOptions(argc, argv);
   checkOptions(options);

   if (options.getArgCount() == 0) {
      data.read(cin);
      runloop(data);
   } else {
      // perform each command-line input file in sequence
      for (int i=1; i<=options.getArgCount(); i++) {
         data.read(options.getArg(i));
         runloop(data);
   
         // pause between performances according to "wait" option
         if (i < options.getArgCount()) {
            millisleep((float)(1000.0 * options.getDouble("wait")));
         }
      }
   }

   return 0;
} 

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// runloop -- the main interpreter of computer keyboard input and 
//     performance realization.
//

void runloop(Performance& data) {
   int quitFlag = 0;

   while (!quitFlag && !data.eof()) {
      // determine if it is time to play another note
      data.perform();

      // check the computer keyboard for input
      if (keyboard.hit()) {
         quitFlag = keyboardchar(keyboard.getch());
      }

      // give unnecessary cpu time to other processes on computer
      millisleep(1);
   }

   cout << endl;

   if (!quitFlag) {
      data.stop();
      return;
   }
}


///////////////////////////////////////////////////////////////////////////

//////////////////////////////
//
// checkOptions -- check the command line options for any arguments.
//    Options are:
//      -t <double> sets the amount by which the tempo is multiplied
//      -g turns OFF global command echo
//      -i <num> sets the ioaddress of the card (deactivated)
//      -v <int> sets the default velocity for keys with unspecified velocities
//      -\? or -h prints the help screen
//   New options:
//      -p <num> set the MIDI output to port <num> (replaces -i functionality)
//      -l       lists possible MIDI output ports and then exits immediately
//      --help   same as -h option.
//      --wait <num>     wait <num> seconds after each file to perform.
//
//

void checkOptions(Options& opts) {
   opts.define("tempo-scale|t=d:1.0");       
   opts.define("tempo=d:80");       
   opts.define("global-echo|g=b");
   opts.define("ioaddress|i=i:330 hex");
   opts.define("velocity|v=i:64");
   opts.define("port|p=i:0");
   opts.define("list-ports|l=b");
   opts.define("help|?|h=b");
   opts.define("wait|w=d:1.0 seconds");
   opts.define("author:b");
   opts.define("version:b");
   opts.define("example:b");
   opts.process();

   if (opts.getBoolean("author")) {
      cout << "Initial author: Kyle Dawkins, 1993-94\n"
           << "Revised by: Craig Stuart Sapp (craig@ccrma.stanford.edu) 1999" 
           << endl;
      exit(0);
   }
   if (opts.getBoolean("version")) {
      cout << "Humdrum tool: perform created 1993, version 8 July 1999" 
           << endl;
      cout << "compiled: " << __DATE__ << endl;
      exit(0);
   }
   if (opts.getBoolean("help")) {
      help();
      exit(0);
   }
   if (opts.getBoolean("example")) {
      example();
      exit(0);
   }

   if (opts.getBoolean("list-ports")) {
      int count = data.getNumPorts();
      for (int i=0; i<count; i++) {
         data.setPort(i);
         cout << "\t" << i << ":\t" << data.getName() << endl;
      }
      exit(0);
   } else {
      data.setPort(opts.getInteger("port"));
      cout << "opening MIDI output port " 
           << opts.getInteger("port") << ":\t" << data.getName() << endl;
      data.open();
   }
   
   default_tempo = opts.getDouble("tempo") * opts.getDouble("tempo-scale");
   if (default_tempo > MAX_TEMPO) {
      default_tempo = MAX_TEMPO;
      cerr << "Warning: reset tempo to: " << MAX_TEMPO << endl;
   } else if (default_tempo < MIN_TEMPO) {
      default_tempo = MIN_TEMPO;
      cerr << "Warning: reset tempo to: " << MIN_TEMPO << endl;
   }

   default_vel = opts.getInteger("velocity");
   if (default_vel > 127) {
      cerr << "Warning: reset default velocity to: " << 127 << endl;
      default_vel = 127;
   } else if (default_vel < 1) {
      cerr << "Warning: reset default velocity to: " << 1 << endl;
      default_vel = 1;
   }

   if (opts.getBoolean("global-echo")) {
      echo = 0;
   } else {
      echo = 1;
   }

   if (opts.getBoolean("ioaddress")) {
      cout << "Warning: the -i option is disabled, use the -p option" << endl;
   }

}



//////////////////////////////
//
// example -- give an example useage of this program
//

void example(void) {
   cout << "No examples yet" << endl;
}



//////////////////////////////
//
// help -- prints out a help page for the perform command including
//     invocation information and options.
//

void help(void) {
   cout << 
   "\n"
   "PERFORM   : Play Humdrum **MIDI files\n"
   "\n"
   "This command provides an interactive, stream-oriented MIDI-sequencer for\n"
   "proof-listening and auditioning of **MIDI-format inputs.\n"
   "\n"
   "Some Interactive commands:                <cr>   : return to beginning\n"
   "<ESC>       : terminate perform            p     : panic reset\n"
   "<space>     : pause; suspend playback      <     : reduce tempo\n"
   "   /<string>: search forward for string    >     : increase tempo\n"
   "   ?<string>: search backwards for string <num>- : back <num> measures\n"
   "   <num><cr>: go to measure <num>         <num>+ : forward <num> measures\n"
   "\n"
   "Syntax:\n"
   "\n"
   "   perform [-g] [-i hex] [-t n.n] [-v n] [inputfile]\n"
   "\n"
   "Options:\n"
   "\n"
   "   -g     : suppress echoing of performance text to standard output\n"
   "   -p n   : use port n for MIDI output\n"
   "   -l     : list possible MIDI output ports, then exit\n"
   "   -t n.n : set initial tempo to n.n times the default tempo\n"
   "   -v n   : set default MIDI key-velocity value: 1 (slow) to 127 (fast)\n"
   << endl;
}



//////////////////////////////
//
// help_screen -- prints out all of the interactive key commands that 
//     are available during the perform process.
//

void help_screen(void) {
   cout <<
   "\n"
   "Key                  Function                      \n"
   "---                  --------                      \n"
   "<Esc> or q           Quit                          \n"
   "<space>              Pause playback                \n"
   "f                    Flag measure                  \n"
   "p                    Panic                         \n"
   "P                    Power panic                   \n"
   "< or ,               Slower tempo                  \n"
   "> or .               Faster tempo                  \n" 
   "<enter>              Restart from beginning of input file\n"
   "-                    Go to previous barline        \n"
   "+                    Go to next barline            \n"
   "<number><enter>      Go to measure <number>        \n"
   "<number>-            Go back <number> measures     \n"
   "<number>+            Go forward <number> measures  \n"
   "/<string>            Search forward for occurrence of <string>\n"
   "?<string>            Search backward for occurrence of <string>\n"
   "h                    Display this help screen      \n"
   << endl;
}



//////////////////////////////
//
// keyboardchar -- handle input from the computer keyboard.
//

int keyboardchar(int key) {
   int quitflag = 0;

   switch (key) {
      case 'p':                // panic
         data.tacet();
         break;

      case 'P':                // super panic
         data.silence();
         break;

      case 'F':                // flag a measure
      case 'f':                // flag a measure
         data.displayClear();
         measure_flag = data.getMeasure();
         cout << "Measure " << measure_flag << " flagged." << endl;
         break;

      case ' ':                // halt
      case 'h':                // halt
      case 'H':                // halt
         data.pause();
         data.displayClear();
         if (tolower(key) == 'h') {
            help_screen();
         }
         cout << "Press space to continue..." << flush;
         break;

      case '0':                // bar number
      case '1':                // bar number
      case '2':                // bar number
      case '3':                // bar number
      case '4':                // bar number
      case '5':                // bar number
      case '6':                // bar number
      case '7':                // bar number
      case '8':                // bar number
      case '9':                // bar number
         cout << "+++ Command Not ready" << endl;
         break;

      case '-':                // relative back new bar
      case '_':                // relative back new bar
         data.backBar(1);
         break;

      case '+':                // relative forward new bar
      case '=':                // relative forward new bar
         data.forwardBar(1);
         break;

      case 0x0d:               // return character
         processBufferStatus(keybufferstatus);
         break;
         
      case '.':                // speed up tempo
      case '>':                // speed up tempo
         {
         double current_tempo = data.getTempo() + 6;
         if (current_tempo > MAX_TEMPO) {
            current_tempo = MAX_TEMPO;
         } else if (current_tempo < MIN_TEMPO) {
            current_tempo = MIN_TEMPO;
         }
         data.setTempo(current_tempo);
         }
         break;

      case ',':                // slow down tempo
      case '<':                // slow down tempo
         {
         double current_tempo = data.getTempo() - 6;
         if (current_tempo > MAX_TEMPO) {
            current_tempo = MAX_TEMPO;
         } else if (current_tempo < MIN_TEMPO) {
            current_tempo = MIN_TEMPO;
         }
         data.setTempo(current_tempo);
         }
         break;

      case '/':                // search forward
         keybufferstatus = BUFF_STATUS_SEARCH_FORWARD;
         break;
      case '?':                // search backward
         keybufferstatus = BUFF_STATUS_SEARCH_BACKWARD;
         break;

      case 0x08:               // backspace
cout << "Not yet ready" << endl;
         break;

      case 0x1b:               // quit (Escape)
      case 'q':                // quit
      case 'Q':                // quit
         data.stop();
         quitflag = 1;
         break;

      default:
         break;
   }

   return quitflag;
}



//////////////////////////////
//
// processBufferStatus -- figure out what to do with the
//   keyboard buffer
//

void processBufferStatus(int keybufferstatus) {
   switch (keybufferstatus) {
      case BUFF_STATUS_SEARCH_BACKWARD:
         break;
      case BUFF_STATUS_SEARCH_FORWARD:
         break;
      case BUFF_STATUS_NOT_USED:
         break;
      default: 
         cout << "Unknown keyboard buffer status flag" << endl;
         exit(1);
   }

}



