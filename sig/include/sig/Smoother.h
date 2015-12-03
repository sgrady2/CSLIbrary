//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Feb  8 23:01:48 PST 1999
// Last Modified: Mon Feb  8 23:01:56 PST 1999
// Filename:      ...sig/code/Filter/Smoother/Smoother.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _SMOOTHER_H_INCLUDED
#define _SMOOTHER_H_INCLUDED


#include "Filter.h"


class Smoother : public Filter {
   public:
                         Smoother         (sampleType aFactor = 0.1);
                        ~Smoother         ();
      sampleType         getFactor        (void);
      sampleType         getInput         (void);
      void               action           (void);
      sampleType         output           (int index);
      void               setFactor        (sampleType aFactor);
      void               setFactor        (Signal& aSignal, int outputChannel);
      void               setFactor        (Signal* aSignal, int outputChannel);
      void               setInput         (sampleType anInput);
      void               setInput         (Signal& aSignal, int outputChannel);
      void               setInput         (Signal* aSignal, int outputChannel);
 
   protected:
      sampleType         currentOutput;

};



#endif  // _SMOOTHER_H_INCLUDED



// md5sum:	e9f07e40882fca995e2e9980d18adf90  Smoother.h
