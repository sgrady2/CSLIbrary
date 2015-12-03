//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Feb 26 04:34:19 PST 1999
// Last Modified: Wed Mar  3 10:12:52 PST 1999
// Last Modified: Thu Jul  1 16:24:15 PDT 1999
// Filename:      ...sig/maint/code/net/NetBase/NetBase.cpp
// Syntax:        C++ 
// $Smake:        g++ -O3 -g -c %f && rm %b.o
//

#include "NetBase.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef VISUAL
   // snprintf is a GNU C function analogous to strncpy()
   void snprintf(char* string, int number,  char* format, int num) {
      sprintf(string, format, num);
   }
#endif

// define static NetBase variables:
int NetBase::namer = 0;


//////////////////////////////
//
// NetBase::NetBase --
//     default value: aName = NULL
//

NetBase::NetBase(const char* aName) 
   : Matrix<double>() { 
   name = NULL;
   if (aName != NULL) {
      setName(aName);
   } else {
      name = new char[16];
      snprintf(name, 16, "unknown%d", namer);
   }
   namer++;
}


NetBase::NetBase(int aSize, const char* aName)
   : Matrix<double>(aSize) { 
   if (aName != NULL) {
      setName(aName);
   } else {
      name = new char[16];
      snprintf(name, 16, "unknown%d", namer);
   }
   namer++;
}


NetBase::NetBase(int rows, int columns, const char* aName) 
   : Matrix<double>(rows, columns) { 
   if (aName != NULL) {
      setName(aName);
   } else {
      name = new char[16];
      snprintf(name, 16, "unknown%d", namer);
   }
   namer++;
}



//////////////////////////////
//
// NetBase::NetBase --
//

NetBase::~NetBase() { 
   if (name != NULL) {
      delete [] name;
      name = NULL;
   }
}



//////////////////////////////
//
// NetBase::getName -- returns the name of the Net object
//

const char* NetBase::getName(void) const { 
   return name;
}



//////////////////////////////
//
// NetBase::setName -- sets the name of the Net object
//

void NetBase::setName(const char* aString) { 
   if (name != NULL) {
      delete [] name;
   }
   int length = strlen(aString);
   name = new char[length + 1];
   strcpy(name, aString);
}



//////////////////////////////
//
// NetBase::setSize -- for one dimensional matrix
//

void NetBase::setSize(int aSize) {
   Matrix<double>::setSize(1, aSize);
}



//////////////////////////////
//
// NetBase::seed -- sets the contents of the matrix elements.
//      Uniform distribution in the range.
//      default values: lowerlimit = -1, upperlimit = 9.
//                      seedValue = 0;
//

void NetBase::seed(double lowerlimit, double upperlimit, int seedValue) { 
   if (lowerlimit >= upperlimit) {
      cout << "Unreasonable seed ranges: " << lowerlimit << " to " 
           << upperlimit << endl;
      cout << "Netbase name = " << getName() << endl;
      exit(1);
   }
   if (seedValue < 0) {
      srand(time(NULL));
   } else if (seedValue > 0) {
      srand(seedValue);
   }
   double range = upperlimit - lowerlimit;
   for (int i=0; i<getSize(); i++) {
      storage[i] = (double)rand()/RAND_MAX * range + lowerlimit;
   }
}


// md5sum:	25e6e90e3c33e94423da64ab05edca89  NetBase.cpp
