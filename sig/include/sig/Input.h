//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Apr 22 22:01:39 GMT-0800 1997
// Last Modified: Mon Apr 28 20:47:38 GMT-0800 1997
// Filename:      ...sig/code/Signal/Input/Input.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _INPUT_H_INCLUDED
#define _INPUT_H_INCLUDED


#include "Collection.h"
#include "Array.h"
#include "Signal.h"


class Input {
   public:
                    Input          (void);
                    Input          (Signal& aSignal);
                    Input          (Signal* aSignal);
                    Input          (Signal& aSignal, Signal& bSignal);
                    Input          (Signal* aSignal, Signal* bSignal);
                    Input          (Signal& aSignal, Signal& bSignal, 
                                      Signal& cSignal);
                    Input          (Signal* aSignal, Signal* bSignal, 
                                      Signal* cSignal);
                   ~Input          ();
  
      int           connectionQ    (int index);
      int           getSize        (void);
      Signal*       insert         (Signal& aSignal, int index = -1, 
                                      int channel = 0);
      Signal*       insert         (Signal* aSignal, int index = -1,
                                      int channel = 0);
      void          printChannels  (void);
      Signal*       remove         (int index = -1);
      int           remove         (Signal& aSignal);
      int           remove         (Signal* aSignal);
      Signal*       signal         (int index);
      sampleType    setDefault     (int index, sampleType aNewDefault);
      void          tick           (long newTickNumber);

      sampleType    operator[]     (int index);

   protected:
      Collection<Signal*> inputs;    
      Array<int>          channels;
      Array<sampleType>   defaults;
};



#endif  // _INPUT_H_INCLUDED



// md5sum:	c813e6dca580558de195130eece22b05  Input.h
