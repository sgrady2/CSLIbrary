//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Nov  8 17:33:48 GMT-0800 1997
// Last Modified: Sat Nov  8 19:33:17 GMT-0800 1997
// Filename:      .../sig/code/LookupTable/LookupTableLI/LookupTableLI.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../../include %
//                -I../LookupTableCI && rm -f %b.o
//

#include "LookupTableLI.h"
#include <iostream.h>


//////////////////////////////
//
// LookupTableLI::LookupTableLI
//	default value: size = 0
//

LookupTableLI::LookupTableLI(int size) : LookupTableCI(size) {
   // do nothing
}


LookupTableLI::LookupTableLI(sampleType* borrowedTable, int size) :
      LookupTableCI(borrowedTable, size) {
   // do nothing
}


LookupTableLI::LookupTableLI(LookupTableLI& aTable) : LookupTableCI(aTable) {
   // do nothing
}



//////////////////////////////
//
// LookupTableLI::~LookupTableLI
//

LookupTableLI::~LookupTableLI() {
   if (tableData != NULL && !borrowQ()) {
      delete [] tableData;
   }
}



/////////////////////////////////////////////////////////////////////////////
//
// private functions:
//


//////////////////////////////
//
// LookupTableLI::read --  Linear interpolation
//

sampleType LookupTableLI::read(double index) const {
   static sampleType a, b;

   a = tableData[(long)index];
   if (index >= maxSize-1) {
      b = tableData[0];
   } else {
      b = tableData[(long)index+1];
   }
   return (b - a) * (index-(long)index) + a;
}




// md5sum:	00738b9f3e83701e1e642e488556a84a  LookupTableLI.cpp
