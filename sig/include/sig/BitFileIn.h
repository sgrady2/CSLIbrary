//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri May  1 20:00:52 PDT 1998
// Last Modified: Fri May  1 20:00:56 PDT 1998
// Filename:      ...sig/code/base/BitFileIn/BitFileIn.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//


#ifndef _BITFILEIN_H_INCLUDED
#define _BITFILEIN_H_INCLUDED


#include <fstream.h>


class BitFileIn : public ifstream {
   public:
                     BitFileIn          (void);
                     BitFileIn          (const char* filename, int state);
                    ~BitFileIn          ();

      long           readBits           (int bitCount);
      int            valid              (void);

   protected:
      int            bitsInByte;        // num of temp stored bits in storage
      unsigned char  inStorage;         // temporary storage for output
      int            endoffile;         // true if at end of file

   
      void           setBit             (int& output, int bitCount, int bit);
      int            getBit             (void);
};


#endif  // _BITFILEIN_H_INCLUDED



// md5sum:	8cdd65a9624f35e486bd7039a31992a8  BitFileIn.h
