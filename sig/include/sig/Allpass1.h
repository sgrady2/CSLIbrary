//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Jul  6 20:35:20 PDT 1997
// Last Modified: Sat Jul 26 14:53:40 GMT-0800 1997
// Filename:      ...sig/code/Filter/Allpass1/Allpass1.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _ALLPASSONE_H_INCLUDED
#define _ALLPASSONE_H_INCLUDED


#include "Filter.h"


class Allpass1 : public Filter {
   public:
                        Allpass1       (void);
                       ~Allpass1       ();
      void              action         (void);
      void              clear          (void);
      sampleType        getInput       (void);
      sampleType        getGain        (void);
      sampleType        output         (int index);
      void              printState     (void);
      void              setInput       (sampleType aGain);
      void              setInput       (Signal& aSignal, int outputChannel = 0);
      void              setInput       (Signal* aSignal, int outputChannel = 0);
      void              setGain        (sampleType aGain);
      void              setGain        (Signal& aSignal, int outputChannel = 0);
      void              setGain        (Signal* aSignal, int outputChannel = 0);
 
   protected:
      sampleType        outputValue;
      sampleType        holdValue;

};



#endif  // _ALLPASSONE_H_INCLUDED



// md5sum:	8be989f8fb4d622113b5fe0ec01d874f  Allpass1.h
