//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Nov  1 23:35:02 PST 1996
// Last Modified: Mon Apr 28 20:14:14 GMT-0800 1997
// Filename:      .../sig/code/Filter/Filter.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../include && rm -f %b.o
//

#ifndef _FILTER_H_INCLUDED
#define _FILTER_H_INCLUDED

#include "Generator.h"
#include "Input.h"


class Filter : public Generator {
   public:
                Filter              (void);
               ~Filter              ();

      void      connect             (Signal& in, int slot = NONE, 
                                       int channel = 0);
      void      connect             (Signal* in, int slot = NONE, 
                                       int channel = 0);
      void      connect             (sampleType defaultValue, int slot);
      Signal*   disconnect          (int slot);
      int       disconnect          (Signal& toRemove);
      int       disconnect          (Signal* toRemove);
      void      printConnections    (int flag = 0);
      void      tick                (int newTickNumber);

//   protected:
      Input     inputs;
};


#endif  _FILTER_H_INCLUDED

// md5sum:	d59dd63a02402063531a4399082101b0  Filter.h
