//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Apr 28 21:46:28 GMT-0800 1997
// Last Modified: Mon Apr 28 21:46:28 GMT-0800 1997
// Filename:      .../sig/code/Generator/Impulse/Impulse.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _IMPULSE_H_INCLUDED 
#define _IMPULSE_H_INCLUDED 


#include "Generator.h"


class Impulse : public Generator {
   public:
                      Impulse           (long aPeriod = 0x7fffffff, 
                                           sampleType anAmplitude = 1.0);
                     ~Impulse           ();
                      
      void            action            (void);
      sampleType      getAmplitude      (void);
      long            getPeriod         (void);
      sampleType      output            (int channel);
      void            printState        (void);
      void            setAmplitude      (sampleType anAmplitude);
      void            setPeriod         (long period);
      void            trigger           (long counterValue = 0);

   protected:
      sampleType      outputValue;
      sampleType      amplitude;
      long            period;
      long            counter;
};



#endif  // _IMPULSE_H_INCLUDED 



// md5sum:	a90c85afd88b795417b40dccaa29cc2a  Impulse.h
