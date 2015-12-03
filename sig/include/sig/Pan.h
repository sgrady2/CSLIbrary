//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon May 26 18:54:34 GMT-0800 1997
// Last Modified: Sat Jun 21 19:50:14 GMT-0800 1997
// Filename:      .../sig/code/Filter/Pan/Pan.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _PAN_H_INCLUDED
#define _PAN_H_INCLUDED


#include "Filter.h"


#define LINEAR_PAN         (0)
#define CONSTANT_POWER_PAN (1)


class Pan : public Filter {
   public:
                     Pan               (int aPanType = CONSTANT_POWER_PAN);
                    ~Pan               ();
      void           action            (void);
      void           clear             (void);
      void           doLinearPan       (void);
      void           doPowerPan        (void);
      sampleType     getInput          (void);
      sampleType     getPan            (void);
      int            getPanType        (void);
      sampleType     output            (int index);
      void           printState        (void);
      void           setInput          (sampleType aValue);
      void           setInput          (Signal& aSignal, int outputChannel = 0);
      void           setInput          (Signal* aSignal, int outputChannel = 0);
      void           setPan            (sampleType aPanLocation);
      void           setPan            (Signal& aSignal, int outputChannel = 0);
      void           setPan            (Signal* aSignal, int outputChannel = 0);
      void           setPanType        (int aPanType);

   protected:
      sampleType     outputValueLeft;  // left channel pan value
      sampleType     outputValueRight; // right channel pan value
      int            panType;          // stores the panning method

};



#endif  // _PAN_H_INCLUDED



// md5sum:	e02bccca3ad975d877debbf8da89090e  Pan.h
