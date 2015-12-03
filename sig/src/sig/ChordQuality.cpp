//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Jun  7 16:24:06 PDT 1998
// Last Modified: Sun Jun 11 12:39:40 PDT 2000
// Filename:      ...sig/src/sig/ChordQuality.cpp
// Syntax:        C++ 
//

#include "ChordQuality.h"
#include "Convert.h"
#include "HumdrumEnumerations.h"

#ifdef VISUAL
   #include <strstrea.h>
#else
   #include <strstream.h>
#endif

#include <string.h>


char* ChordQuality::displayString = NULL;



//////////////////////////////
//
// ChordQuality::ChordQuality --
//

ChordQuality::ChordQuality(void) {
   chordType = E_unknown;
   chordInversion = E_unknown;
   chordRoot = E_unknown;
   if (getDisplay() == NULL) {
      setDisplay("t:i:r");
   }
}


ChordQuality::ChordQuality(const ChordQuality& aChordQuality) {
   chordType = aChordQuality.getType();;
   chordInversion = aChordQuality.getInversion();;
   chordRoot = aChordQuality.getRoot();
}


ChordQuality::ChordQuality(int aType, int anInversion, int aRoot) {
   chordType = aType;
   chordInversion = anInversion;
   chordRoot = aRoot;
}



//////////////////////////////
//
// ChordQuality::~ChordQuality --
//

ChordQuality::~ChordQuality() {
   chordType = E_unknown;
   chordInversion = E_unknown;
   chordRoot = E_unknown;
}



//////////////////////////////
//
// ChordQuality::getBassNote --
//

int ChordQuality::getBassNote(void) const {
   return Convert::chordQualityToBaseNote(*this);
}



//////////////////////////////
//
// ChordQuality::getDisplay --
//

const char* ChordQuality::getDisplay(void) {
   return displayString;
}



//////////////////////////////
//
// ChordQuality::getInversion --
//

int ChordQuality::getInversion(void) const {
   return chordInversion;
}



//////////////////////////////
//
// ChordQuality::getInversionName --
//

const char* ChordQuality::getInversionName(void) const {
   return Convert::chordInversion.getName(getInversion());
}



//////////////////////////////
//
// ChordQuality::getNotesInChord --
//

Collection<int> ChordQuality::getNotesInChord(void) const {
   Collection<int>* output;
   output = new Collection<int>;
   *output = Convert::chordQualityToNoteSet(*this);
   return *output;
}



//////////////////////////////
//
// ChordQuality::getRoot --
//

int ChordQuality::getRoot(void) const {
   return chordRoot;
}



//////////////////////////////
//
// ChordQuality::getRootName --
//

const char* ChordQuality::getRootName(void) const {
   return Convert::kernPitchClass.getName(getRoot());
}



//////////////////////////////
//
// ChordQuality::getType --
//

int ChordQuality::getType(void) const {
   return chordType;
}



//////////////////////////////
//
// ChordQuality::getTypeName --
//

const char* ChordQuality::getTypeName(void) const {
   return Convert::chordType.getName(getType());
}



//////////////////////////////
//
// ChordQuality::makeString --
//

void ChordQuality::makeString(char* space) {
   strstream temp;
   temp << *this << ends;
   strcpy(space, temp.str());
}



//////////////////////////////
//
// ChordQuality::print --
//

void ChordQuality::print(const char* aDisplayString, ostream& out) const {
   int i = 0;
   while (aDisplayString[i] != '\0') {
      switch (aDisplayString[i]) {
         case 't':
            out << getTypeName();
            break;
         case 'i':
            if (getType() == E_chord_note) {
               ;  // do nothing
            } else {
               out << getInversionName();
            }
            break;
         case 'r':
            out << getRootName();
            break;
         default:
            out << aDisplayString[i];
      }
      i++;
   }
}


void ChordQuality::print(ostream& out) const {
   print(getDisplay(), out);
}



//////////////////////////////
//
// ChordQuality::setDisplay --
//

void ChordQuality::setDisplay(const char* aDisplayFormat) {

   if (displayString != NULL) {
      delete [] displayString;
   }
   displayString = new char[strlen(aDisplayFormat) + 1];
   strcpy(displayString, aDisplayFormat);
}



//////////////////////////////
//
// ChordQuality::setInversion --
//

void ChordQuality::setInversion(int anInversion) {
   chordInversion = anInversion;
}


void ChordQuality::setInversion(const char* anInversionName) {
   chordInversion = Convert::chordInversion.getValue(anInversionName);
}



//////////////////////////////
//
// ChordQuality::setRoot --
//

void ChordQuality::setRoot(int aRoot) {
   chordRoot = aRoot;
}


void ChordQuality::setRoot(const char* aRoot) {
   chordRoot = Convert::kernPitchClass.getValue(aRoot);
}



//////////////////////////////
//
// ChordQuality::setQuality --
//

void ChordQuality::setQuality(const char* aQuality) {
   chordType      = Convert::chordQualityToType(aQuality);
   chordInversion = Convert::chordQualityToInversion(aQuality);
   chordRoot      = Convert::chordQualityToRoot(aQuality);
}



//////////////////////////////
//
// ChordQuality::setType --
//

void ChordQuality::setType(int aType) {
   chordType = aType;
}


void ChordQuality::setType(const char* aTypeName) {
   chordType = Convert::chordType.getValue(aTypeName);
}



///////////////////////////////////////////////////////////////////////////
//
// external functions
//


//////////////////////////////
//
// operator<<  --
//

ostream& operator<<(ostream& out, const ChordQuality& aChordQuality) {

   aChordQuality.print(out);
   return out;
}




// md5sum:	c7a7ac25c2c3d912953c2cd28f9fe2bd  ChordQuality.cpp
