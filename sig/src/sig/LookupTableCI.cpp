//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Nov  8 17:33:48 GMT-0800 1997
// Last Modified: Sat Nov  8 19:33:17 GMT-0800 1997
// Filename:      .../sig/code/base/LookupTable/LookupTableCI/LookupTableCI.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../../include && rm -f %b.o
//

#include "LookupTableCI.h"
#include <iostream.h>
#include <stdlib.h>


//////////////////////////////
//
// LookupTableCI::LookupTableCI
//	default value: size = 0
//

LookupTableCI::LookupTableCI(int size) {
   if (size < 0) {
      cerr << "Error in creating LookupTableCI: "
           << "table size cannot be negative: " << size << endl;
      exit(1);
   }

   maxSize = size;
   if (size > 0) {
      tableData = new sampleType[maxSize];
   } else {
      tableData = NULL;
   }
   borrowTableQ = 0;   // Not borrowing a table;
}


LookupTableCI::LookupTableCI(sampleType* borrowedTable, int size) {
   if (size < 0) {
      cerr << "Error: table size cannot be negative: " << size << endl;
      exit(1);
   }

   maxSize = size;
   tableData = borrowedTable;
   borrowTableQ = 1;   // borrowing a table;
}


LookupTableCI::LookupTableCI(LookupTableCI& aTable) {
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
// LookupTableCI::~LookupTableCI
//

LookupTableCI::~LookupTableCI() {
   if (tableData != NULL && !borrowQ()) {
      delete [] tableData;
   }
}



//////////////////////////////
//
// LookupTableCI::base
//

sampleType* LookupTableCI::base(void) const {
   return tableData;
}



//////////////////////////////
//
// LookupTableCI::borrow
//

void LookupTableCI::borrow(sampleType* aTable, int size) {
   if (size < 0) {
      cerr << "Error: cannot set size of table to a negative value: "
           << size << endl;
      exit(1);
   }

   if (aTable == tableData) {
      cerr << "Error: cannot borrow table: already owner." << endl;
      exit(1);
   }

   // won't be needing internal table:
   if (tableData != NULL) {
      delete [] tableData;
   }
   maxSize = size;
   tableData = aTable;
}


void LookupTableCI::borrow(LookupTableCI& aTable) {
   if (!borrowQ() && tableData != NULL) {
      delete [] tableData;
   }
   borrowTableQ = 1;
   maxSize = aTable.maxSize;
   tableData = aTable.tableData;
}



//////////////////////////////
//
// LookupTableCI::borrowQ
//

int LookupTableCI::borrowQ(void) const {
   return (int)borrowTableQ;
}



//////////////////////////////
//
// LookupTableCI::getSize
//

long LookupTableCI::getSize(void) const {
   return maxSize;
}



//////////////////////////////
//
// LookupTableCI::operator=
//

LookupTableCI& LookupTableCI::operator=(LookupTableCI& aTable) {
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



//////////////////////////////
//
// LookupTableCI::operator+=
//

LookupTableCI& LookupTableCI::operator+=(LookupTableCI& aTable) {
   if (getSize() != aTable.getSize()) {
      cerr << "Error: cannot add lookup tables with different sizes." << endl;
      exit(1);
   }

   for (int i=0; i<getSize(); i++) {
      tableData[i] += aTable.tableData[i];
   }

   return *this;
}



//////////////////////////////
//
// LookupTableCI::operator-=
//

LookupTableCI& LookupTableCI::operator-=(LookupTableCI& aTable) {
   if (getSize() != aTable.getSize()) {
      cerr << "Error: cannot add lookup tables with different sizes." << endl;
      exit(1);
   }

   for (int i=0; i<getSize(); i++) {
      tableData[i] -= aTable.tableData[i];
   }

   return *this;
}



//////////////////////////////
//
// LookupTableCI::operator*=
//

LookupTableCI& LookupTableCI::operator*=(LookupTableCI& aTable) {
   if (getSize() != aTable.getSize()) {
      cerr << "Error: cannot add lookup tables with different sizes." << endl;
      exit(1);
   }

   for (int i=0; i<getSize(); i++) {
      tableData[i] *= aTable.tableData[i];
   }

   return *this;
}


LookupTableCI& LookupTableCI::operator*=(double amplitude) {
   for (int i=0; i<getSize(); i++) {
      tableData[i] *= amplitude;
   }

   return *this;
}


LookupTableCI& LookupTableCI::operator*=(int amplitude) {
   for (int i=0; i<getSize(); i++) {
      tableData[i] *= amplitude;
   }

   return *this;
}



//////////////////////////////
//
// LookupTableCI::operator[]
//

sampleType& LookupTableCI::operator[](long readIndex) {
   return read(readIndex);
}


sampleType LookupTableCI::operator[](double readIndex) {
   return read(readIndex);
}



//////////////////////////////
//
// LookupTableCI::setSize -- if trying to change size on a borrowed table,
//	then stop borrowing table and make own table with specified size.
//

void LookupTableCI::setSize(long size) {
   if (size < 0) {
      cerr << "Error: cannot set size of table to a negative value: "
           << size << endl;
      exit(1);
   }
  
   if (borrowQ()) {
      borrowTableQ = 0;
   } 
   if (tableData != NULL) {
      delete [] tableData;
   }
   maxSize = size;
   if (maxSize == 0) {
      tableData = NULL;
   } else {
      tableData = new sampleType[maxSize];
   }
}



//////////////////////////////
//
// LookupTableCI::zero
//	default value: aValue = 0;
//

void LookupTableCI::zero(sampleType aValue) {
   // be careful when zeroing a borrowed table!
   for (int i=0; i<maxSize; i++) {
      tableData[i] = aValue;
   }
}   



/////////////////////////////////////////////////////////////////////////////
//
// private functions:
//


//////////////////////////////
//
// LookupTableCI::read
//

sampleType& LookupTableCI::read(long index) {
   if (index < 0 || index >= maxSize) {
      cerr << "Error invalid array acces in LookupTable: " << index << endl;
   }
   return tableData[index];
}

sampleType LookupTableCI::read(double index) const {
   // constant interpolation, ignore fractional value of index
   return tableData[(long)index];
}




// md5sum:	6ab7d95d3116331f32af437b0c5b9004  LookupTableCI.cpp
