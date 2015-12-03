//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Nov  2 10:25:09 PST 1996
// Last Modified: Wed Apr 23 22:36:04 GMT-0800 1997
// Filename:      ...sig/code/Filter/Multiply/Multiply.h
// Syntax:        C++ 
// $Smake:        cc -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _MULTIPLY_H_INCLUDED
#define _MULTIPLY_H_INCLUDED


#include "Filter.h"


class Multiply : public Filter {
   public:
                      Multiply     (void);

      void            action       (void);
      sampleType      output       (int channel);
 
   protected:
      sampleType      Output;
};



#endif  // _MULTIPLY_H_INCLUDED



// md5sum:	ca632f71697699420583af6897197d7f  Multiply.h
