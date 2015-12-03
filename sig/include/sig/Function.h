//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Apr 19 20:45:43 GMT-0800 1997
// Last Modified: Sat Apr 19 20:45:46 GMT-0800 1997
// Filename:      ...sig/code/Generator/Function/Function.h
// Web Address:   http://sig.sapp.org/include/sig/Function.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _FUNCTION_H_INCLUDED 
#define _FUNCTION_H_INCLUDED 


#include "Generator.h"

typedef sampleType (*funcPtr)(double timeValue);


class Function : public Generator {
   public:
                      Function          (funcPtr aFunction);
                     ~Function          ();
                      
      void            action            (void);
      sampleType      output            (int channel);
      void            reset             (double newTime = 0);
      void            setFunction       (funcPtr aFunction);
      void            printState        (void);

   protected:
      funcPtr         generatingFunction;
      double          timeVariable;
      sampleType      outputValue;
};



#endif  // _FUNCTION_H_INCLUDED 



// md5sum:        192c92d93a338ca49fbe617164770dcf  Function.h
