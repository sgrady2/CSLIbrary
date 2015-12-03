//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Jun 11 07:00:50 GMT-0800 1997
// Last Modified: Wed Jun 11 07:00:53 GMT-0800 1997
// Filename:      .../sig/code/Filter/ReverbA/ReverbA.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _REVERBA_H_INCLUDED
#define _REVERBA_H_INCLUDED

#include "Reverb.h"
#include "Comb.h"
#include "Allpass.h"
#include "Add.h"
#include "Constant.h"
#include "Action.h"


class ReverbA : public Reverb {
   public:
                         ReverbA             (sampleType aReverbTime = 1.0);
                        ~ReverbA             ();
      void               action              (void);
      sampleType         getStoredReverbTime (void);
      sampleType         output              (int index);
      void               setStoredReverbTime (sampleType aReverbTime);
 
   protected:
      sampleType         reverbTime;
      Comb               comb1;
      Comb               comb2;
      Comb               comb3;
      Comb               comb4;
      Allpass            allpass1;
      Allpass            allpass2;
      Add                add;
      Constant           inputSignal;
      Action             act;
};


#endif  _REVERBA_H_INCLUDED



// md5sum:	e89ea8458ca313ae85234fc3cce74836  ReverbA.h
