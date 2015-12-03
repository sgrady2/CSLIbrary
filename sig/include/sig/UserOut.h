//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Apr 29 22:43:03 GMT-0800 1997
// Last Modified: Tue Apr 29 22:43:06 GMT-0800 1997
// Filename:      ...sig/code/Filter/UserOut/UserOut.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _USEROUT_H_INCLUDED
#define _USEROUT_H_INCLUDED


#include "Filter.h"


class UserOut : public Filter {
   public:
                         UserOut        (char *aTag = "UserOut");
      void               action         (void);
      sampleType         output         (int index);
      void               setTag         (char *aTag);
 
   protected:
      char              *tag; 
};



#endif  // _USEROUT_H_INCLUDED



// md5sum:	90a898d531e870adcb5e4e16e8326686  UserOut.h
