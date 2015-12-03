//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Jun  7 08:46:00 GMT-0800 1997
// Last Modified: Sat Jun  7 08:46:04 GMT-0800 1997
// Filename:      ...sig/code/Filter/UnitDelay/UnitDelay.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _UNITDELAY_H_INCLUDED
#define _UNITDELAY_H_INCLUDED


#include "Filter.h"


class UnitDelay : public Filter {
   public:
                         UnitDelay      (void);
                        ~UnitDelay      ();
      void               action         (void);
      sampleType         output         (int index);
 
   protected:
      sampleType         currentValue;
      sampleType         outputValue;
};



#endif  // _UNITDELAY_H_INCLUDED



// md5sum:	35abbf977abe09578759cb3e23cb78da  UnitDelay.h
