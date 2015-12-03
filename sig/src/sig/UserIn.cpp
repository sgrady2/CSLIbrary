//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Apr 28 21:46:28 GMT-0800 1997
// Last Modified: Tue Apr 29 22:03:53 GMT-0800 1997
// Filename:      ...sig/code/Generator/UserIn/UserIn.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#include "UserIn.h"
#include <iostream.h>


//////////////////////////////
//
// UserIn::UserIn
//

UserIn::UserIn(void) {
   setName("UserIn");
   outputValue = 0;
   brandname = GENERATOR;
}



//////////////////////////////
//
// UserIn::~UserIn
//

UserIn::~UserIn() { }



//////////////////////////////
//
// UserIn::action
//

void UserIn::action(void) { 
   cout << "Enter a sample: ";
   cin >> outputValue;
}



//////////////////////////////
//
// UserIn::output
//

sampleType UserIn::output(int channel) { 
   return outputValue;
}



// md5sum:	5c3f3d044f05c81b5b96abd9bf675f97  UserIn.cpp
