//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Mar 25 00:31:53 GMT-0800 1998
// Last Modified: Wed Mar 25 00:31:57 GMT-0800 1998
// Filename:      ...sig/code/Filter/DCBlock/DCBlock.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _DCBLOCK_H_INCLUDED
#define _DCBLOCK_H_INCLUDED


#include "Filter.h"


class DCBlock : public Filter {
   public:
                      DCBlock         (void);
                     ~DCBlock         ();
      void            action          (void);
      void            clear           (void);
      sampleType      getInput        (void);
      sampleType      getPolePosition (void);
      sampleType      output          (int index);
      void            setInput        (sampleType aValue);
      void            setInput        (Signal& aSignal, int outputChannel = 0);
      void            setInput        (Signal* aSignal, int outputChannel = 0);
      void            setPolePosition (sampleType aValue);
      void            setPolePosition (Signal& aSignal, int outputChannel = 0);
      void            setPolePosition (Signal* aSignal, int outputChannel = 0);
      void            printState      (void);
 
   protected:
      sampleType      outputValue;    // stores the calculated sample
      sampleType      previousInput;  // the last input signal value 
};


#endif  // _DCBLOCK_H_INCLUDED



// md5sum:	e8b568257d22980e16218e0174d0f639  DCBlock.h
