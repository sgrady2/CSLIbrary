//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri May  1 20:14:03 PDT 1998
// Last Modified: Fri May  8 21:10:01 PDT 1998
// Filename:      ...sig/code/base/BitFileOut/BitFileOut.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#include "BitFileOut.h"


typedef unsigned char uchar;


//////////////////////////////
//
// BitFileOut::BitFileOut
//

BitFileOut::BitFileOut(void) {
   bitsInByte = 0; 
   outStorage = 0;
};


BitFileOut::BitFileOut(const char* filename, int state) :
#ifdef VISUAL
   ofstream(filename, state | ios::out | ios::binary) { 
#else
   ofstream(filename, state | ios::out) { 
#endif
   bitsInByte = 0; 
   outStorage = 0;
};



//////////////////////////////
//
// BitFileOut::~BitFileOut
//

BitFileOut::~BitFileOut() {
   byteAlign();
}



//////////////////////////////
//
// BitFileOut::byteAlign -- flushes the bit storage, and
//	set the output bit size to the nearest byte position.
//      Returns the number of padding bits which are set to 
//	zero.
//

int  BitFileOut::byteAlign(void) {
   int output;
   if (bitsInByte != 0) {
      (*this) << (uchar)(outStorage << (8-bitsInByte));
      output = 8 - bitsInByte;
      bitsInByte = 0;
   } else {
      output = 0;
   }
   return output;
}



//////////////////////////////
//
// BitFileOut::writeBits  -- writes the specified number of bits
//	in the storage area to the file.
//

void BitFileOut::writeBits(long bits, int bitCount) {
   // check to see if won't even fill up outStorage
   if (bitCount + bitsInByte < 8) {
      outStorage = (outStorage << bitCount) | ((1<<bitCount)-1 & bits);
      bitsInByte += bitCount;
      return;
   }

   int space;
   while (bitCount > 0) {
      if (bitCount + bitsInByte < 8) {
         outStorage = (outStorage << bitCount) | ((1<<bitCount)-1 & bits);
         bitsInByte += bitCount;
         return;
      } else {
         // fill up outStorage
         space = 8-bitsInByte;
         outStorage = (outStorage << space) | 
            ((1<<space)-1 & bits >> (bitCount - space));
         (*this) << (uchar)(outStorage);
         bitsInByte = 0;
         bitCount -=space;
      }
   }
}



// md5sum:	72acd7c59c7fce770c863ec72c9e77e1  BitFileOut.cpp
