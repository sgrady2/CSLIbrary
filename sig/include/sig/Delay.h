//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 27 00:51:22 GMT-0800 1997
// Last Modified: Sun Apr 27 00:51:22 GMT-0800 1997
// Filename:      .../sig/code/Filter/DelayLine/Delay.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _DELAY_H_INCLUDED
#define _DELAY_H_INCLUDED


#include "sigTypes.h"


class Delay {
   public:
                         Delay             (int size = 1);
                        ~Delay             ();

      ulong              getSize           (void);
      sampleType&        operator[]        (long readIndex);
      sampleType&        operator[]        (int readIndex);
      sampleType         operator[]        (float readIndex);
      sampleType         operator[]        (double readIndex);
      void               setSize           (long size);
      void               write             (sampleType sample);
      void               zero              (void);

   protected:
      ulong              allocSize;
      ulong              writeIndex;
      sampleType*        array;
      ulong              indexMask;

      ulong              nextPowerOfTwo    (ulong num);
      sampleType&        read              (long index);
      sampleType         read              (float index);
};


#endif  _DELAY_H_INCLUDED

// md5sum:	a7d463fd5f65a027b05fd363112840fe  Delay.h
