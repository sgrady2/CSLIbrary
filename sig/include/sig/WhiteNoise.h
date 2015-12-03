//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Apr 30 14:25:59 GMT-0800 1997
// Last Modified: Wed Apr 30 14:26:02 GMT-0800 1997
// Filename:      .../sig/code/Generator/WhiteNoise/WhiteNoise.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _WHITENOISE_H_INCLUDED 
#define _WHITENOISE_H_INCLUDED 


#include "Generator.h"


class WhiteNoise : public Generator {
   public:
                      WhiteNoise        (sampleType aMaxAmplitude = 1.0, 
                                           int randomSeed = 0);
                     ~WhiteNoise        ();
                      
      void            action            (void);
      sampleType      getAmplitude      (void);
      sampleType      output            (int channel);
      void            printState        (void);
      void            seed              (int aSeed = 0);
      void            setAmplitude      (sampleType anAmplitude);

   protected:
      sampleType      amplitude;
      sampleType      outputValue;
};



#endif  // _WHITENOISE_H_INCLUDED 



// md5sum:	f24714188b55361050676f616cdba164  WhiteNoise.h
