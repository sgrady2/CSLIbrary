//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Nov 17 00:39:26 PST 1996
// Last Modified: Wed Apr 23 22:44:36 GMT-0800 1997
// Filename:      ...sig/code/Filter/Scale/Scale.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _SCALE_H_INCLUDED
#define _SCALE_H_INCLUDED


#include "Filter.h"
#include "Array.h"


class Scale : public Filter {
   public:
      Scale                            (sampleType value = 1);
      void                action       (void);
      sampleType          getScale     (void);
      sampleType          output       (int channel);
      void                setScale     (sampleType newScale);

 
   protected:
      Array<sampleType>   Output;
      sampleType          scaleValue;

};



#endif  // _SCALE_H_INCLUDED



// md5sum:	e22b58a2e34875f3440e71a489e3a3bd  Scale.h
