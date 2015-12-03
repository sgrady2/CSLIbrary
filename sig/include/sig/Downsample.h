//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun May 18 17:47:39 GMT-0800 1997
// Last Modified: Sun May 18 17:47:42 GMT-0800 1997
// Filename:      .../sig/code/Filter/Downsample/Downsample.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _DOWNSAMPLE_H_INCLUDED
#define _DOWNSAMPLE_H_INCLUDED


#include "Filter.h"


class Downsample : public Filter {
   public:
                         Downsample     (int aPeriod);
                        ~Downsample     ();
      void               action         (void);
      int                getPeriod      (void);
      sampleType         output         (int index);
      void               setPeriod      (int aPeriod, int counter = -1);
      void               tick           (int newTickNumber);
      void               trigger        (int counter = 0);
 
   protected:
      int period;
      int periodCount;
};



#endif  // _DOWNSAMPLE_H_INCLUDED



// md5sum:	5c84575c159e23b3f19ba0c212c9a3ca  Downsample.h
