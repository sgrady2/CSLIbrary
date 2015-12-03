//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Nov  2 10:25:09 PST 1996
// Last Modified: Wed Apr 23 22:32:31 GMT-0800 1997
// Filename:      ...sig/code/Filter/LogisticLimiter/LogisticLimiter.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _LOGISTICLIMITER_H_INCLUDED
#define _LOGISTICLIMITER_H_INCLUDED


#include "Filter.h"


class LogisticLimiter : public Filter {
   public:
                      LogisticLimiter   (void);
                     ~LogisticLimiter   ();
      void            action            (void);
      void            clear             (void);
      sampleType      output            (int index);
      void            printState        (void);
 
   protected:
      sampleType      outputValue;      // stores the sum of the inputs
};


#endif  // _LOGISTICLIMITER_H_INCLUDED



// md5sum:	da66b4faa5fa98bd5c999bc42cb2d00f  LogisticLimiter.h
