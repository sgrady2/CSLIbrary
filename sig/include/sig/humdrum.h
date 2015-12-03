//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat May 23 21:13:15 PDT 1998
// Last Modified: Sat Jun 13 23:10:36 PDT 1998
// Last Modified: Mon May 31 16:18:36 PDT 1999
// Filename:      ...humdrum++/include/humdrum.h
// Syntax:        C++ 
//
// Description:   Includes all header files necessary for humdrum++
//		  library.
//

#ifndef _HUMDRUM_H_INCLUDED
#define _HUMDRUM_H_INCLUDED


// headers:
   #include "HumdrumEnumerations.h"
   #include "humdrumVersion.h"



// native humdrum++ classes:
   #include "HumdrumRecord.h"
   #include "HumdrumFile.h"
   #include "EnumerationData.h"
   #include "Enumeration.h"
   #include "Convert.h"
   #include "ChordQuality.h"
   #include "Identify.h"


// support classes borrowed from sig++
   #include "Collection.h"
   #include "Array.h"
   #include "Options.h"





// have to include for some template bug:
// Works in this release:
// gcc version egcs-2.91.66 19990314/Linux (egcs-1.1.2 release)
//   #include "Options.cpph"


#endif  // _HUMDRUM_H_INCLUDED



// md5sum:	1ac9b2dfd0a285bc0033073ca95c09f4  humdrum.h
