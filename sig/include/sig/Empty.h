//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Nov  2 10:25:09 PST 1996
// Last Modified: Wed Apr 23 22:32:31 GMT-0800 1997
// Filename:      .../sig/code/Filter/Empty/Empty.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _EMPTY_H_INCLUDED
#define _EMPTY_H_INCLUDED


#include "Filter.h"


class Empty : public Filter {
   public:
                         Empty          (void);
      void               action         (void);
      sampleType         output         (int index);
};



#endif  // _EMPTY_H_INCLUDED



// md5sum:	b21f93deb61e8e3c4eb32cd6274f75a9  Empty.h
