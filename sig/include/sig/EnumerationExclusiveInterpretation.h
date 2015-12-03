//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Jun 10 22:56:56 PDT 1998
// Last Modified: Wed Jun 10 22:57:02 PDT 1998
// Filename:      ...humdrum++/include/EnumerationExclusiveInterpretation.h
// Syntax:        C++ 
//

#ifndef _ENUMERATION_EXCLUSIVE_INTERPRETATION_H_INCLUDED
#define _ENUMERATION_EXCLUSIVE_INTERPRETATION_H_INCLUDED


#include "Enumeration.h"
#include "Enum_exclusiveInterpretation.h"


class EnumerationExclusiveInterpretation : public Enumeration {
   public:
      EnumerationExclusiveInterpretation(void) : Enumeration() {
         add(E_UNKNOWN_EXINT ,   E_UNKNOWN_EXINT_NAME ,   ENUM_FIXED_ALLOC);

      // standard types of exclusive interpretations
         add(E_KERN_EXINT    ,   E_KERN_EXINT_NAME    ,   ENUM_FIXED_ALLOC);


      // humdrum++ defined exclusive interpretations
         add(E_QUAL_EXINT    ,   E_QUAL_EXINT_NAME     ,   ENUM_FIXED_ALLOC);
        
      }

};


#endif  // _ENUMERATION_EXCLUSIVE_INTERPRETATION_H_INCLUDED



// md5sum:	e8f23f62b89acb6d75e281e033b06b5b  EnumerationExclusiveInterpretation.h
