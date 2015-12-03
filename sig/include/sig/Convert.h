//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Jun  8 00:38:46 PDT 1998
// Last Modified: Tue Jun 23 14:00:23 PDT 1998
// Last Modified: Fri May  5 12:17:08 PDT 2000 (added kernToMidiNoteNumber())
// Filename:      ...sig/include/sig/Convert.h
// Syntax:        C++ 
//
// Description:   This class contains static function that can be used
//                to convert from one data representation to another.
//


#ifndef _CONVERT_H_INCLUDED
#define _CONVERT_H_INCLUDED

#include "HumdrumEnumerations.h"
#include "ChordQuality.h"
#include "Collection.h"


class Convert {
   public: 
 
   // enumeration databases

      static EnumerationExclusiveInterpretation exint;
      static EnumerationChordQualityType        chordType;
      static EnumerationChordQualityInversion   chordInversion;
      static EnumerationChordQualityRoot        kernPitchClass;
      static EnumerationMusePitchClass          musePitchClass;
      static EnumerationInterval                intervalNames;

   // conversions dealing with humdrum data


   // conversions dealing with **kern data

      static int       kernToMidiNoteNumber      (const char* aKernString);
      static char*     durationToKernRhythm      (char* output, double input, 
                                                   int timebase = 1);
      static double    kernToDuration            (const char* aKernString);
      static double    kernTimeSignatureTop      (const char* aKernString);
      static double    kernTimeSignatureBottomToDuration   
                                                 (const char* aKernString);
      static char*     musePitchToKernPitch      (char* kernOutput, const
                                                    char* museInput);
      static char*     museClefToKernClef        (char* kernOutput, 
                                                    int museInput);

   // conversions dealing with **qual data

      static ChordQuality chordQualityStringToValue (const char* aString);
      static int       chordQualityToBaseNote    (const ChordQuality& aQuality);
      static Collection<int> chordQualityToNoteSet (const ChordQuality& 
                                                     aQuality);
      static int       chordQualityToInversion   (const char* aQuality);
      static int       chordQualityToRoot        (const char* aQuality);
      static int       chordQualityToType        (const char* aQuality);
      static ChordQuality noteSetToChordQuality  (const Collection<int>& aSet);


   // conversions dealing with base 40 system of notation

      static char*     base40ToKern               (char* output, int aPitch);
      static char*     base40ToKernTranspose      (char* output, int transpose,
                                                     int keysignature);
      static int       kernToBase40               (const char* kernfield);
      static int       kernToBase40Class          (const char* kernfield);
      static int       kernNoteToBase40           (const char* name);
      static Collection<int> keyToScaleDegrees    (int aKey, int aMode);
      static int       museToBase40               (const char* pitchString);


   protected:

      static int     calculateInversion         (int aType, int bassNote, 
                                                    int root);
      static int     checkChord                 (const Collection<int>& aSet);
      static int     intcompare                 (const void* a, const void* b);
      static void    rotatechord                (Collection<int>& aChord);


};


#endif // _CONVERT_H_INCLUDED



// md5sum:	cf1b119f7457b15250258d52f4f0c752  Convert.h
