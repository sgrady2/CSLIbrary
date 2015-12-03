//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri May  1 20:54:33 PDT 1998
// Last Modified: Fri May  1 20:54:29 PDT 1998
// Filename:      ...sig/code/base/BitFileIn/BitFileIn.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#include "BitFileIn.h"


typedef unsigned char uchar;


//////////////////////////////
//
// BitFileIn::BitFileIn
//

BitFileIn::BitFileIn(void) {
   bitsInByte = 0; 
   inStorage = 0;
};

BitFileIn::BitFileIn(const char* filename, int state) :
#ifdef VISUAL
   ifstream(filename, state | ios::in | ios::binary) { 
#else
   ifstream(filename, state | ios::in) { 
#endif

   bitsInByte = 0; 
   inStorage = 0;
   endoffile = 0;
};



//////////////////////////////
//
// BitFileIn::~BitFileIn
//

BitFileIn::~BitFileIn() {
   // do nothing
}



//////////////////////////////
//
// BitFileIn::readBits
//

long BitFileIn::readBits(int bitCount) {
   int output = 0;

   while (!endoffile && bitCount > 0) {
      setBit(output, bitCount, getBit());
      bitCount--;
   }

   return output;
}


//////////////////////////////
//
// BitFileIn::valid -- returns true if the last bit output
//	did not reach the end of file.
//

int BitFileIn::valid() {
   return !endoffile;
}


///////////////////////////////////////////////////////////////////////////
//
// Private Functions
//


//////////////////////////////
//
// BitFileIn::setBit
//

void BitFileIn::setBit(int& output, int bitCount, int bit) {
   output |= (bit & 0x01) << (bitCount - 1);
}



//////////////////////////////
//
// BitFileIn::getBit
//

int BitFileIn::getBit(void) {
      if (bitsInByte <= 0) {
         if (!eof()) {
            (*this) >> inStorage;
            bitsInByte = 8;
         } 
      }

      if (eof()) {
         endoffile = 1;
         inStorage = 0;
         bitsInByte = 8;
         return 0;
      }

      bitsInByte--;
      return (inStorage >> bitsInByte) & 0x01;
}
   


// md5sum:	006f4dc8a6d5fdb4839587bfce675179  BitFileIn.cpp
