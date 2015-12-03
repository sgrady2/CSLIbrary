//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat May 23 20:05:41 PDT 1998
// Last Modified: Wed Jun 10 22:42:18 PDT 1998
// Filename:      Enum_exclusiveInterpretation.h
// Syntax:        C++ 
//

#ifndef _ENUM_EXCLUSIVEINTERPRETATION_H_INCLUDED
#define _ENUM_EXCLUSIVEINTERPRETATION_H_INCLUDED

#include "Enum_basic.h"

#define E_UNKNOWN_EXINT       (0000)      

#define E_UNKNOWN_EXINT_NAME     "**unknown"

// standard humdrum exclusive interpretations
#define E_KERN_EXINT          (0001)      /*     **kern       */

#define E_KERN_EXINT_NAME     "**kern"


// humdrum++ exclusive interpretations.
#define E_QUAL_EXINT          (1001)      /*    **qual        */

#define E_QUAL_EXINT_NAME     "**qual"


#endif  // _ENUM_EXCLUSIVEINTERPRETATION_H_INCLUDED



// md5sum:	f7d74b76eaa81ad9ccaffc21d475ed7b  Enum_exclusiveInterpretation.h
