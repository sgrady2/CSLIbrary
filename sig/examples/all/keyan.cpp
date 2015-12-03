//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun May 16 12:54:28 PDT 1999
// Last Modified: Sun May 16 16:06:30 PDT 1999
// Filename:      .../improv/examples/synthImprov/keyan/keyan.cpp
// Syntax:        C++; synthImprov 2.1
//  
// Description:   analyze the key of musical input and output the determined
//                key as a low note
//
// References:    Key analysis method taken from the "key" program of Humdrum
//                The Key analysis technique is described in the book:
//                Krumhansel & Kessler
//


#include "synthImprov.h"      
#include <math.h>
#include <iomanip.h>
#include <iostream.h>


/*----------------- beginning of improvization algorithms ---------------*/


MidiMessage message;            // for extracting notes from the synthesizer

CircularBuffer<char> notes;     // storage for notes being played
CircularBuffer<long> times;     // storage for note times being played
double occurances[12] = {0};    // number of notes occurances
int fadeNote = 0;               // next to to go out of scope
int fadeTime = 0;               // next time to go out of scope
SigTimer metronome;             // for display period of key analysis
double analysisDuration;        // duration in seconds of analysis window
double tempo;                   // tempo of the keyanalysis
int keyoctave = 7;              // the analysis key performance octave
int displayKey2 = 0;            // display the second key possibility

double majorKey[12];            // weights for analysis of major keys
double minorKey[12];            // weights for analysis of minor keys

Voice  firstVoice;              // for primary key voice
Voice  secondVoice;             // for secondary key voice
double offFraction = 0.75;      // for turning off next keyvoice.

// function declarations:
void        analyzekey         (void);
void        storeNote          (char storeNote, long storeTime);
const char* name               (int pitch_class);


/*--------------------- maintenance algorithms --------------------------*/


//////////////////////////////
//
// description -- this function is called by the improv interface
//     whenever a capital "D" is pressed on the computer keyboard.
//     Put a description of the program and how to use it here.
//

void description(void) {
   cout << 
   "Keyan 1 -- analyze musical input for key\n"
   << endl;

} 



//////////////////////////////
//
// initialization -- this function is called by the improv
//     intervace once at the start of the program.  Put items
//     here which need to be initialized at the beginning of
//     the program.
//

void initialization(void) { 
   analysisDuration = 7.0;          // duration in seconds of analysis window
   tempo = 60.0;                     // tempo of the keyanalysis
   metronome.setTempo(tempo);
   firstVoice.setChannel(0);
   secondVoice.setChannel(0);
   notes.setSize(10000);
   times.setSize(10000);

   // initialize majorKey and minorKey arrays
   // The tonality coefficients listed here were determined by 
   // Krumhansl & Kessler (1982) and are defined for both 
   // the major and minor keys.
   majorKey[0]  = 6.35;       minorKey[0]  = 6.33;
   majorKey[1]  = 2.23;       minorKey[1]  = 2.68;
   majorKey[2]  = 3.48;       minorKey[2]  = 3.52;
   majorKey[3]  = 2.33;       minorKey[3]  = 5.38;
   majorKey[4]  = 4.38;       minorKey[4]  = 2.60;
   majorKey[5]  = 4.09;       minorKey[5]  = 3.53;
   majorKey[6]  = 2.52;       minorKey[6]  = 2.54;
   majorKey[7]  = 5.19;       minorKey[7]  = 4.75;
   majorKey[8]  = 2.39;       minorKey[8]  = 3.98;
   majorKey[9]  = 3.66;       minorKey[9]  = 2.69;
   majorKey[10] = 2.29;       minorKey[10] = 3.34;
   majorKey[11] = 2.88;       minorKey[11] = 3.17;
}



//////////////////////////////
//
// finishup -- this function is called by the improv interface
//     whenever the program is exited.  Put items here which
//     need to be taken care of when the program is finished.
//

void finishup(void) { 
   firstVoice.off();
   secondVoice.off();
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
   while (synth.getNoteCount() > 0) {
      message = synth.extractNote();
      if (message.p2() != 0) {
         storeNote(message.p1(), t_time);
      }
   }

   if (metronome.expired()) {
      analyzekey();
      metronome.reset();
   }

   if (metronome.getPeriodCount() > offFraction) {
      firstVoice.off();
      secondVoice.off();
   }

}
      

/*-------------------- triggered algorithms -----------------------------*/


///////////////////////////////
//
// keyboardchar -- this function is called by the improv interface
//     whenever a key is pressed on the computer keyboard.
//     Put commands here which will be executed when a key is
//     pressed on the computer keyboard.
//

void keyboardchar(int key) { 
   switch (key) {

      case '2':            // for playing the secondary key possibility
         displayKey2 = !displayKey2;         
         if (displayKey2) {
            cout << "Secondary key output turned ON" << endl;
         } else {
            cout << "Secondary key output turned OFF" << endl;
         }
         break;

      case ',':            // slow down the tempo which will be
         tempo /= 1.05;    // activated at the next note to be played
         if (tempo < 20) {
            tempo = 20;
         }
         metronome.setTempo(tempo);
         cout << "Tempo is: " << tempo << endl;
         break;

      case '.':            // speed up the tempo which will be
         tempo *= 1.05;    // activated at the next note to be played
         if (tempo < 240) {
            tempo = 240;
         }
         metronome.setTempo(tempo);
         cout << "Tempo is: " << tempo << endl;
         break;

   }
}


/*------------------ begining of assistant functions --------------------*/



//////////////////////////////
//
// analyzekey -- figure out the key possibilities and then play the
//   best two choices.
//

void analyzekey(void) { 
   int i;

   // first, adjust the number of notes being analyzed
   while (times.getCount() > 0 && fadeTime < t_time - analysisDuration * 1000) {
      if (fadeTime != 0) {
         occurances[fadeNote%12]--;
      }
      fadeTime = times.extract();
      fadeNote = notes.extract();
   }
      
   // now analyze the keys
   double total = 0;
   cout << "Count:";
   for (i=0; i<12; i++) {
      cout.width(3);
      cout << occurances[i];
      total += occurances[i];
   }
   cout << flush;
   
   // Determine the mean frequency for the pitch distribution
   double mean       = total / 12.0;
   double major_mean = 3.4825;
   double minor_mean = 3.709167;

   if (total <= 0) {
      cout << endl;
      return;
   }
      
   double r_major[12];
   double r_minor[12];
   double maj_numerator;
   double min_numerator;
   double maj_denominator;
   double min_denominator;
   double maj_temp;
   double min_temp;
   double temp;
   int subscript;
   int j;
   for (i=0; i<12; i++) {
      maj_numerator = min_numerator = 0;
      maj_denominator = min_denominator = 0;
      maj_temp = min_temp = temp = 0;

      // Examine all pitches for each key.
      for (j=0; j<12; j++) {
         subscript = (i+j) % 12;

         temp += (occurances[subscript] - mean) *
                 (occurances[subscript] - mean);

         // For the major keys.
         maj_numerator += (majorKey[j]-major_mean) * 
                          (occurances[subscript]-mean);
         maj_temp += (majorKey[j] - major_mean) * 
                     (majorKey[j] - major_mean);
         
         // For the minor keys.
         min_numerator += (minorKey[j]-minor_mean) * 
                          (occurances[subscript]-mean);
         min_temp += (minorKey[j] - minor_mean) *
                     (minorKey[j] - minor_mean);

      }

      maj_denominator = sqrt(maj_temp * temp);
      min_denominator = sqrt(min_temp * temp);
      
      if (maj_denominator == 0 || min_denominator == 0) {
         return;
      }

      r_major[i] = maj_numerator / maj_denominator;
      r_minor[i] = min_numerator / min_denominator;
   }

   // Now determine which correlation is the greatest.
   // Start off with the assumption that C major is the best key.
   double best_key = 0.0; 
   char* mode = "unknown";
   int pitch_class = 0;            // tonic note of best key
   
   // Compare all the remaining key correlations.
   for (i=0; i<12; i++) {
      if (r_major[i] > best_key) {
         best_key = r_major[i]; 
         mode = "major"; 
         pitch_class = i;
      }
      if (r_minor[i] > best_key) {
         best_key = r_minor[i]; 
         mode = "minor"; 
         pitch_class = i;
      }
   }

   // A confidence measure can be determined by taking the difference
   // between the correlation for the "best key" and subtracting the
   // correlation for the "second best key".  The maximum confidence
   // score is 100; the minimum is zero.
   // First, having found the "best key", find the "second best key."
   double second_best_key = 0;
   char* secondmode = "unknown";
   int sec_pitch_class = 0;
   for (i=0; i<12; i++) {
      if (r_major[i] != best_key && r_major[i] > second_best_key) {
         second_best_key = r_major[i];
         secondmode = "major";
         sec_pitch_class = i;
      }
      if (r_minor[i] != best_key && r_minor[i] > second_best_key) {
         second_best_key = r_minor[i];
         secondmode = "minor";
         sec_pitch_class = i;
      }
   }

   // The value 3.0 below is a scaling factor.
   double confidence = (best_key - second_best_key) * 100 * 3.0;
   if (confidence > 100.0) confidence = 100.0;
   
   // Print the analysis results:
   cout << "   Key: " << name(pitch_class) << " " << mode 
        << " (";
   cout.width(3);
   cout << (int)confidence << "%), or " 
        << name(sec_pitch_class) << " " << secondmode << endl;

   // finally, play the determined keys according to confidence
   int velocity = (int)(127.0 * confidence/100.0);
   
   firstVoice.play(pitch_class + 12 * keyoctave, velocity);
   if (displayKey2) {
      secondVoice.play(sec_pitch_class + 12 * keyoctave, 127 - velocity);
   }

}


//////////////////////////////
//
// name -- return the name of the input pitch class
//

const char* name(int pitch_class) {
   pitch_class = pitch_class % 12;
   switch (pitch_class) {
      case 0:   return "C ";
      case 1:   return "C#";
      case 2:   return "D ";
      case 3:   return "Eb";
      case 4:   return "E ";
      case 5:   return "F ";
      case 6:   return "F#";
      case 7:   return "G ";
      case 8:   return "Af";
      case 9:   return "A ";
      case 10:  return "Bf";
      case 11:  return "B ";
   }
   
   return "unknown";
}
   




//////////////////////////////
//
// storeNote -- store the next note
//

void storeNote(char storeNote, long storeTime) {
   notes.insert(storeNote);
   times.insert(storeTime);
   occurances[storeNote%12]++;
}



/*------------------ end improvization algorithms -----------------------*/



