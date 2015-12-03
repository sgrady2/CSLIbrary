//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Nov  8 17:33:48 GMT-0800 1997
// Last Modified: Sat Nov  8 19:33:17 GMT-0800 1997
// Filename:      ...sig/code/base/LookupTable/LookupTable.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include %
//                -ILookupTableCI && rm -f %b.o
//

#include "LookupTable.h"
#include <iostream.h>
#include <stdlib.h>


//////////////////////////////
//
// LookupTable::LookupTable
//	default value: size = 0
//

LookupTable::LookupTable(int size) : LookupTableCI(size) {
   interpolationType = LOOKUP_LINEAR;
}


LookupTable::LookupTable(sampleType* borrowedTable, int size) :
      LookupTableCI(borrowedTable, size) {
   interpolationType = LOOKUP_LINEAR;
}


LookupTable::LookupTable(const LookupTable& aTable) {
   interpolationType = aTable.interpolationType;
   maxSize = aTable.maxSize;
   borrowTableQ = aTable.borrowTableQ;

   if (!borrowQ()) {         // internal table
      if (maxSize == 0) {
         tableData = NULL;
      } else {
         tableData = new sampleType[maxSize];
         for (int i=0; i<maxSize; i++) {
            tableData[i] = aTable.tableData[i];
         }
      }
   } else {                     // borrowing a table
      tableData = aTable.tableData;
   }
}



//////////////////////////////
//
// LookupTable::~LookupTable
//

LookupTable::~LookupTable() {
   // nothing to do
}



//////////////////////////////
//
// LookupTable::doConstantInterpolation
//

void LookupTable::doConstantInterpolation(void) {
   interpolationType = LOOKUP_CONSTANT;
}



//////////////////////////////
//
// LookupTable::doLinearInterpolation
//

void LookupTable::doLinearInterpolation(void) {
   interpolationType = LOOKUP_LINEAR;
}



//////////////////////////////
//
// LookupTable::operator=
//

LookupTable& LookupTable::operator=(LookupTable& aTable) {
   interpolationType = aTable.interpolationType;
   maxSize = aTable.maxSize;
   borrowTableQ = aTable.borrowTableQ;

   if (!borrowQ()) {         // internal table
      if (maxSize == 0) {
         tableData = NULL;
      } else {
         tableData = new sampleType[maxSize];
         for (int i=0; i<maxSize; i++) {
            tableData[i] = aTable.tableData[i];
         }
      }
   } else {                     // borrowing a table
      tableData = aTable.tableData;
   }
 
   return *this;
}



/////////////////////////////////////////////////////////////////////////////
//
// private functions:
//


//////////////////////////////
//
// LookupTable::read --  Either Linear or Constant interpolation
//

sampleType LookupTable::read(double index) const {
   static sampleType a, b;

   switch (interpolationType) {
      case LOOKUP_CONSTANT:
         return tableData[(long)index];
         break;
      case LOOKUP_LINEAR:
         a = tableData[(long)index];
         if (index >= maxSize-1) {
            b = tableData[0];
         } else {
            b = tableData[(long)index+1];
         }
         return (b - a) * (index-(long)index) + a;
         break;
      default:
         cerr << "Unknown interpolation type: " << interpolationType << endl;
         exit(1);
   }

   // include the following line for stuipd Visual C++ 5.0 compiler.
   // this line should never be reached.
   return 0.0;
}




// md5sum:	5a94b26190970809a2504b0e6d52b5be  LookupTable.cpp
