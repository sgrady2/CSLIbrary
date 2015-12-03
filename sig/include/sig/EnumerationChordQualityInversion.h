//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Jun 12 22:55:05 PDT 1998
// Last Modified: Fri Jun 12 22:55:09 PDT 1998
// Filename:      ...humdrum++/src/EnumerationChordQualityInversion.h
// Syntax:        C++ 
//

#ifndef _ENUMERATION_CHORDQUALITY_INVERSION_H_INCLUDED
#define _ENUMERATION_CHORDQUALITY_INVERSION_H_INCLUDED


#include "Enumeration.h"
#include "Enum_chordQuality.h"


class EnumerationChordQualityInversion : public Enumeration {
   public:
      EnumerationChordQualityInversion(void) : Enumeration() {
         setNullName("X", ENUM_FIXED_ALLOC);
         add(E_inv_root   ,   E_inv_name_root   ,   ENUM_FIXED_ALLOC);
         add(E_inv_first  ,   E_inv_name_first  ,   ENUM_FIXED_ALLOC);
         add(E_inv_second ,   E_inv_name_second ,   ENUM_FIXED_ALLOC);
         add(E_inv_third  ,   E_inv_name_third  ,   ENUM_FIXED_ALLOC);
         add(E_inv_fourth ,   E_inv_name_fourth ,   ENUM_FIXED_ALLOC);
         add(E_inv_fifth  ,   E_inv_name_fifth  ,   ENUM_FIXED_ALLOC);
         add(E_inv_sixth  ,   E_inv_name_sixth  ,   ENUM_FIXED_ALLOC);
      }

};


#endif  // _ENUMERATION_CHORDQUALITY_INVERSION_H_INCLUDED



// md5sum:	e413d6c8b6637db601d5b759c54bfbdf  EnumerationChordQualityInversion.h
