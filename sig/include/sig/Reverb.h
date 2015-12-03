//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Jun 11 18:55:03 GMT-0800 1997
// Last Modified: Wed Jun 11 18:55:05 GMT-0800 1997
// Filename:      .../sig/code/Reverb/Reverb.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _REVERB_H_INCLUDED
#define _REVERB_H_INCLUDED

#include "Filter.h"


class Reverb : public Filter {
   public:
                         Reverb            (void);
                        ~Reverb            ();
      sampleType         getReverbTime     (void);
      sampleType         getSignal         (void);
      void               setReverbTime     (sampleType aReverbTime);
};


#endif  _REVERB_H_INCLUDED

// md5sum:	44cfe626185aa847074d866d1008d86c  Reverb.h
