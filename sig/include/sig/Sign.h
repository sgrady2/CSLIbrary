//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Mar 25 17:09:25 GMT-0800 1998
// Last Modified: Wed Mar 25 17:09:31 GMT-0800 1998
// Filename:      ...sig/code/Filter/Sign/Sign.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _SIGN_H_INCLUDED
#define _SIGN_H_INCLUDED


#include "Filter.h"


class Sign : public Filter {
   public:
                      Sign            (void);
                     ~Sign            ();
      void            action          (void);
      void            clear           (void);
      sampleType      getInput        (void);
      sampleType      output          (int index);
      void            printState      (void);
      void            setInput        (sampleType aValue);
      void            setInput        (Signal& aSignal, int outputChannel = 0);
      void            setInput        (Signal* aSignal, int outputChannel = 0);
 
   protected:
      sampleType      outputValue;    // stores the current output
};


#endif  // _SIGN_H_INCLUDED



// md5sum:	b2112d0bf115ad578b48b2a57ce03ed3  Sign.h
