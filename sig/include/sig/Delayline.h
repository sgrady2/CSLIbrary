//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 27 11:19:52 GMT-0800 1997
// Last Modified: Sun Apr 27 11:19:55 GMT-0800 1997
// Filename:      ...sig/code/Filter/Delayline/Delayline.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _DELAYLINE_H_INCLUDED
#define _DELAYLINE_H_INCLUDED

#include "Delay.h"
#include "Filter.h"


class Delayline : public Filter, public Delay {
   public:
                       Delayline          (int delay = 1);
                      ~Delayline          ();

      void             action             (void);
      ulong            getDelay           (void);
      sampleType       output             (int index);
      sampleType       output             (float index);
      sampleType       output             (double index)
                                             { return output((float)index); };
      void             setDelay           (long delayamount);

   protected:
      long             delayAmount;

};



#endif  // _DELAYLINE_H_INCLUDED



// md5sum:	a7eeea9ade232ba03d9ca3cd4f0be442  Delayline.h
