//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Jun 11 22:17:06 GMT-0800 1997
// Last Modified: Wed Jun 11 22:17:13 GMT-0800 1997
// Filename:      ...sig/code/Filter/RandomDist/RandomDist.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _RANDOMDIST_H_INCLUDED
#define _RANDOMDIST_H_INCLUDED


#include "Filter.h"
#include "Distribution.h"


class RandomDist : public Filter, public Distribution {
   public:
                         RandomDist     (void);
                        ~RandomDist     ();
      void               action         (void);
      sampleType         output         (int index);
 
   protected:
      sampleType         outputValue;
};



#endif  // _RANDOMDIST_H_INCLUDED



// md5sum:	3854420c98eb03f3bbcd7e08409900c8  RandomDist.h
