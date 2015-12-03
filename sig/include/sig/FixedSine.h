//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Apr 30 14:55:47 GMT-0800 1997
// Last Modified: Wed Apr 30 14:55:51 GMT-0800 1997
// Filename:      ...sig/code/Generator/FixedSine/FixedSine.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _FIXEDSINE_H_INCLUDED 
#define _FIXEDSINE_H_INCLUDED 


#include "Generator.h"


class FixedSine : public Generator {
   public:
                      FixedSine         (float aFrequency, 
                                            float anAmplitude = 1.0, 
                                            float aPhase = 0.0,
                                            float aSrate = 44100.0);
                     ~FixedSine         ();
                      
      void            action            (void);
      float           getAmplitude      (void);
      float           getFrequency      (void);
      float           getPhase          (void);
      sampleType      output            (int channel);
      void            printState        (void);
      void            reset             (void);
      void            setAmplitude      (float anAmplitude);
      void            setFrequency      (float aFequency);
      void            setPhase          (float aPhase);

   protected:
      sampleType      outputValue;
      ulong           sampleNum;
      float           amplitude;
      float           angularFrequency;
      float           phase;
};



#endif  // _FIXEDSINE_H_INCLUDED 



// md5sum:	d8018bc51b74ba947ba3ea4934af588c  FixedSine.h
