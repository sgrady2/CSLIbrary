//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Jul  9 14:25:29 PDT 1998
// Last Modified: Thu Jul  9 14:25:29 PDT 1998
// Filename:      ...humdrum++/src/Identify.cpp
// Syntax:        C++ 
// $Smake:        g++ -O -o %b %f && strip %b
//


#include "Identify.h"
#include <string.h>
#include <ctype.h>


//////////////////////////////
//
// Identify::humdrumNullField --
//

int Identify::humdrumNullField(const char* aField) {
   if (strcmp(aField, ".") == 0) {
      return 1;
   } else {
      return 0;
   }
}



//////////////////////////////
//
// Identify::kernTimeSignature --
//

int Identify::kernTimeSignature(const char* kernString) {
   // meter tandem interpretation must start with a '*'
   if (kernString[0] != '*') {
      return 0;
   }

   // next character after '*' must be a number:
   if (!isdigit(kernString[1])) {
      return 0;
   }

   // there must be a '/' in the string:
   char* slash = strchr(kernString, '/');
   if (slash == NULL) {
      return 0;
   }

   // The next character after a slash must be a number
   if (!isdigit(slash[1])) {
      return 0;
   }
  
   // got this far, so assume that input is a time signature   

   return 1;
}





// md5sum:	16a143efd2759d7823ea504bcd2cfbbe  Identify.cpp
