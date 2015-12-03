//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Jun 11 15:34:57 GMT-0800 1997
// Last Modified: Wed Jun 11 20:08:15 GMT-0800 1997
// Filename:      ...sig/code/Filter/Reverberator/Reverberator.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _REVERBERATOR_H_INCLUDED
#define _REVERBERATOR_H_INCLUDED


#include "Filter.h"
#include "Add.h"
#include "Multiply.h"
#include "Constant.h"
#include "ReverbA.h"


#define REVERBA (1)


class Reverberator : public Filter {
   public:
                         Reverberator   (void);
                        ~Reverberator   ();
      void               action         (void);
      sampleType         getReverbAmount(void);
      sampleType         getReverbTime  (void);
      sampleType         getSignal      (void);
      sampleType         output         (int index);
      void               setReverbAmount(sampleType aReverbAmount);
      void               setReverbTime  (sampleType aReverbTime);
      void               setReverbType  (const char* name);
      void               setReverbType  (int reverbType);
 
   protected:
      Add                add;
      Multiply           multiply;
      Constant           inputSignal;
      Constant           reverbAmount;
      Constant           reverbTime;
      Reverb*            reverb;
      Action             act;
};



#endif  // _REVERBERATOR_H_INCLUDED



// md5sum:	1432763aeabc1675ca0dde813cddafb7  Reverberator.h
