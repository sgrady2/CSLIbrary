//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun May 18 19:51:29 GMT-0800 1997
// Last Modified: Sun May 18 19:51:32 GMT-0800 1997
// Filename:      ...sig/code/Filter/Switch/Switch.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _SWITCH_H_INCLUDED
#define _SWITCH_H_INCLUDED


#include "Filter.h"


class Switch : public Filter {
   public:
                         Switch             (int switchSample);
                        ~Switch             ();
      void               action             (void);
      int                getDurationSamples (void);
      sampleType         output             (int index);
      void               setDurationSamples (int switchSample);
      void               tick               (long newTickNumber);
      void               trigger            (void);
 
   protected:
      int                period;
      long               periodCounter;
};



#endif  // _SWITCH_H_INCLUDED



// md5sum:	451b008ef12c42500d3f4487b729619a  Switch.h
