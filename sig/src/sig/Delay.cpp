//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 27 00:51:22 GMT-0800 1997
// Last Modified: Sun Apr 27 11:06:50 GMT-0800 1997
// Filename:      ...sig/code/Filter/DelayLine/Delay.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "Delay.h"
#include <iostream.h>


//////////////////////////////
//
// Delay::Delay
//

Delay::Delay(int size) {
   allocSize = nextPowerOfTwo(size+1);
   indexMask = allocSize - 1;
   writeIndex = 0;
   array = new sampleType[allocSize];
   zero();
}



//////////////////////////////
//
// Delay::~Delay
//

Delay::~Delay() {
   delete [] array;
}



//////////////////////////////
//
// Delay::getSize
//

ulong Delay::getSize(void) {
   return  allocSize;
}



//////////////////////////////
//
// Delay::operator[]
//

sampleType& Delay::operator[](long readIndex) {
   return read(readIndex);
}

sampleType& Delay::operator[](int readIndex) {
   return read((long)readIndex);
}

sampleType Delay::operator[](float readIndex) {
   return read(readIndex);
}

sampleType Delay::operator[](double readIndex) {
   return read((float)readIndex);
}



//////////////////////////////
//
// Delay::setSize
//

void Delay::setSize(long size) {
   if (size < 0) size = -size;
   else if (size == 0) return;
   
   allocSize = nextPowerOfTwo(size);
   indexMask = allocSize - 1;
   writeIndex = 0;
   delete [] array;
   array = new sampleType[allocSize];
   zero();
}



//////////////////////////////
//
// Delay::write
//

void Delay::write(sampleType sample) {
   writeIndex = (writeIndex + 1) & indexMask;
   array[writeIndex] = sample;
}



//////////////////////////////
//
// Delay::zero
//

void Delay::zero(void) {
   for (ulong i=0; i<allocSize; i++) {
      array[i] = 0;
   }
}   



/////////////////////////////////////////////////////////////////////////////
//
// private functions:
//

//////////////////////////////
//
// nextPowerOfTwo
//

ulong Delay::nextPowerOfTwo(ulong num) {
   int leadingZeros = 0;
   int leadingZeroSearch = 1;
   int numberOfOnes = 0;
   int digit;
   
   for (int i=8*sizeof(ulong)-1; i>=0; i--) {
      digit = (num & (1 << i)) > 0 ? 1 : 0;
      if (digit == 1) {
         numberOfOnes++;
         leadingZeroSearch = 0;
      } else if (leadingZeroSearch && digit == 0) {
         leadingZeros++;
      }
   }

   if (numberOfOnes == 1) {
      return num;
   } else if (leadingZeros > 0) {
      return (1 << (8*sizeof(ulong)-leadingZeros+1));
   } else {
      return 2;
   }
}


//////////////////////////////
//
// Delay::read
//

sampleType& Delay::read(long index) {
   return array[(writeIndex+index+allocSize) & indexMask];
}

sampleType Delay::read(float index) {
   float fraction = index - (int)index;   

   sampleType element1 = read((long)index);
   sampleType element2 = read((long)(index - 1));

   return (element1-element2) * fraction + element1;
}

// md5sum:	666e82e27936aa65e89d51b6dd8e2c06  Delay.cpp
