//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Apr 28 21:46:28 GMT-0800 1997
// Last Modified: Mon Apr 28 21:46:28 GMT-0800 1997
// Filename:      .../sig/code/Generator/Constant/Constant.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _CONSTANT_H_INCLUDED 
#define _CONSTANT_H_INCLUDED 


#include "Generator.h"


class Constant : public Generator {
   public:
                      Constant          (sampleType aValue = 0);
                     ~Constant          ();
                      
      void            action            (void);
      sampleType      output            (int channel);
      void            printState        (void);
      void            setValue          (sampleType aValue);

   protected:
      sampleType      value;
};



#endif  // _CONSTANT_H_INCLUDED 



// md5sum:	1c5183c8329ec42db82f117b2f6aa28d  Constant.h
