//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue May 13 22:52:44 GMT-0800 1997
// Last Modified: Tue May 13 22:52:50 GMT-0800 1997
// Filename:      .../sig/code/Filter/OnePole/OnePole.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _ONEPOLE_H_INCLUDED
#define _ONEPOLE_H_INCLUDED


#include "Filter.h"


class OnePole : public Filter {
   public:
                         OnePole        (sampleType aFeeback = 0.0,
                                            sampleType aGain = 1.0);
                        ~OnePole        ();
      void               action         (void);
      sampleType         getFeedback    (void);
      sampleType         getGain        (void);
      sampleType         output         (int index);
      void               setFeedback    (sampleType aFeedback);
      void               setGain        (sampleType aGain);
 
   protected:
      sampleType         outputValue;
      sampleType         previousOutput;
      sampleType         gain;
      sampleType         feedback;
};



#endif  // _ONEPOLE_H_INCLUDED



// md5sum:	e686afb0aabd0b8be39d2a330b83e01c  OnePole.h
