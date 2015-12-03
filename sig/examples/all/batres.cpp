//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Feb  2 16:49:52 PST 1999
// Last Modified: Sun Feb  7 17:53:35 PST 1999
// Filename:      ...sig/doc/examples/sig/sigcontrol/batres/batres.cpp
// Syntax:        C++; batonImprov; sig
//  
// Description: Plays sound through a two-pole resonating filter
//      with baton 1 center frequency and bandwidth.
//	The filter can be controlled either in the z-plane or s-plane.
//

#include "batonImprov.h" 
#include "sig.h"
#include "math.h"


/*----------------- beginning of improvization algorithms ---------------*/

// sound elements
Constant      frequency;
Constant      bandwidth;
Constant      amplitude;
WhiteNoise    whitenoise;
Resonator     reson;
Multiply      mul;
LinuxSoundOut outsound(44100, 2);

Action        action;
SigTimer      soundTimer;
SigTimer      poledisplay;
int           poledisplayQ = 1;

#define       MODE_Z 0
#define       MODE_S 1
int           mode = MODE_Z;


/*--------------------- maintenance algorithms --------------------------*/

void description(void) {
   cout 
   << "Initial test program for live sound with Improv programs\n"
   << "Keyboard commands: \n"
   << " p = toggle position information display\n"
   << " s/z = use either s-plane (rectangular) or z-plane (circular) domains\n"
   << endl;
}

void initialization(void) { 
   // sound element connections
   outsound.connect(mul);     // left
   outsound.connect(mul);     // right
   mul.connect(reson);
   mul.connect(amplitude);
   reson.connect(whitenoise, 0);
   reson.connect(frequency, 1);
   reson.connect(bandwidth, 2);
   action.freezeState(outsound);
   soundTimer.setPeriod(1);
   poledisplay.setPeriod(200);
   eventIdler.setPeriod(0);
}


void finishup(void) { }


/*-------------------- main loop algorithms -----------------------------*/

double getradius(int x, int y) {
   double xr = x/125.0 * 2 - 1;
   double yr = y/125.0 * 2 - 1;
   double output = sqrt(xr*xr + yr*yr);
   if (output > 1.0) {
      output = 0.9999;
   }

   // stretch the outer radius values
   output = 1 - pow(output - 1, 2);

   return output;
}

double getangle(int x, int y) {
   double xr = x/127.0 * 2 - 1;
   double yr = y/127.0 * 2 - 1;
   return atan2(yr,xr);
}

  
double angle;
double radius;
double amp;

void mainloopalgorithms(void) { 

   switch (mode) {
      case MODE_S:
         if (baton.z2p < 63) {
            amp = 0;
         } else { 
            amp    = (baton.z2p/127.0 - 0.5) * 2;
         }

         amplitude.setValue(amp);
         frequency.setValue(midiscale(baton.x2p, 0, 10000));
         bandwidth.setValue(midiscale(baton.y2p, 0, 3000));
         break;
      case MODE_Z:
         angle  = getangle(baton.x2p, baton.y2p);
         radius = getradius(baton.x2p, baton.y2p);
         if (baton.z2p < 63) {
            amp = 0;
         } else {
            amp    = (baton.z2p/127.0 - 0.5) * 2;
            amp    *= (pow(radius, 18) + 0.5) * 10;
         }

         amplitude.setValue(amp);
         frequency.setValue(fabs(angle)*1000);
         bandwidth.setValue((1.001-radius)*3000);
         break;
   }

   if (poledisplayQ && poledisplay.expired()) {
      switch (mode) {
         case MODE_S:
            cout << "\rcenter: " << frequency.output(0) << "Hz"
                 << "     bandwidth: " << bandwidth.output(0)
                 << "             "
                 << flush;
            break;
         case MODE_Z:
            poledisplay.reset();
            cout << "\rpole radius: " << radius 
                 << "     angle: " << angle << "          " 
                 << flush;
            break;
      }

   }

   if (soundTimer.expired()) {
      soundTimer.reset();
      action.tickFrozenState(1024);
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
      case 'p':
         poledisplayQ = !poledisplayQ;
         if (poledisplayQ) {
            cout << "Pole position display ON" << endl;
         } else {
            cout << "\nPole position display OFF" << endl;
         }
         break;
      case 's':
         mode = MODE_S;
         cout << "\ns-plane mode" << endl;
         break;
      case 'z':
         mode = MODE_Z;
         cout << "\nz-plane mode" << endl;
         break;
      case ' ':
         cout << "\nCenter: " << frequency.output(0) << "Hz"
              << "      Bandwidth: " << bandwidth.output(0) 
              << endl;
         break;
   }
}

/*------------------ end improvization algorithms -----------------------*/




