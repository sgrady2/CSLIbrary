//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Apr 29 22:22:22 GMT-0800 1997
// Last Modified: Tue Apr 29 22:42:56 GMT-0800 1997
// Filename:      ...sig/code/Filter/UserOut/UserOut.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "UserOut.h"
#include <stdlib.h>
#include <string.h>


//////////////////////////////
//
// UserOut::UserOut
//    default value: aTag = "UserOut"
//

UserOut::UserOut(char *aTag) {
   setName("UserOut");
   tag = NULL;
   setTag(aTag);
   brandname = FILTER;
}



//////////////////////////////
//
// UserOut::action
//

void UserOut::action(void) {
   for (int i=0; i<inputs.getSize(); i++) {
      cout << tag << ": i = " << i << "\t value = " << inputs[i] << endl;
   }
}



//////////////////////////////
//
// UserOut::output
//

sampleType UserOut::output(int channel) {
   return inputs[channel];
}



//////////////////////////////
//
// UserOut::setTag
//

void UserOut::setTag(char *aTag) {
   if (tag != NULL) delete [] tag;
   tag = new char[strlen(aTag)+1];
   strcpy(tag, aTag);
}



// md5sum:	7f7dc14174be57897dde51f4cddfeec9  UserOut.cpp
