//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Feb  2 16:49:52 PST 1999
// Last Modified: Wed Feb 10 13:10:39 PST 1999
// Filename:      ...improv/doc/examples/sig/sigcontrol/lightres/lightres.cpp
// Syntax:        C++; synthImprov
//  
// Description: Plays sound through a two-pole resonating filter
//      with lightening with controllers 21 and 22.
//	The filter can be controlled either in the z-plane or s-plane.
//      by pressing 's' or 'z'.  The default is the z-plane where
//      the resonance is increased the closer you get to the unit circle;
//      the center of the x-y plane with sound the most like whitenoise.
// Warning: The coding is extremely inefficient; program should work without
//      problems on 200 MHz or greater computers.
// Possible extensions: get sound input to resonate instead of whitenoise.
//

#include "synthImprov.h" 
#include "sig.h"
#include "math.h"


/*----------------- beginning of improvization algorithms ---------------*/

// sound elements
Constant      frequency;     // current center frequency value is stored here
Constant      bandwidth;     // current bandwidth value is stored here
Constant      amplitude;     // current amplitude value is stored here
WhiteNoise    whitenoise;    // a whitenoise generator
Resonator     reson;         // a two-pole resonator filter
Multiply      mul;           // multiplication for amplitude envelope on output
LinuxSoundOut outsound(44100, 2); // output sound to soundcard 
Action        action;        // class to handle calculation of sound samples

SigTimer      soundTimer;    // timer to keep track of generating sound chunks
SigTimer      poledisplay;   // time for displaying the pole/radius freq/bandw
int           poledisplayQ = 1; // boolean for displaying position info

#define       MODE_Z 0       /* z-plane (circular) resonator controls */
#define       MODE_S 1       /* s-plane (rectangular) resontaor controls */
int           mode = MODE_Z; // currently active plane type 

#define MINCFREQ 0           /* minimum resonator center frequency in Hz */
#define MAXCFREQ 10000       /* maximum resonator center frequency in Hz */
#define MINBANDW 0           /* minimum bandwidth in Hz (on unit circle) */
#define MAXBANDW 3000        /* maxmimum bandwidth in Hz (at circle center) */

#define XAXIS 21             /* default controller number for the x-axis */
#define YAXIS 22             /* default controller number for the y-axis */
#define ZAXIS 23             /* default controller number for the z-axis */

int     xaxis = XAXIS;          // x-axis cont controller
int     yaxis = YAXIS;          // y-axis cont controller
int     zaxis = ZAXIS;          // z-axis cont controller
int     contdirection = 1;      // xyz keys will select next highest contrller
                                // used with the 'd' computer keyboard key.


/*--------------------- maintenance algorithms --------------------------*/

void description(void) {
   cout 
   << "Initial test program for live sound with Improv programs\n"
   << "Continuous controller version for Lightening\n"
   << "Keyboard commands: \n"
   << " p = toggle position information display\n"
   << " s = toggle s-plane (rectangular) or z-plane (circular) domains\n"
   << " x/y/z = increment/decrement the controller number for each axis\n"
   << " d = switch the increment/decrement of xyz controllers\n"
   << "X-axis and Y-axis control center freq. and bandwidth according to s/z\n"
   << "Z-axis controller the overall amplitude\n"
   << "' ' = display snapshot of center frequency and bandwidth\n"
   << endl;
}

void initialization(void) { 
   // sound element connections
   outsound.connect(mul);         // left channel for stereo
   outsound.connect(mul);         // right channel for stereo
   mul.connect(reson);            // connections point to upstream generators
   mul.connect(amplitude);        //  "
   reson.connect(whitenoise, 0);  //  attach whitenoise to input 0
   reson.connect(frequency, 1);   //  attach frequency signal to input 1
   reson.connect(bandwidth, 2);   //  attach bandwidth signal to input 2

   action.freezeState(outsound);  // take a picture of current connections
                                  // for later sound extraction.

   poledisplay.setPeriod(200);    // display position data 5 times a second
   eventIdler.setPeriod(0);       // turn off multiprocessor niceness
   soundTimer.setPeriod(1);       // calculate a sound chunk every 1 ms.

/* Note: the Sound output is not very stable yet, so I turn off
   the eventIdling (which was added last month to improv for 
   Linux multiprocess environment).  The output sound will block
   automatically if you try to give it too much data, so there is
   no danger of the sound getting out of sync with the input controllers.
*/

}


void finishup(void) { }


/*-------------------- main loop algorithms -----------------------------*/

//////////////////////////////
//
// getradius -- calculate a z-plane radius from the x- and y- axes.
//

double getradius(int x, int y) {
   double xr = x/125.0 * 2 - 1;
   double yr = y/125.0 * 2 - 1;
   double output = sqrt(xr*xr + yr*yr);
   if (output > 1.0) {
      output = 0.9999;
   }

   // stretch the outer radius values to get better intuitive control of sound
   output = 1 - pow(output - 1, 2);

   return output;
}



//////////////////////////////
//
// getangle -- calculate a z-plane angle from the x- and y- axes.
//

double getangle(int x, int y) {
   double xr = x/127.0 * 2 - 1;
   double yr = y/127.0 * 2 - 1;
   return atan2(yr,xr);
}

  
double angle;
double radius;
double amp;

void mainloopalgorithms(void) { 

   // the switch statement is used to calculate the correct
   // center frequency and bandwidth according to the plane
   // you have selected (z-plane or s-plane).
   // the amplitude algorithm is for the Radio Drum, and may not
   // be too useful for other types of controllers.  The radio
   // druM Z-AXIS is useable from values around 60 to 127.
   // Also, the cfreq and bandwidth are being recalculated way
   // too often at this time, and should be restrained in a more
   // elegant program.
   switch (mode) {
      case MODE_S:
         if (synth.controller(zaxis) < 63) {
            amp = 0;
         } else { 
            amp    = (synth.controller(zaxis)/127.0 - 0.5) * 2;
         }
         amp = 1;

         amplitude.setValue(amp);
         frequency.setValue(midiscale(synth.controller(xaxis), 
              MINCFREQ, MAXCFREQ));
         bandwidth.setValue(midiscale(synth.controller(yaxis), 
              MINBANDW, MAXBANDW));
         break;
      case MODE_Z:
         angle  = getangle(synth.controller(xaxis), synth.controller(yaxis));
         radius = getradius(synth.controller(xaxis), synth.controller(yaxis));
         if (synth.controller(zaxis) < 63) {
            amp = 0;
         } else {
            amp    = (synth.controller(zaxis)/127.0 - 0.5) * 2;
            amp    *= (pow(radius, 18) + 0.5) * 10;
         }
         amp = 1;

         amplitude.setValue(amp);
         frequency.setValue(fabs(angle)*1000);
         bandwidth.setValue((1.001-radius)*3000);
         break;
   }


   // give the user some feedback on the position of the input controller
   // as it relates to the output sound if they press the 'p' key.
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

   // if it is time to calculate a sound chunk, then do so.
   // each chunk of sound has 512 samples.
   if (soundTimer.expired()) {
      soundTimer.reset();
      action.tickFrozenState(512);
   }
}


/*-------------------- triggered algorithms -----------------------------*/

void keyboardchar(int key) { 
   switch (key) {
      case 'p':               // toggle display of current sound info.
         poledisplayQ = !poledisplayQ;
         if (poledisplayQ) {
            cout << "position display ON" << endl;
         } else {
            cout << "\nposition display OFF" << endl;
         }
         break;
      case 'd':                // direction of xyz keys to select controller
         contdirection *= -1;
         cout << "controller selection direction = " << contdirection << endl;
         break;
      case 'x':                // x-axis controller number
         xaxis += contdirection;
         xaxis &= 0x7f;
         cout << "X-axis controller is: " << xaxis << endl;
         break;
      case 'y':                // y-axis controller number
         yaxis += contdirection;
         yaxis &= 0x7f;
         cout << "Y-axis controller is: " << yaxis << endl;
         break;
      case 'z':                // z-axis controller number
         zaxis += contdirection;
         zaxis &= 0x7f;
         cout << "Z-axis controller is: " << zaxis << endl;
         break;
      case 's':                // s-plane coordinate systems for resonator
         if (mode == MODE_S) {
            mode = MODE_Z;
            cout << "\nz-plane mode" << endl;
         } else {
            mode = MODE_S;
            cout << "\ns-plane mode" << endl;
         }
         break;
      case ' ':                // display the current cfreq/bandw
         cout << "\nCenter: " << frequency.output(0) << "Hz"
              << "      Bandwidth: " << bandwidth.output(0) 
              << endl;
         break;
   }
}

/*------------------ end improvization algorithms -----------------------*/



