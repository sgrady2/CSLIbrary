//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 20 19:02:37 GMT-0800 1997
// Last Modified: Sun Apr 20 19:02:40 GMT-0800 1997
// Filename:      ...sig/code/Filter/Comb/Comb.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _COMB_H_INCLUDED
#define _COMB_H_INCLUDED


#include "Filter.h"
#include "Delay.h"


class Comb : public Filter {
   public:
                         Comb           (void);
                        ~Comb           ();
      void               action         (void);
      void               clear          (void);
      sampleType         getDelay       (void);
      sampleType         getFund        (void);
      sampleType         getGain        (void);
      sampleType         getMinFund     (void);
      sampleType         getSignal      (void);
      sampleType         output         (int index);
      void               setDelay       (sampleType aDelay);
      void               setFund        (sampleType aFund);
      void               setLoopTime    (sampleType aLoopTime);
      void               setMinFund     (sampleType aMin);
      void               setReverbTime  (sampleType aReverbTime);
      void               setGain        (sampleType aScale);
 
   protected:
      sampleType         outputValue;
      Delay              delayline;
};



#endif  // _COMB_H_INCLUDED



// md5sum:	4315d2b079e82f662befbaa90bff1124  Comb.h
