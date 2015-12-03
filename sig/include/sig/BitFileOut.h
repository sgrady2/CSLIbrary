//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri May  1 20:00:52 PDT 1998
// Last Modified: Fri May  8 21:08:56 PDT 1998
// Filename:      ...sig/code/base/BitFileOut/BitFileOut.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//


#ifndef _BITFILEOUT_H_INCLUDED
#define _BITFILEOUT_H_INCLUDED


#include <fstream.h>


class BitFileOut : public ofstream {
   public:
                     BitFileOut         (void);
                     BitFileOut         (const char* filename, int state = 0);
                    ~BitFileOut         ();

      void           writeBits          (long bits, int bitCount);
      int            byteAlign          (void);


   protected:
      int            bitsInByte;        // num of temp stored bits in storage
      int            outStorage;        // temporary storage for output
};


#endif  // _BITFILEOUT_H_INCLUDED



// md5sum:	3bf3999067970d05b75d374f82449226  BitFileOut.h
