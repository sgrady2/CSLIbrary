//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Nov  8 17:33:48 GMT-0800 1997
// Last Modified: Sat Nov  8 19:33:17 GMT-0800 1997
// Filename:      .../sig/code/LookupTable/PeriodTable/PeriodTable.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../../include %
//                -I.. -I../LookupTableCI && rm -f %b.o
//

#include "PeriodTable.h"
#include <iostream.h>
#include <iomanip.h>
#include <stdlib.h>
#include <math.h>


//////////////////////////////
//
// PeriodTable::PeriodTable
//	default value: size = 0
//

PeriodTable::PeriodTable(int size) : LookupTable(size) {
   zero();
}


PeriodTable::PeriodTable(sampleType* borrowedTable, int size) :
      LookupTable(borrowedTable, size) {
   zero();
}


PeriodTable::PeriodTable(PeriodTable& aTable) : LookupTable(aTable) {
   zero();
}


// default value: size = 1024
PeriodTable::PeriodTable(char* oscTable, int size) : LookupTable(size) {
   zero();
   add(oscTable);
}



//////////////////////////////
//
// PeriodTable::~PeriodTable
//

PeriodTable::~PeriodTable() {
   // nothing to do
}



//////////////////////////////
//
// PeriodTable::add
//	default value: phase = 0
//

void PeriodTable::add(int periods, double amp, double phase) {
   for (int i=0; i<getSize(); i++) {
      tableData[i] += amp * sin(TWOPI * periods / getSize() * i + phase);
   }
}
   

void PeriodTable::add(char* oscString) {
   cout << "Not implemented yet" << endl;
}
   


//////////////////////////////
//
// PeriodTable::flip -- similar to reverse, but the first
//	element in the array remains fixed.
//

void PeriodTable::flip(void) {
   sampleType temp;
   for (int i=1; i<getSize()/2+1; i++) {
      temp = tableData[i];
      tableData[i] = tableData[getSize()-i];
      tableData[getSize()-i] = temp;
   }
}



//////////////////////////////
//
// PeriodTable::leftShift
//

void PeriodTable::leftShift(int amount) {
   if (amount < 0) {
      rightShift(-amount);
      return;
   } else if (amount == 0) return;
   else if (amount >= getSize()) {
      cerr << "Error: cannot shift by more than size of table" << endl;
      exit(1);
   }

   int i, j;
   sampleType *temp;
   temp = new sampleType[amount];
   for (i=0; i<amount; i++) {
      temp[i] = tableData[i];
   }
   for (i=0; i<getSize()-amount; i++) {
      tableData[i] = tableData[amount+i];
   }
   for (i=getSize()-amount, j=0; i<getSize() && j<amount; i++, j++) {
      tableData[i] = temp[j];
   }
   delete [] temp;
}



//////////////////////////////
//
// PeriodTable::print -- print a list plot for Mathematica.
//

void PeriodTable::print(void) {
   cout << "ListPlot[{";
   for (int i=0; i<getSize()-1; i++) {
      if (tableData[i] < 0.00001 && tableData[i] > -0.00001) {
         cout << "0.00000, ";
      } else {
         cout << tableData[i] << ", ";
      }
      if ((i+1)%10 == 0) cout << "\n" ;
   }
   if (tableData[getSize()-1] < 0.00001 && tableData[getSize()-1] > -0.00001) {
      cout << "0.00000";
   } else {
      cout << tableData[getSize()-1];
   }
   cout << "}, Frame->True, PlotJoined->True, PlotRange->All];" << endl;
}



//////////////////////////////
//
// PeriodTable::ramp
//	default value: amplitude = 1
//

void PeriodTable::ramp(double amplitude) {
   double increment = 2.0 * amplitude / (getSize()-1);
   double value = -amplitude;
   for (int i=0; i<maxSize; i++) {
      tableData[i] = value;
      value += increment;
   }
}



//////////////////////////////
//
// PeriodTable::remove
//	default value: phase = 0
//

void PeriodTable::remove(int periods, double amp, double phase) {
   for (int i=0; i<getSize(); i++) {
      tableData[i] -= sin(TWOPI * periods * i + phase);
   }
}
   


//////////////////////////////
//
// PeriodTable::reverse
//

void PeriodTable::reverse(void) {
   sampleType temp;
   for (int i=0; i<getSize()/2; i++) {
      temp = tableData[i];
      tableData[i] = tableData[getSize()-1-i];
      tableData[getSize()-1-i] = temp;
   }
}



//////////////////////////////
//
// PeriodTable::rightShift
//

void PeriodTable::rightShift(int amount) {
   if (amount < 0) {
      leftShift(-amount);
      return;
   } else if (amount == 0) return;
   else if (amount >= getSize()) {
      cerr << "Error: cannot shift by more than size of table" << endl;
      exit(1);
   }

   int i;
   sampleType *temp;
   temp = new sampleType[amount];
   for (i=0; i<amount; i++) {
      temp[i] = tableData[getSize()-amount+i];
   }
   for (i=0; i<getSize()-amount; i++) {
      tableData[getSize()-i-1] = tableData[getSize()-amount-i-1];
   }
   for (i=0; i<amount; i++) {
      tableData[i] = temp[i];
   }
   delete [] temp;
}



//////////////////////////////
//
// PeriodTable::sine
//	default value: amplitude = 1
//

void PeriodTable::sine(double amplitude) {
   for (int i=0; i<maxSize; i++) {
      tableData[i] = amplitude * sin(TWOPI * (double)i / (double)maxSize);
   }
}



//////////////////////////////
//
// PeriodTable::square
//	default value: amplitude = 1
//

void PeriodTable::square(double amplitude) {
   int i;
   for (i=0; i<getSize()/2; i++) {
      tableData[i] = amplitude;
   }
   for (i=getSize()/2; i<getSize(); i++) {
      tableData[i] = -amplitude;
   }

   if (getSize() % 2 == 1) {
      tableData[getSize()/2] = 0;
   }
}



//////////////////////////////
//
// PeriodTable::subtract
//

void PeriodTable::subtract(char* oscString) {
   cerr << "Does not work yet" << endl;
}



//////////////////////////////
//
// PeriodTable::triangle
//	default value: amplitude = 1
//

void PeriodTable::triangle(double amplitude) {
   double increment;
   double value = -amplitude;
   int i;

   if (getSize() % 2 == 0) {
      increment = 2.0 * amplitude / (getSize()/2);
      for (i=0; i<getSize()/2; i++) {
         tableData[i] = value;
         value += increment;
      }
      for (i=getSize()/2; i<getSize(); i++) {
         tableData[i] = value;
         value -= increment;
      }
   } else {
      increment = 2.0 * amplitude / ((getSize()-1)/2);
      for (i=0; i<getSize()/2; i++) {
         tableData[i] = value;
         value += increment;
      }
      increment = 2.0 * amplitude / ((getSize()+1)/2);
      for (i=getSize()/2; i<getSize(); i++) {
         tableData[i] = value;
         value -= increment; 
      }
   }
}




// md5sum:	f0232b000777e86b65c2588dcbbc7be6  PeriodTable.cpp
