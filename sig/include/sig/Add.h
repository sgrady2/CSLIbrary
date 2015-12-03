//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Nov  2 10:25:09 PST 1996
// Last Modified: Wed Apr 23 22:32:31 GMT-0800 1997
// Filename:      ...sig/code/Filter/Add/Add.h
// Web Address:   http://sig.sapp.org/include/sig/Add.h
// Syntax:        C++ 
//

#ifndef _ADD_H_INCLUDED
#define _ADD_H_INCLUDED


#include "Filter.h"


class Add : public Filter {
   public:
                      Add             (void);
                     ~Add             ();
      void            action          (void);
      void            clear           (void);
      sampleType      output          (int index);
      void            printState      (void);
 
   protected:
      sampleType      outputValue;    // stores the sum of the inputs
};


#endif  // _ADD_H_INCLUDED



// md5sum:        448db5a4483131268b52513419d7b3ee Add.h
