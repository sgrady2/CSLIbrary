//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Apr  5 16:42:31 PDT 2000
// Last Modified: Wed Apr  5 16:42:35 PDT 2000
// Filename:      ...improv/doc/examples/batonImprov/tempojnd/tempojnd.cpp
// Syntax:        C++; sig 1.0.0
//  
// Description: Program for running an tempo JND experiment.
//

#include "sig.h"

// function declarations
void checkOptions(Options& opts);
void example(void);
void usage(const char* command);


// sound elements

Wavetable     click;
Multiply      scale;
Constant      volume(1.0);

LinuxSoundOut outsound(44100, 2);

Action        action;
SigTimer      soundTimer;
Options       options;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) { 
   options.setOptions(argc, argv);
   checkOptions(options);
   initialize();

   while (1) {
      if (soundTimer.expired()) {
         soundTimer.reset();
         action.tick(outsound, 512);
      }
   }
}


///////////////////////////////////////////////////////////////////////////



//////////////////////////////
//
// checkOptions -- check the command line options
//

void checkOptions(Options& opts) {
   options.define("click|i|input=s:"click.wav");
   options.process();

   if (opts.getBoolean("author")) {
      cout << "Written by Craig Stuart Sapp, "
           << "craig@ccrma.stanford.edu, April 2000" << endl;
      exit(0);
   }
   if (opts.getBoolean("version")) {
      cout << "compiled: " << __DATE__ << endl;
      cout << SIG_VERSION << endl;
      exit(0);
   }
   if (opts.getBoolean("help")) {
      usage(opts.getCommand());
      exit(0);
   }
   if (opts.getBoolean("example")) {
      example();
      exit(0);
   }             




}



//////////////////////////////
//
// description --
//

void description(void) {
   cout 
   << "Initial test program for tempo JND experiment\n";
   << endl;
}



//////////////////////////////
//
// example -- show examples of how to use this program.
//

void example(void) {
   // no examples for now
}



//////////////////////////////
//
// initialization -- setup the program
//

void initialization(void) { 
   
   SoundFileIn inputsound(options.getString("click"));
   int samples = inputsound.getSamples();

   click.setSize(samples);

   // sound element connections

   scale.connect(click);
   scale.connect(volume);
   outsound.connect(scale, 0, 0);
   outsound.connect(scale, 0, 1);

   action.freezeState(outsound);
   eventIdler.setPeriod(0);
   soundTimer.setPeriod(1);
}


void finishup(void) { }



//////////////////////////////
//
// keyboardchar -- process functions for computer keyboard keys.
//

void keyboardchar(int key) { 

}



//////////////////////////////
//
// usage -- process functions for computer keyboard keys.
//

void usage(const char* command) {

}


