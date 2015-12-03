//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue May  6 20:07:25 GMT-0800 1997
// Last Modified: Tue May  6 20:07:29 GMT-0800 1997
// Filename:      ...sig/code/Filter/Selector/Selector.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _SELECTOR_H_INCLUDED
#define _SELECTOR_H_INCLUDED


#include "Filter.h"


class Selector : public Filter {
   public:
                         Selector       (double aChannel = 0);
      void               action         (void);
      double             getChannel     (void);
      sampleType         output         (int index);
      void               setChannel     (double aChannel);
 
   protected:
      double             streamChannel;
};



#endif  // _SELECTOR_H_INCLUDED



// md5sum:	9a81af37e6bffb193875145e537d98b0  Selector.h
