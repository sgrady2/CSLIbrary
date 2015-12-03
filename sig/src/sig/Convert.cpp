//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Jun  8 00:38:46 PDT 1998
// Last Modified: Tue Jun 23 14:00:23 PDT 1998
// Last Modified: Fri May  5 12:18:44 PDT 2000 (added kernToMidiNoteNumber())
// Last Modified: Tue Jun  6 13:51:55 PDT 2000 (durationToKernRhythm() improved)
// Filename:      ...humdrum++/src/Convert.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -I../include -g -c %f && rm %b.o
//


#include "Convert.h"
#include "HumdrumEnumerations.h"
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#ifdef VISUAL
   #include <strstrea.h>        /* for Windows 95 */
#else
   #include <strstream.h>
#endif


EnumerationExclusiveInterpretation Convert::exint;
EnumerationChordQualityType        Convert::chordType;
EnumerationChordQualityInversion   Convert::chordInversion;
EnumerationChordQualityRoot        Convert::kernPitchClass;
EnumerationMusePitchClass          Convert::musePitchClass;
EnumerationInterval                Convert::intervalNames;


///////////////////////////////////////////////////////////////////////////
//
// conversions dealing with humdrum data
//




///////////////////////////////////////////////////////////////////////////
//
// conversions dealing with **kern data
//

//////////////////////////////
//
// Convert::kernToMidiNoteNumber -- -1 means a rest or other negative
//    number; otherwise the notes should end up in the range from 0 to 127.
//

int Convert::kernToMidiNoteNumber(const char* aKernString) {
   int base40 = Convert::kernToBase40(aKernString);
   if (base40 < 0) {
      return -1;
   }

   int octave = base40 / 40;
   int pitch = 0;
   switch (base40 % 40) {
      case  0: pitch = -2;     break;   // C--
      case  1: pitch = -1;     break;   // C-
      case  2: pitch = 0;      break;   // C
      case  3: pitch = 1;      break;   // C#
      case  4: pitch = 2;      break;   // C##

      case  6: pitch = 0;      break;   // D--
      case  7: pitch = 1;      break;   // D-
      case  8: pitch = 2;      break;   // D
      case  9: pitch = 3;      break;   // D#
      case 10: pitch = 4;      break;   // D##

      case 12: pitch = 2;      break;   // E--
      case 13: pitch = 3;      break;   // E-
      case 14: pitch = 4;      break;   // E
      case 15: pitch = 5;      break;   // E#
      case 16: pitch = 6;      break;   // E##

      case 17: pitch = 3;      break;   // F--
      case 18: pitch = 4;      break;   // F-
      case 19: pitch = 5;      break;   // F
      case 20: pitch = 6;      break;   // F#
      case 21: pitch = 7;      break;   // F##

      case 23: pitch = 5;      break;   // G--
      case 24: pitch = 6;      break;   // G-
      case 25: pitch = 7;      break;   // G
      case 26: pitch = 8;      break;   // G#
      case 27: pitch = 9;      break;   // G##

      case 29: pitch = 7;      break;   // A--
      case 30: pitch = 8;      break;   // A-
      case 31: pitch = 9;      break;   // A
      case 32: pitch = 10;     break;   // A#
      case 33: pitch = 11;     break;   // A##

      case 35: pitch =  9;     break;   // B--
      case 36: pitch = 10;     break;   // B-
      case 37: pitch = 11;     break;   // B
      case 38: pitch = 12;     break;   // B#
      case 39: pitch = 13;     break;   // B##

      default: 
      cout << "Pitch Unknown: " << base40 % 40 << endl;
      pitch = -1000;
   }
  
   return octave * 12 + pitch + 10;
}



//////////////////////////////
//
// Convert::durationToKernRhythm -- not allowed to have more than
//	two rhythmic dots
//	default value: timebase = 1;
//

char* Convert::durationToKernRhythm(char* output, double input, int timebase) {
   strstream temp;
   double testinput = input;
   double basic = 4.0 / input * timebase;
   double diff = basic - (int)basic;

   if (diff > 0.998) {
      diff = 1.0 - diff;
      basic += 0.002;
   }

   if (diff < 0.002) {
      temp << (int)basic << ends;
      strcpy(output, temp.str());
   } else { 
      testinput = input / 3.0 * 2.0;
      basic = 4.0 / testinput;
      diff = basic - (int)basic;
      if (diff < 0.002) {
         temp << (int)basic << ends;
         strcpy(output, temp.str());
         strcat(output, ".");
      } else {
         testinput = input / 7.0 * 4.0;
         basic = 4.0 / testinput;
         diff = basic - (int)basic;
         if (diff < 0.002) {
            temp << (int)basic << ends;
            strcpy(output, temp.str());
            strcat(output, "..");
         } else {
            cerr << "Error: Convert::durationToKernRhythm choked on the "
                 << "duration: " << input << endl; 
            exit(1);
         }
      }
   }

   return output;
}



//////////////////////////////
//
// Convert::kernToDuration -- returns the kern rhythm's duration, using
//	1.0 as the duration of a quarter note (rhythm=4).
//

double Convert::kernToDuration(const char* aKernString) {

   int index = 0;
   while (aKernString[index] != '\0' && !isdigit(aKernString[index])) {
      index++;
   }
   if (aKernString[index] == '\0') {
      // no rhythm data found
      return 0;
   }
   
   // should now be at start of kern rhythm
   int orhythm = 0;
   while (aKernString[index] != '\0' && isdigit(aKernString[index])) {
      orhythm *= 10;
      orhythm += aKernString[index] - '0';
      index++;
   }

   // check for dots to modify rhythm
   int dotcount = 0;
   while (aKernString[index] != '\0' && aKernString[index] == '.') {
      dotcount++;
      index++;
   }

   // now know everything to create a duration
   double oduration;
   if (orhythm == 0) {
      oduration = 8.0;
   } else {
      oduration = 4.0 / orhythm;
   }
   double duration = oduration;
   for (int i=0; i<dotcount; i++) {
      duration += oduration * pow(2, -(i+1));
   }
      
   return duration;
}



//////////////////////////////
//
// Convert::kernTimeSignatureBottom -- returns the rhythm value
//	in the bottom part of a time signature
//

double Convert::kernTimeSignatureBottomToDuration (const char* aKernString) {
   char* slash = strchr(aKernString, '/');
   if (slash == NULL) {
      cerr << "Error: poorly formed time signature: " << aKernString << endl;
      exit(1);
   } else {
      return Convert::kernToDuration(&slash[1]);
   }
}



//////////////////////////////
//
// Convert::kernTimeSignatureTop -- returns the number of beats
//	in the top part of a time signature
//

double Convert::kernTimeSignatureTop (const char* aKernString) {
   if (strchr(aKernString, '+') != NULL) {
      cerr << "Error: cannot handle time signature: " << aKernString
           << " yet." << endl;
      exit(1);
   } else {
      return (double)atoi(&aKernString[1]);
   }
}



//////////////////////////////
//
// Convert::musePitchToKernPitch
//

char* Convert::musePitchToKernPitch(char* kernOutput, const char* museInput) {
   base40ToKern(kernOutput, museToBase40(museInput));
   return kernOutput;
}



//////////////////////////////
//
// Convert::museClefToKernClef
//

char* Convert::museClefToKernClef(char* kernOutput, int museInput) {
   switch (museInput) {
      case 0:   strcpy(kernOutput, "GX");       break;
      case 1:   strcpy(kernOutput, "G5");       break;
      case 2:   strcpy(kernOutput, "G4");       break;
      case 3:   strcpy(kernOutput, "G3");       break;
      case 4:   strcpy(kernOutput, "G2");       break;
      case 5:   strcpy(kernOutput, "G1");       break;
      
      case 10:  strcpy(kernOutput, "CX");       break;
      case 11:  strcpy(kernOutput, "C5");       break;
      case 12:  strcpy(kernOutput, "C4");       break;
      case 13:  strcpy(kernOutput, "C3");       break;
      case 14:  strcpy(kernOutput, "C2");       break;
      case 15:  strcpy(kernOutput, "C1");       break;
      
      case 20:  strcpy(kernOutput, "FX");       break;
      case 21:  strcpy(kernOutput, "F5");       break;
      case 22:  strcpy(kernOutput, "F4");       break;
      case 23:  strcpy(kernOutput, "F3");       break;
      case 24:  strcpy(kernOutput, "F2");       break;
      case 25:  strcpy(kernOutput, "F1");       break;
      
      case 30:  strcpy(kernOutput, "GvX");       break;
      case 31:  strcpy(kernOutput, "Gv5");       break;
      case 32:  strcpy(kernOutput, "Gv4");       break;
      case 33:  strcpy(kernOutput, "Gv3");       break;
      case 34:  strcpy(kernOutput, "Gv2");       break;
      case 35:  strcpy(kernOutput, "Gv1");       break;
      
      case 40:  strcpy(kernOutput, "CvX");       break;
      case 41:  strcpy(kernOutput, "Cv5");       break;
      case 42:  strcpy(kernOutput, "Cv4");       break;
      case 43:  strcpy(kernOutput, "Cv3");       break;
      case 44:  strcpy(kernOutput, "Cv2");       break;
      case 45:  strcpy(kernOutput, "Cv1");       break;
      
      case 50:  strcpy(kernOutput, "FvX");       break;
      case 51:  strcpy(kernOutput, "Fv5");       break;
      case 52:  strcpy(kernOutput, "Fv4");       break;
      case 53:  strcpy(kernOutput, "Fv3");       break;
      case 54:  strcpy(kernOutput, "Fv2");       break;
      case 55:  strcpy(kernOutput, "Fv1");       break;
      
      case 60:  strcpy(kernOutput, "G^X");       break;
      case 61:  strcpy(kernOutput, "G^5");       break;
      case 62:  strcpy(kernOutput, "G^4");       break;
      case 63:  strcpy(kernOutput, "G^3");       break;
      case 64:  strcpy(kernOutput, "G^2");       break;
      case 65:  strcpy(kernOutput, "G^1");       break;
      
      case 70:  strcpy(kernOutput, "C^X");       break;
      case 71:  strcpy(kernOutput, "C^5");       break;
      case 72:  strcpy(kernOutput, "C^4");       break;
      case 73:  strcpy(kernOutput, "C^3");       break;
      case 74:  strcpy(kernOutput, "C^2");       break;
      case 75:  strcpy(kernOutput, "C^1");       break;
      
      case 80:  strcpy(kernOutput, "F^X");       break;
      case 81:  strcpy(kernOutput, "F^5");       break;
      case 82:  strcpy(kernOutput, "F^4");       break;
      case 83:  strcpy(kernOutput, "F^3");       break;
      case 84:  strcpy(kernOutput, "F^2");       break;
      case 85:  strcpy(kernOutput, "F^1");       break;
      default:  strcpy(kernOutput, "X");         
   }

   return kernOutput;
}



///////////////////////////////////////////////////////////////////////////
//
// conversions dealing with **qual data
//

//////////////////////////////
//
// Convert::chordQualityStringToValue
//

ChordQuality Convert::chordQualityStringToValue(const char* aString) {

   ChordQuality output;
   char* temp;
   char* token;
   temp = new char[strlen(aString)+1];
   strcpy(temp, aString);
   token = strtok(temp, ":");
   for (int i=0; i<3; i++) {
      // determine what the current token is
      if (token == NULL) {
         break;
      } else if (Convert::chordType.memberQ(token)) {
         output.setType(token);
      } else if (Convert::chordInversion.memberQ(token)) {
         output.setInversion(token);
      } else if (Convert::kernPitchClass.memberQ(token)) {
         output.setRoot(token);
      } else {
         cerr << "Error: unknown chord value: " << token << endl;
         exit(1);
      }
   }
   delete [] temp;
   return output;
}



//////////////////////////////
//
// Convert::chordQualityToBaseNote
//

int Convert::chordQualityToBaseNote(const ChordQuality& aQuality) {

   Collection<int> notes;
   notes = chordQualityToNoteSet(aQuality);
   return notes[aQuality.getInversion()] % 40;
}



//////////////////////////////
//
// Convert::chordQualityToNoteSet
//

Collection<int> Convert::chordQualityToNoteSet(const ChordQuality& aQuality) {

   Collection<int> output;
   output.setSize(0);
   output.allowGrowth();
   output[0] = 0;
   
   switch (aQuality.getType()) {
      case E_chord_note:  break;
      case E_chord_incmin: output[1] = 11; break;
      case E_chord_incmaj: output[1] = 12; break;
      case E_chord_secsev:
      case E_chord_dim:    output[1] = 11; output[2] = 22; break;
      case E_chord_min:    output[1] = 11; output[2] = 23; break;
      case E_chord_neopol: 
      case E_chord_secdom:
      case E_chord_maj:    output[1] = 12; output[2] = 23; break;
      case E_chord_aug:    output[1] = 12; output[2] = 24; break;
      
      case E_chord_secsevo:
      case E_chord_fullydim: output[1] = 11; output[2] = 22; output[3] = 33;
                             break;
      case E_chord_secsevc:
      case E_chord_halfdim:  output[1] = 11; output[2] = 22; output[3] = 34;
                             break;
      case E_chord_minmin:   output[1] = 11; output[2] = 23; output[3] = 34;
                             break;
      case E_chord_minmaj:   output[1] = 11; output[2] = 23; output[3] = 35;
                             break;
      case E_chord_secdomsev:
      case E_chord_domsev:   output[1] = 12; output[2] = 23; output[3] = 34;
                             break;
      case E_chord_majmaj:   output[1] = 12; output[2] = 23; output[3] = 35;
                             break;
      case E_chord_frensix:  output[1] = 12; output[2] = 18; output[3] = 30;
                             break;
      case E_chord_germsix:  output[1] = 12; output[2] = 23; output[3] = 30;
                             break;
      case E_chord_italsix:  output[1] = 12; output[2] = 30; break;
      default:
         cerr << "Error: unknown type of chord quality: " << aQuality.getType()
              << endl;
   }

   // add bass offset
   for (int i=0; i<output.getSize(); i++) {
      output[i] += aQuality.getRoot();
      output[i] %= 40;
   }

   return output;
}



//////////////////////////////
//
// Convert::chordQualityToInversion
//

int Convert::chordQualityToInversion(const char* aQuality) {

   ChordQuality conversion = chordQualityStringToValue(aQuality);
   return conversion.getInversion();
}



//////////////////////////////
//
// Convert::chordQualityToRoot
//

int Convert::chordQualityToRoot(const char* aQuality) {

   ChordQuality conversion = chordQualityStringToValue(aQuality);
   return conversion.getRoot();
}



//////////////////////////////
//
// Convert::chordQualityToType
//

int Convert::chordQualityToType(const char* aQuality) {

   ChordQuality conversion = chordQualityStringToValue(aQuality);
   return conversion.getType();
}



//////////////////////////////
//
// Convert::noteSetToChordQuality
//

ChordQuality Convert::noteSetToChordQuality(const Collection<int>& 
      aSet) {

   ChordQuality output;
   Collection<int> newnotes(aSet.getSize());
   int i, k;

   // remove rests and find the lowest note
   int index = 0;
   for (i=0; i<aSet.getSize(); i++) {
      if (aSet[i] > E_root_rest) {
         newnotes[index] = aSet[i];
         index++;
      }
   }
   if (index == 0) {
      output.setType(E_chord_rest);
      output.setInversion(E_blank);
      output.setRoot(E_blank);
      return output;
   } else {
      newnotes.setSize(index);
   }

   qsort(newnotes.getBase(), newnotes.getSize(), sizeof(int), intcompare);
   int basenote = BASE40_PITCHCLASS(newnotes[0]);


   // remove octave values
   for (i=0; i<newnotes.getSize(); i++) {
      newnotes[i] = BASE40_PITCHCLASS(newnotes[i]);
   }
   qsort(newnotes.getBase(), newnotes.getSize(), sizeof(int), intcompare);

   // remove redundant notes
   int uniqcount = newnotes.getSize();
   for (i=1; i<uniqcount; i++) {
      while (newnotes[i] == newnotes[i-1] && i < uniqcount) {
         for (k=i-1; k<uniqcount-1; k++) {
            newnotes[k] = newnotes[k+1];
         }
         uniqcount--;
      }
   }
   newnotes.setSize(uniqcount);

   int root = E_unknown;
   int chordFound = E_unknown;
   for (i=0; i<uniqcount; i++) {
      chordFound = checkChord(newnotes);
      if (chordFound != E_unknown) {
         root = newnotes[0];
         break;
      } else {
         rotatechord(newnotes);
      }
   }

   output.setType(chordFound);

   if (chordFound == E_unknown) {
      output.setInversion(E_unknown);
      output.setRoot(E_unknown);
   } else {
      output.setInversion(calculateInversion(chordFound, basenote, root));
      output.setRoot(root % 40);
   }

   return output;
}



///////////////////////////////////////////////////////////////////////////
//
// conversions dealing with base 40 system of notation
//



//////////////////////////////
//
// Convert::base40ToKern
//

char* Convert::base40ToKern(char* output, int aPitch) {
   int octave = aPitch / 40;
   if (octave > 12 || octave < -1) {
      cerr << "Error: unresonable octave value: " << octave << endl;
      exit(1);
   }
   int chroma = aPitch % 40;

   strcpy(output, kernPitchClass.getName(chroma));
   if (octave >= 4) {
      output[0] = tolower(output[0]);
   } else {
      output[0] = toupper(output[0]);
   }
   int repeat = 0;
   switch (octave) {
      case 4:  repeat = 0; break;
      case 5:  repeat = 1; break;
      case 6:  repeat = 2; break;
      case 7:  repeat = 3; break;
      case 8:  repeat = 4; break;
      case 9:  repeat = 5; break;
      case 3:  repeat = 0; break;
      case 2:  repeat = 1; break;
      case 1:  repeat = 2; break;
      case 0:  repeat = 3; break;
      case -1: repeat = 4; break;
      default:
         cerr << "Error: unknown octave value: " << octave << endl;
         exit(1);
   }
   if (repeat == 0) {
      return output;
   }

   int length = strlen(output);
   output[length + repeat] = '\0';
   int i; 
   for (i=length-1; i>=0; i--) {
      output[i+repeat] = output[i];
   }
   for (i=0; i<repeat; i++) {
      output[i+1] = output[0];
   }

   return output;
}

//////////////////////////////
//
// Convert::base40ToKernTranspose -- returns the transpose
//	string value (c.f. p221 of Humdrum Reference (1994)
//

char* Convert::base40ToKernTranspose(char* output, int transpose, 
      int keysignature) {
   int keyacc;
   int keydia;
   int oldbase;
   int newacc;
   int newdia;

   switch (keysignature) {
      case  0:  keyacc = 0;   keydia = 0;  oldbase = 0;  break;  // C 
      case  1:  keyacc = 0;   keydia = 4;  oldbase = 23; break;  // G 
      case  2:  keyacc = 0;   keydia = 1;  oldbase = 6;  break;  // D 
      case  3:  keyacc = 0;   keydia = 5;  oldbase = 29; break;  // A 
      case  4:  keyacc = 0;   keydia = 2;  oldbase = 12; break;  // E 
      case  5:  keyacc = 0;   keydia = 6;  oldbase = 35; break;  // B 
      case  6:  keyacc = +1;  keydia = 3;  oldbase = 18; break;  // F#
      case  7:  keyacc = +1;  keydia = 0;  oldbase = 1;  break;  // C#
      case -1:  keyacc = 0;   keydia = 3;  oldbase = 17; break;  // F 
      case -2:  keyacc = -1;  keydia = 6;  oldbase = 34; break;  // Bb
      case -3:  keyacc = -1;  keydia = 2;  oldbase = 11; break;  // Eb
      case -4:  keyacc = -1;  keydia = 5;  oldbase = 28; break;  // Ab
      case -5:  keyacc = -1;  keydia = 1;  oldbase = 5;  break;  // Db
      case -6:  keyacc = -1;  keydia = 4;  oldbase = 22; break;  // Gb
      case -7:  keyacc = -1;  keydia = 0;  oldbase = -1; break;  // Cb
      default:  keyacc = 0;   keydia = 0;  oldbase = 0;  break;  
   }
  
   int newbase = (oldbase + transpose + 400) % 40;

   switch (newbase) {
      case  0:  newacc = 0;   newdia = 0;  break;  // C 
      case 23:  newacc = 0;   newdia = 4;  break;  // G 
      case  6:  newacc = 0;   newdia = 1;  break;  // D 
      case 29:  newacc = 0;   newdia = 5;  break;  // A 
      case 12:  newacc = 0;   newdia = 2;  break;  // E 
      case 35:  newacc = 0;   newdia = 6;  break;  // B 
      case 18:  newacc = +1;  newdia = 3;  break;  // F#
      case  1:  newacc = +1;  newdia = 0;  break;  // C#
      case 17:  newacc = 0;   newdia = 3;  break;  // F 
      case 34:  newacc = -1;  newdia = 6;  break;  // Bb
      case 11:  newacc = -1;  newdia = 2;  break;  // Eb
      case 28:  newacc = -1;  newdia = 5;  break;  // Ab
      case  5:  newacc = -1;  newdia = 1;  break;  // Db
      case 22:  newacc = -1;  newdia = 4;  break;  // Gb
      case -1:  newacc = -1;  newdia = 0;  break;  // Cb
      default:  newacc = 0;   newdia = 0;  break;  
   }

   int chrom = newacc - keyacc;
   int diaton = newdia - keydia;
   if (diaton < 0) {
      diaton += 7;
   }
   if (transpose < 0) {
      diaton -= 7;
//      diaton -= 7 * (abs(transpose) % 40);
   } else {
//      diaton += 7 * (abs(transpose) % 40);
   }
    
   strstream tempoutput;
   tempoutput << "d" << diaton << "c" << chrom << ends;
   strcpy(output, tempoutput.str());
    
   return output; 
}


//////////////////////////////
//
// Convert::kernToBase40 -- extracts the pitch from a kern data field.
//      returns -1 if a null token.
//

int Convert::kernToBase40(const char* kernfield) {

   if (kernfield[0] == '.') {
      return -1;
   }

   int note = E_unknown;
   int i = 0;
   while (kernfield[i] != '\0') {
      if (isalpha(kernfield[i])) {
         switch (toupper(kernfield[i])) {
            case 'A': case 'B': case 'C': case 'D':
            case 'E': case 'F': case 'G':
               note = kernNoteToBase40(&kernfield[i]);
               goto finishup;
               break;
            case 'R':
               return E_base40_rest;
               break;
         }
      }
      i++;
   }

finishup:

   return note;
}



//////////////////////////////
//
// Convert::kernToBase40Class -- extracts the pitch from a kern data field.
//      Pitch class of the note.  Returns -1 if a null token
//

int Convert::kernToBase40Class(const char* kernfield) {

   int absolute = kernToBase40(kernfield);
   if (absolute >= 0) {
      return absolute % 40;
   } else {
      return absolute;
   }
}



//////////////////////////////
//
// kernNoteToBase40 -- returns the absolute base 40 pitch number of
//      a kern pitch.  Returns -1 on error or null token.
//

int Convert::kernNoteToBase40(const char* name) {

   int output;

   if (name[0] == '.') {
      return -1;
   }

   switch (name[0]) {
      case 'a': output = 4 * 40 + E_root_a;     break;
      case 'b': output = 4 * 40 + E_root_b;     break;
      case 'c': output = 4 * 40 + E_root_c;     break;
      case 'd': output = 4 * 40 + E_root_d;     break;
      case 'e': output = 4 * 40 + E_root_e;     break;
      case 'f': output = 4 * 40 + E_root_f;     break;
      case 'g': output = 4 * 40 + E_root_g;     break;
      case 'A': output = 3 * 40 + E_root_a;     break;
      case 'B': output = 3 * 40 + E_root_b;     break;
      case 'C': output = 3 * 40 + E_root_c;     break;
      case 'D': output = 3 * 40 + E_root_d;     break;
      case 'E': output = 3 * 40 + E_root_e;     break;
      case 'F': output = 3 * 40 + E_root_f;     break;
      case 'G': output = 3 * 40 + E_root_g;     break;
      case 'R': return E_root_rest;             break;
      default:  return -1;
   }

   int octave=0;
   while (name[octave] != '\0' && name[octave] == name[0]) {
      octave++;
   }
   if (islower(name[0])) {
      output += (octave - 1) * 40;
   } else {
      output -= (octave - 1) * 40;
   }

   // check for first accidental sign
   int accidental = octave;
   if (name[accidental] != '\0' && name[accidental] == '-') {
      output--;
   } else if (name[accidental] != '\0' && name[accidental] == '#') {
      output++;
   }

   // chek for second accidental sign
   if (name[accidental] == '\0') {
      ; // nothing
   } else { // check for double sharp or double flat
      accidental++;
      if (name[accidental] != '\0' && name[accidental] == '-') {
         output--;
      } else if (name[accidental] != '\0' && name[accidental] == '#') {
         output++;
      }
   }

   if (output >= 0) {
      return output;
   } else {
      cerr << "Error: pitch \"" << name << "\" is too low." << endl;
      exit(1);
   }
}


//////////////////////////////
//
// Convert::keyToScaleDegrees
//

Collection<int> Convert::keyToScaleDegrees(int aKey, int aMode) {
   Collection<int> output;
   
   switch (aMode) {
      case E_mode_major:
      case E_mode_ionian:
         output.setSize(7);
         output[0] = aKey;
         output[1] = aKey + E_base40_maj2;
         output[2] = aKey + E_base40_maj3;
         output[3] = aKey + E_base40_per4;
         output[4] = aKey + E_base40_per5;
         output[5] = aKey + E_base40_maj6;
         output[6] = aKey + E_base40_maj7;
         break;
      case E_mode_minor:
      case E_mode_aeolian:
         output[0] = aKey;
         output[1] = aKey + E_base40_maj2;
         output[2] = aKey + E_base40_min3;
         output[3] = aKey + E_base40_per4;
         output[4] = aKey + E_base40_per5;
         output[5] = aKey + E_base40_min6;
         output[6] = aKey + E_base40_min7;
         break;
      case E_mode_harm_minor:
         output[0] = aKey;
         output[1] = aKey + E_base40_maj2;
         output[2] = aKey + E_base40_min3;
         output[3] = aKey + E_base40_per4;
         output[4] = aKey + E_base40_per5;
         output[5] = aKey + E_base40_min6;
         output[6] = aKey + E_base40_maj7;
         break;
      default:
         cerr << "unknown mode: " << aMode << endl;
   }

   // keep the pitches in the primary octave
   for (int i=0; i<output.getSize(); i++) {
      output[i] = output[i] % 40;
   }

   return output;
}



//////////////////////////////
//
// Convert::museToBase40
//

int Convert::museToBase40(const char* pitchString) {
   char *temp;
   int length = strlen(pitchString);
   temp = new char[length+1];
   strcpy(temp, pitchString);
   int octave;
   int i = length;
   while (i >= 0 && !isdigit(temp[i])) {
      i--;
   }
  
   if (i <= 0) {
      cerr << "Error: could not find octave in string: " << pitchString << endl;
      exit(1);
   }

   octave = temp[i] - '0';
   temp[i] = '\0';

   return musePitchClass.getValue(temp) + 40 * (octave);
}



///////////////////////////////////////////////////////////////////////////
//
// Protected functions
//


//////////////////////////////
//
// calculateInversion --
//

int Convert::calculateInversion(int aType, int bassNote, int root) {

   if (aType == E_chord_note) {
      return -1;
   }

   ChordQuality cq(aType, 0, root);
   Collection<int> notes;
   notes = chordQualityToNoteSet(cq);
   int i;
   for (i=0; i<notes.getSize(); i++) {
      notes[i] %= 40;
   }
   bassNote = bassNote % 40;
   for (i=0; i<notes.getSize(); i++) {
      if (notes[i] % 40 == bassNote) {
         return i;
      }
   }
   
   cerr << "Error: Note: " 
        << Convert::kernPitchClass.getName(bassNote%40) << " is not in chord " 
        << Convert::chordType.getName(aType) << " with root "
        << Convert::kernPitchClass.getName(root) << endl;

   exit(1);
}


//////////////////////////////
//
// checkChord --
//

int Convert::checkChord(const Collection<int>& aSet) {

   int output;
   Collection<int> inval(aSet.getSize()-1);
   for (int i=0; i<inval.getSize(); i++) {
      inval[i] = aSet[i+1] - aSet[0];
   }

   switch (inval.getSize()) {
      case 0:
         output = E_chord_note;
         break;
      case 1:


         if (inval[0] == 11) {
            output = E_chord_incmin;
         } else if (inval[0] == 12) {
            output = E_chord_incmaj;
         } else {
            output = E_unknown;
         }
         break;
      case 2:
         if (inval[0] == 11 && inval[1] == 22) {
            output = E_chord_dim;
         } else if (inval[0] == 11 && inval[1] == 23) {
            output = E_chord_min;
         } else if (inval[0] == 12 && inval[1] == 23) {
            output = E_chord_maj;
         } else if (inval[0] == 12 && inval[1] == 24) {
            output = E_chord_aug;
         } else {
            output = E_unknown;
         }
         break;
      case 3:
         if (inval[0] == 11 && inval[1] == 22 && inval[2] == 33) {
            output = E_chord_fullydim;
         } else if (inval[0] == 11 && inval[1] == 22 && inval[2] == 34) {
            output = E_chord_halfdim;
         } else if (inval[0] == 11 && inval[1] == 23 && inval[2] == 34) {
            output = E_chord_minmin;
         } else if (inval[0] == 11 && inval[1] == 23 && inval[2] == 35) {
            output = E_chord_minmaj;
         } else if (inval[0] == 12 && inval[1] == 23 && inval[2] == 34) {
            output = E_chord_domsev;
         } else if (inval[0] == 12 && inval[1] == 23 && inval[2] == 35) {
            output = E_chord_majmaj;
         } else {
            output = E_unknown;
         }
         break;
      default:
         output = E_unknown;
   }

   return output;
}
   


//////////////////////////////
//
// intcompare -- compare two integers for ordering
//

int Convert::intcompare(const void* a, const void* b) {

   if (*((int*)a) < *((int*)b)) {
      return -1;
   } else if (*((int*)a) > *((int*)b)) {
      return 1;
   } else {
      return 0;
   }
}



//////////////////////////////
//
// Convert::rotatechord --
//

void Convert::rotatechord(Collection<int>& aChord) {

   int temp = aChord[0];
   for (int i=1; i<aChord.getSize(); i++) {
      aChord[i-1] = aChord[i];
   }
   aChord[aChord.getSize()-1] = temp + 40;
}





// md5sum:	3ff953277ae6774cb2a6419f7cfa1266  Convert.cpp
