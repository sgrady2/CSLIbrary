//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 20 21:00:17 GMT-0800 1997
// Last Modified: Fri Mar 27 01:44:43 GMT-0800 1998
// Filename:      ...sig/code/Filter/Allpass/Allpass.h
// Web Address:   http://sig.sapp.org/include/sig/Allpass.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _ALLPASS_H_INCLUDED
#define _ALLPASS_H_INCLUDED


#include "Filter.h"
#include "Delay.h"


class Allpass : public Filter {
   public:
                      Allpass       (int aMaxDelay = 4000);
                     ~Allpass       ();

      void            action        (void);
      void            clear         (void);
      sampleType      getDelay      (void);
      sampleType      getGain       (void);
      sampleType      getInput      (void);
      sampleType      getLoopTime   (void);
      sampleType      getMaxDelay   (void);
      sampleType      getReverbTime (void);
      sampleType      output        (int channel = 0);
      void            setDelay      (sampleType aValue);
      void            setDelay      (Signal& aSignal, int outputChannel);
      void            setDelay      (Signal* aSignal, int outputChannel);
      void            setGain       (sampleType aValue);
      void            setGain       (Signal& aSignal, int outputChannel = 0);
      void            setGain       (Signal* aSignal, int outputChannel = 0);
      void            setInput      (sampleType aValue);
      void            setInput      (Signal& aSignal, int outputChannel = 0);
      void            setInput      (Signal* aSignal, int outputChannel = 0);
      void            setLoopTime   (sampleType aLoopTime);
      void            setMaxDelay   (int aMax);
      void            setReverbTime (sampleType aReverbTime);
 
   protected:
      sampleType      outputValue;  // current output of filter
      Delay           delayline;    // delay storage 
};


#endif  // _ALLPASS_H_INCLUDED



// md5sum:	b8305d6283179040f34dd71c66eb8871  Allpass.h
