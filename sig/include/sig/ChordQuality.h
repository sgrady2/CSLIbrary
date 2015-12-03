//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Jun  7 16:24:06 PDT 1998
// Last Modified: Sun Jun  7 16:24:06 PDT 1998
// Filename:      ...humdrum++/src/ChordQuality.cpp
// Syntax:        C++ 
// $Smake:        cc -O3 -o %b %f -lg++ && strip %b
//

#ifndef _CHORDQUALITY_H_INCLUDED
#define _CHORDQUALITY_H_INCLUDED

#include "Collection.h"
#include <iostream.h>


class ChordQuality {
   public:
                         ChordQuality     (void);
                         ChordQuality     (const ChordQuality& aChordQuality);
                         ChordQuality     (int aType, int anInversion, 
                                             int aRoot);
                        ~ChordQuality     ();

      int                getBassNote      (void) const;
      static const char* getDisplay       (void);
      int                getInversion     (void) const;
      const char*        getInversionName (void) const;
      Collection<int>    getNotesInChord  (void) const;
      int                getRoot          (void) const;
      const char*        getRootName      (void) const;
      int                getType          (void) const;
      const char*        getTypeName      (void) const;
      void               makeString       (char* space);
      void               print            (const char* aDisplayString, 
                                             ostream& out = cout) const;
      void               print            (ostream& out = cout) const;
      static void        setDisplay       (const char* aDisplayFormat);
      void               setInversion     (int anInversion);
      void               setInversion     (const char* anInversionName);
      void               setQuality       (const char* aQuality);
      void               setRoot          (int aRoot);
      void               setRoot          (const char* aRootName);
      void               setType          (int aType);
      void               setType          (const char* aTypeName);

   private:
      int                chordType;       // chord type enumeration
      int                chordInversion;  // inversion of the chord
      int                chordRoot;       // root pitch class of the chord
      static char*       displayString;   // format to display quality data
};

ostream& operator<<(ostream& out, const ChordQuality& aQuality);


#endif  // _CHORDQUALITY_H_INCLUDED



// md5sum:	3e322b0e22fad1dfe25497917adeaa5b  ChordQuality.h
