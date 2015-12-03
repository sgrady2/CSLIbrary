//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Feb  2 16:49:52 PST 1999
// Last Modified: Sat Mar 13 16:18:43 PST 1999
// Filename:      ...sig/doc/examples/sig/sigcontrol/sloop/sloop.cpp
// Syntax:        C++; synthImprov; sig
//  
// Description:   Create soundfile loops in real-time with optional
//                saving of the mix to a soundfile.
//
// Summary of sound signal connections:
//    soundfiles -> soundloops -> add => scale => speakers -> outfile
// 

/* Keyboard Interface Commands:

      -  / =       = lower/raise volume level

   File Commands:           
      f <x> <file> = load soundfile <file> into memory <x>
      s            = show list of soundfiles in memory
      m            = show memory taken up by soundfiles in memory
      o <file>     = set output filename (will not overwrite old files)
      r            = start/stop recording of output file
   
   Loop Commands: 
      a <x> <y>    = create loop <x> using soundfile number <y> 
      d            = display current loops in memory
      l <x>        = focus on loop <x>
      i            = show information of current loop
      b            = start at beginning of loop
      < / >        = loop backwards / forwards
      t / p        = straight loop / palindrome loop
      q <x>        = fill end of period with <x> number of blank samples
      [ ] / { }    = fine loop end controls/gross loop end controls
      ; ' / : "    = fine loop begin controls/gross loop begin controls
      <space>      = pause/unpause current loop
   
      <esc>        = cancel current input
*/
   
#include "sig.h"
#include "synthImprov.h" 


/*----------------- beginning of improvization algorithms ---------------*/

// function declarations:
void runcommands(int key);
void displayLoopInformation(int loopnumber = -1);

#define MODE_COMMAND     0
#define MODE_FILENAME    1
#define MODE_OUTFILENAME 2
#define MODE_LOOPNUM     3
#define MODE_LOOPADD     4
#define MODE_BLANK       5
int inputMode = MODE_COMMAND;

Array<char> input_buffer;

// sound elements

SoundHeader   header;
LinuxSoundOut speakers(44100, 2);
SoundFileOut *fileout = NULL;
Scale         scale(0.1);
Add           add;

SoundLoop     refer;
#define MAX_SOUNDFILES 25

Collection<SoundFileInMemory*> soundfiles;  // list of soundfile in memory
Collection<SoundLoop*>         soundloops;  // list of soundfile loops
int           focusLoop = -1;               // currently active loop
int           outrecordingQ = 0;            // for recording output file
int           sampleStep    = 10;           // for loop begin/end settings

Action        action;
SigTimer      soundTimer;


double        smoothFactor  = 0.0005;


/*--------------------- maintenance algorithms --------------------------*/

void description(void) {
   cout 
   << "sloop: Interactive sound looping program\n"
   << "\n"
   << "   -  / =       = lower/raise volume level\n"
   << "File Commands:           \n"
   << "   f <x> <file> = load soundfile <file> into memory <x>\n"
   << "   s            = show list of soundfiles in memory\n"
   << "   m            = show memory taken up by soundfiles in memory\n"
   << "   o <file>     = set output filename (will not overwrite old files)\n"
   << "   r            = start/stop recording of output file\n"
   << "\n"
   << "Loop Commands: \n"
   << "   a <x> <y>    = create loop <x> using soundfile number <y> \n"
   << "   d            = display current loops in memory\n"
   << "   l <x>        = focus on loop <x>\n"
   << "   i            = show information of current loop\n"
   << "   b            = start at beginning of loop\n"
   << "   < / >        = loop backwards / forwards\n"
   << "   t / p        = straight loop / palindrome loop\n"
   << "   q <x>        = fill end of period with <x> number of blank samples\n"
   << "   [ ] / { }    = fine loop end controls/gross loop end controls\n"
   << "   ; ' / : \"    = fine loop begin controls/gross loop begin controls\n"
   << "   <space>      = pause/unpause current loop\n"
   << "\n"
   << "   <esc>        = cancel current input\n"
   << endl;
}

void initialization(void) { 

   input_buffer.setSize(0);
   input_buffer.allowGrowth();

   header.setHighMono();

   soundfiles.setSize(MAX_SOUNDFILES);
   for (int i=0; i<MAX_SOUNDFILES; i++) {
      soundfiles[i] = NULL;
   }
   
   soundloops.setSize(30);
   for (int i=0; i<soundloops.getSize(); i++) {
      soundloops[i] = NULL;
   }
   soundloops.allowGrowth(0);
  
   // sound element connections

   scale.connect(add);
   speakers.connect(scale);

   header.setHighMono();
   fileout = new SoundFileOut("test.snd", header);
   fileout->connect(speakers);

   eventIdler.setPeriod(0);
   soundTimer.setPeriod(1);
}


void finishup(void) { 
   if (fileout != NULL) {
      delete fileout;
   }
   
   int i;
   for (i=0; i<soundfiles.getSize(); i++) {
      if (soundfiles[i] != NULL) {
         delete soundfiles[i];
         soundfiles[i] = NULL;
      }
   }

   for (i=0; i<soundloops.getSize(); i++) {
      if (soundloops[i] != NULL) {
         delete soundloops[i];
         soundloops[i] = NULL;
      }
   }

}


/*-------------------- main loop algorithms -----------------------------*/

void mainloopalgorithms(void) { 


   if (soundTimer.expired()) {
      soundTimer.reset();
      action.tick(*fileout, 512);
   }
}


/*-------------------- triggered algorithms -----------------------------*/

void keyboardchar(int key) { 
   if (key == 27) {             // escape key
      switch (inputMode) {
         case MODE_LOOPADD:
         case MODE_LOOPNUM:
         case MODE_FILENAME:
         case MODE_OUTFILENAME:
         case MODE_BLANK:
            cout << "\nCancelled" << endl;
            input_buffer.setSize(0);
            break;
         default:
            input_buffer.setSize(0);
            break;
      }
      inputMode = MODE_COMMAND;
   }

   if (key == '\n') {
      switch (inputMode) {
         case MODE_LOOPADD:
         {
            cout << endl;
            int temp1 = 0;
            int temp2 = 0;
            char* current;
            current = strtok(input_buffer.getBase(), " \n\t");
            if (current == NULL || current[0] == '\0') {
               cout << "Invalid loop addition, canceled" << endl;
               inputMode = MODE_COMMAND;
               input_buffer.setSize(0);
               return;
            } else {
               temp1 = atoi(current) - 1;
            }
          
            current = strtok(NULL, " \n\t");
            if (current == NULL || current[0] == '\0') {
               cout << "Invalid filename addition, canceled" << endl;
               inputMode = MODE_COMMAND;
               input_buffer.setSize(0);
               return;
            } else {
               temp2 = atoi(current) - 1;
            }

            if (temp1 == soundloops.getSize()) {
               soundloops[temp1] = NULL;
            }
            if (soundloops[temp1] == NULL) {
               soundloops[temp1] = new SoundLoop;
            }
            soundloops[temp1]->borrow(*soundfiles[temp2]);
            soundloops[temp1]->reset();
            focusLoop = temp1;
            add.disconnect(*soundloops[temp1]);
            add.connect(*soundloops[temp1]);

            input_buffer.setSize(0);
            inputMode = MODE_COMMAND;
            return;
         }
         case MODE_LOOPNUM:
         {
            cout << endl;
            char* current;
            current = strtok(input_buffer.getBase(), " \n\t");
            if (current == NULL || current[0] == '\0') {
               cout << "Selected Loop is: " << focusLoop+1 << endl;
               input_buffer.setSize(0);
               inputMode = MODE_COMMAND;
               return;
            } else {
               int temp = atoi(current);
               if (temp < 0 || temp > soundloops.getSize()) {
                  cout << "Error: selected loop is out of range." << endl;
                  cout << "Keeping prior loop: " << focusLoop+1 << endl;
               } else {
                  focusLoop = temp-1;
                  cout << "Selected loop is: " << focusLoop+1 << endl;
               }
            }
            inputMode = MODE_COMMAND;
            return;
         }
         case MODE_FILENAME:
         {  
            cout << endl;
            char blank = '\0';
            input_buffer.append(blank);
            char* current;
            current = strtok(input_buffer.getBase(), " \n\t");
            int location = atol(current) - 1;
            if (location < 0 || location >= MAX_SOUNDFILES) {
               cout << "Error: soundfile location out of range." << endl;
               inputMode = MODE_COMMAND;
               return;
            }
            current = strtok(NULL, " \n\t");
            if (soundfiles[location] == NULL) {
               soundfiles[location] = new SoundFileInMemory;
            }
            soundfiles[location]->setFile(current);
            cout << "file " << location + 1 << " set to " 
                 << current << endl;
            input_buffer.setSize(0);
            inputMode = MODE_COMMAND;
            return;
         }
         case MODE_OUTFILENAME:
         {  
            char blank = '\0';
            input_buffer.append(blank);
            char* current;
            current = strtok(input_buffer.getBase(), " \n\t");
            if (fileout != NULL) {
               delete fileout;
            }
            fileout = new SoundFileOut(current, header);
            cout << "Output soundfile set to : " 
//                 << fileout->getFilename() 
                  << "unknown"
                 << endl;
            input_buffer.setSize(0);
            inputMode = MODE_COMMAND;
            return;
         }
            
         case MODE_BLANK:
         {  
            char blank = '\0';
            input_buffer.append(blank);
            int blanks = atoi(input_buffer.getBase());
            if (focusLoop >= 0) {
               soundloops[focusLoop]->setFill(blanks);
            }
            input_buffer.setSize(0);
            inputMode = MODE_COMMAND;
            return;
         }
         
         default:
            inputMode = MODE_COMMAND;
            return;
      }
   }

   switch (inputMode) {
      case MODE_LOOPNUM:               // adding char to loop selection
      case MODE_LOOPADD:               // adding char to loop selection
      case MODE_FILENAME:               // adding char to input filename
      case MODE_OUTFILENAME:            // adding char to output filename
      case MODE_BLANK:                  // adding char to blank count
      {  
         char realkey = (char)key;
         if (input_buffer.getSize() > 0 && (realkey == 127 || realkey == 8)) {
            input_buffer.setSize(input_buffer.getSize()-1);
            cout << "\b \b";
         } else {
            cout << realkey << flush;
            input_buffer.append(realkey);
         }
         return;
      }

      default:
         break;
   }

   runcommands(key);

}


//////////////////////////////
//
// runcommands -- keyboard commands and what they do
//

void runcommands(int key) {
   int i;
   switch (key) {
      case 's':                   // display soundfile list
         for (i=0; i<MAX_SOUNDFILES; i++) {
            if (soundfiles[i] != NULL) {
               cout << "file: " << i+1 << '\t'
                    << soundfiles[i]->getFilename() 
                    << endl;
            }
         }
         break; 

      case 'f':                   // load a soundfile
         cout << "\nEnter (1) sound location and (2) soundfile name: " << flush;
         inputMode = MODE_FILENAME;
         break;

      case 'a':                   // add a loop
         cout << "\Enter (1) the loop number to add/replace and \n"
                  "(2) soundfile num: " << flush;
         inputMode = MODE_LOOPADD;
         break;
    
      case 'l':                   // select a loop
         cout << "\nEnter a loop to focus on: " << flush;
         inputMode = MODE_LOOPNUM;
         break;

      case 'm':                   // display soundfile memory usage
         cout << "using: " << SoundFileInMemory::getMemoryUsage()/1024.0
              << " kbytes for soundfiles" << endl;
         break;

      case '-':                   // lower the volume
      {
         double value;
         value = scale.getScale();
         value *= 0.95;
         cout << "Volume set to: " << value << endl;
         scale.setScale(value);
         break;
      }

      case '=':                   // raise the volume
      {
         double value;
         value = scale.getScale();
         value *= 1.05;
         cout << "Volume set to: " << value << endl;
         scale.setScale(value);
         break;
      }

      case '<':                   // loop backwards
         if (focusLoop >= 0) {
            cout << "Loop " << focusLoop << " backwards " << endl;
            soundloops[focusLoop]->goBackward();
         }
         break;

      case '>':                   // loop forwards
         if (focusLoop >= 0) {
            cout << "Loop " << focusLoop << " forwards " << endl;
            soundloops[focusLoop]->goForward();
         }
         break;

      case 't':                   // loop straight
         if (focusLoop >= 0) {
            cout << "Loop " << focusLoop << " straight " << endl;
            soundloops[focusLoop]->loopStraight();
         }
         break;

      case 'p':                   // loop palindrome
         if (focusLoop >= 0) {
            cout << "Loop " << focusLoop << " palindrome " << endl;
            soundloops[focusLoop]->loopPalindrome();
         }
         break;
 
      case 'b':                   // loop begin
         if (focusLoop >= 0) {
            cout << "Loop " << focusLoop << " beginning " << endl;
            soundloops[focusLoop]->gotoStart();
         }
         break;
 
      case 'i':                   // loop information 
         if (focusLoop >= 0) {
            displayLoopInformation(focusLoop);
         }
         break;
 
      case 'd':                   // all loops information 
         displayLoopInformation();
         break;
 
      case '[':                   // loop end fine control down
         if (focusLoop >= 0) {
            soundloops[focusLoop]->changeEnd(-1);
            cout << "Loop " << focusLoop << " end = " 
                 << soundloops[focusLoop]->getEnd() << endl;
         }
         break;
 
      case ']':                   // loop end fine control up
         if (focusLoop >= 0) {
            soundloops[focusLoop]->changeEnd(1);
            cout << "Loop " << focusLoop << " end = " 
                 << soundloops[focusLoop]->getEnd() << endl;
         }
         break;
 
      case '{':                   // loop end gross control down
         if (focusLoop >= 0) {
            soundloops[focusLoop]->changeEnd(-sampleStep);
            cout << "Loop " << focusLoop << " end = " 
                 << soundloops[focusLoop]->getEnd() << endl;
         }
         break;
 
      case '}':                   // loop end gross control up
         if (focusLoop >= 0) {
            soundloops[focusLoop]->changeEnd(sampleStep);
            cout << "Loop " << focusLoop << " end = " 
                 << soundloops[focusLoop]->getEnd() << endl;
         }
         break;
 
      case ';':                   // loop begin fine control down
         if (focusLoop >= 0) {
            soundloops[focusLoop]->changeBegin(-1);
            cout << "Loop " << focusLoop << " end = " 
                 << soundloops[focusLoop]->getEnd() << endl;
         }
         break;
 
      case '\'':                  // loop begin fine control up
         if (focusLoop >= 0) {
            soundloops[focusLoop]->changeBegin(1);
            cout << "Loop " << focusLoop << " end = " 
                 << soundloops[focusLoop]->getEnd() << endl;
         }
         break;
 
      case ':':                  // loop begin gross control down
         if (focusLoop >= 0) {
            soundloops[focusLoop]->changeBegin(-sampleStep);
            cout << "Loop " << focusLoop << " end = " 
                 << soundloops[focusLoop]->getEnd() << endl;
         }
         break;
 
      case '\"':                 // loop begin gross control up
         if (focusLoop >= 0) {
            soundloops[focusLoop]->changeBegin(sampleStep);
            cout << "Loop " << focusLoop << " end = " 
                 << soundloops[focusLoop]->getEnd() << endl;
         }
         break;

   }
}



//////////////////////////////
//
// displayLoopInformation --
//     default value: loopnumber = -1
//

void displayLoopInformation(int loopnumber) {
   cout << "Loop information goes here" << endl;
}



/*------------------ end improvization algorithms -----------------------*/



