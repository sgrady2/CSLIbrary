//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Mar  8 20:28:55 PST 2000
// Last Modified: Wed Mar  8 21:14:28 PST 2000
// Filename:      ...sig/doc/examples/all/wiggle.cpp
// Syntax:        C++; batonImprov 2.0; sig 1.0
//  
// Description:  Program that converts baton position information
//               into a soundfile.
//

#include "batonImprov.h" 
#include "sig.h"


/*----------------- beginning of improvization algorithms ---------------*/

SoundFileOut  *outsound = NULL;      // sound file writing object
int            writingQ = 0;         // 0=off, 1=on
int            channels = 1;         // 1=monophonic, 2=stereo

/*--------------------- maintenance algorithms --------------------------*/

//////////////////////////////
//
// description -- this function is called by the improv interface
//     whenever a capital "D" is pressed on the computer keyboard.
//     Put a description of the program and how to use it here.
//

void description(void) {
   cout << 
      "Wiggle: a program that creates a sound file from positions\n"
      "from the radio batons.\n"
      "\n"
      "Commands: \n"
      "   Space: turn sound file writing on/off\n"
      "   1:     monophonic sound with x-axis of baton1 only\n"
      "   2:     stereo sound with x-axis and y-axis of baton1\n"
      "\n"
      << endl;
} 


//////////////////////////////
//
// initialization -- this function is called by the improv
//     interface once at the start of the program.  Put items
//     here which need to be initialized at the beginning of
//     the program.
//

void initialization(void) { }



//////////////////////////////
//
// finishup -- this function is called by the improv interface
//     whenever the program is exited.  Put items here which
//     need to be taken care of when the program is finished.
//

void finishup(void) { 
   if (outsound != NULL) {
      delete outsound;
      outsound = NULL;
   }
}


/*-------------------- main loop algorithms -----------------------------*/

//////////////////////////////
//
// mainloopalgorithms -- this function is called by the improv interface
//   continuously while the program is running.  The global variable t_time
//   which stores the current time is set just before this function is
//   called and remains constant while in this functions.
//

void mainloopalgorithms(void) { 
   if (outsound != NULL && baton.t1pb.getCount() > 0) {
      baton.t1pb.extract();
      outsound->connect(baton.x1pb.extract()/64.0 - 1.0, 0);
      outsound->connect(baton.y1pb.extract()/64.0 - 1.0, 1);
      outsound->action();
   }
}


/*-------------------- triggered algorithms -----------------------------*/

//////////////////////////////
//
// stick1trig -- this function is called automatically whenever
//   a baton stick #1 trigger is received.
//

void stick1trig(void) { }



//////////////////////////////
//
// stick2trig -- this function is called automatically whenever
//   a baton stick #2 trigger is received.
//

void stick2trig(void) { }



//////////////////////////////
//
// b14plustrig -- this function is called automatically whenever
//   the b14+ button is pressed.
//

void b14plustrig(void) { }



//////////////////////////////
//
// b15plustrig -- this function is called automatically whenever
//   the b15+ button is pressed.
//

void b15plustrig(void) { }




//////////////////////////////
//
// b14minusuptrig -- this function is called automatically whenever
//   the b14- foot trigger is pressed.
//

void b14minusuptrig(void) { }



//////////////////////////////
//
// b14minusdowntrig -- this function is called automatically whenever
//   the b14- foot trigger is released.
//

void b14minusdowntrig(void) { }



//////////////////////////////
//
// b15minusuptrig -- this function is called automatically whenever
//   the b15- foot trigger is pressed.
//

void b15minusuptrig(void) { }



//////////////////////////////
//
// b15minusdowntrig -- this function is called automatically whenever
//   the b15- foot trigger is released.
//

void b15minusdowntrig(void) { }



///////////////////////////////
//
// keyboardchar -- this function is called by the improv interface
//     whenever a key is pressed on the computer keyboard.
//     Put commands here which will be executed when a key is
//     pressed on the computer keyboard.
//

void keyboardchar(int key) { 
   switch (key) {
      case ' ':             // toggle writing of soundfile
         if (writingQ) {
            delete outsound;
            outsound = NULL;
            writingQ = 0;
         } else {
            cout << "Going to write a " << channels << " channel"
                 << " sound file." << endl;
            cout << "Enter an output soundfile name "
                    "(ending in .wav , .au or .snd) :";
            char* buffer = new char[1000];
            echoKeysOn();
            cin  >> buffer;
            echoKeysOff();
            if (outsound != NULL) {
               delete outsound;
               outsound = NULL;
            }
            SoundHeader header;
            header.setHighMono();
            header.setChannels(channels);
            outsound = new SoundFileOut(buffer, header);
            delete buffer;
  
            // flush the baton position buffers
            baton.t1pb.reset();
            baton.x1pb.reset();
            baton.y1pb.reset();
            writingQ = 1;
         }
         break;
      case '1':             // set channels to mono
         if (writingQ) {
            cout << "Error: Cannot change channel settings while"
                 << " writing a soundfile." << endl;
         } else {
            cout << "Setting channel count to 1." << endl;
            channels = 1;
         }
         break;
      case '2':             // set channels to stereo
         if (writingQ) {
            cout << "Error: Cannot change channel settings while"
                 << " writing a soundfile." << endl;
         } else {
            cout << "Setting channel count to 2." << endl;
            channels = 2;
         }
         break;
   }
}



/*------------------ end improvization algorithms -----------------------*/

