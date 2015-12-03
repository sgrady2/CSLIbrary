//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Feb  2 16:49:52 PST 1999
// Last Modified: Tue Feb  2 16:49:57 PST 1999
// Filename:      ...improv/doc/examples/batonImprov/theremin/theremin.cpp
// Syntax:        C++; batonImprov
//  
// Description: Plays an oscillator with baton 1/2 x-axis controlling
//	frequency and baton 1/2 y-axis controlling amplitude.
//

#include "batonImprov.h" 
#include "sig.h"


/*----------------- beginning of improvization algorithms ---------------*/

// sound elements

Constant      smoothamt(0.0005);
Constant      Axaxis;
Constant      Ayaxis;
Smoother      Afrequency;
Smoother      Aamplitude;
Osc           Aosc;

Constant      Bsmoothamt(0.001);
Constant      Bxaxis;
Constant      Byaxis;
Smoother      Bfrequency;
Smoother      Bamplitude;
Osc           Bosc;

Add           add;
Scale         scale(0.5);
LinuxSoundOut outsound(44100, 2);

Action        action;
SigTimer      soundTimer;
SigTimer      displayTimer;
int           positionDisplay = 0;

double smoothFactor  = 0.0005;


SoundHeader  header;
SoundFileOut *fileout;


int          sine1 = 1;     // boolean for outputing sine1 sound
int          sine2 = 1;     // boolean for outputing sine2 sound

/*--------------------- maintenance algorithms --------------------------*/

void description(void) {
   cout 
   << "Initial test program for live sound with Improv programs"
   << "press 'p' to toggle frequency position display\n"
   << endl;
}

void initialization(void) { 
   header.setHighMono();
   fileout = new SoundFileOut("test.snd", header);

   // sound element connections

   Afrequency.connect(Axaxis);
   Afrequency.connect(smoothamt);
   Aamplitude.connect(Ayaxis);
   Aamplitude.connect(smoothamt);
   Aosc.connect(Afrequency);
   Aosc.connect(Aamplitude);

   Bfrequency.connect(Bxaxis);
   Bfrequency.connect(smoothamt);
   Bamplitude.connect(Byaxis);
   Bamplitude.connect(smoothamt);
   Bosc.connect(Bfrequency);
   Bosc.connect(Bamplitude);

   add.connect(Aosc);
   add.connect(Bosc);
   scale.connect(add);
   outsound.connect(scale);
   outsound.connect(scale);
   fileout->connect(outsound);

   action.freezeState(*fileout);
   eventIdler.setPeriod(0);
   soundTimer.setPeriod(1);
   displayTimer.setPeriod(200);
}


void finishup(void) { }


/*-------------------- main loop algorithms -----------------------------*/

void mainloopalgorithms(void) { 
   Axaxis.setValue((baton.x1p/127.0)*3200 + 200);
   Ayaxis.setValue(baton.y1p/127.0);
   Bxaxis.setValue((baton.x2p/127.0)*3200 + 200);
   Byaxis.setValue(baton.y2p/127.0);
   
   if (positionDisplay && displayTimer.expired()) {
      displayTimer.reset();
      cout << "\rfrequency: " << Afrequency.output(0)
           << ", " << Bfrequency.output(0)
           << "        amplitude: "   << Aamplitude.output(0)
           << ", " << Bamplitude.output(0)
           << flush;
   }

   if (soundTimer.expired()) {
      soundTimer.reset();
      action.tick(*fileout, 512);
   }
}


/*-------------------- triggered algorithms -----------------------------*/

void stick1trig(void) { }
void stick2trig(void) { }
void b14plustrig(void) { }
void b15plustrig(void) { }
void b14minusuptrig(void) { }
void b14minusdowntrig(void) { }
void b15minusuptrig(void) { }
void b15minusdowntrig(void) { }

void keyboardchar(int key) { 
   switch (key) {
      case '[':       // decrease smooth factor
         smoothFactor -= 0.00001;
         if (smoothFactor < 0.000) {
            smoothFactor = 0.0;
         }
         smoothamt.setValue(smoothFactor);
         cout << "Smooth factor is: " << smoothFactor << endl;
         break;
      case ']':       // increase smooth factor
         smoothFactor += 0.00001;
         if (smoothFactor > 1.000) {
            smoothFactor = 1.0;
         }
         smoothamt.setValue(smoothFactor);
         cout << "Smooth factor is: " << smoothFactor << endl;
         break;
      case 'p':      // toggle position display
         positionDisplay = !positionDisplay;
         if (positionDisplay) {
            cout << "position display ON" << endl;
         } else {
            cout << "\nposition display OFF" << endl;
         }
         break;

      case '1':
         sine1 = !sine1;
         if (sine1) {
            add.connect(Aosc, 1, 1);
            cout << "Turning on sine 1" << endl;
         } else {
            add.disconnect(Aosc);
            cout << "Turning off sine 1" << endl;
         }
         break;

      case '2':
         sine2 = !sine2;
         if (sine2) {
            add.connect(Bosc, 1, 2);
            cout << "Turning on sine 2" << endl;
         } else {
            add.disconnect(Bosc);
            cout << "Turning off sine 2" << endl;
         }
         break;
           
   }

}

/*------------------ end improvization algorithms -----------------------*/





