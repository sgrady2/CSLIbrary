//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Jun  8 10:44:32 GMT-0800 1997
// Last Modified: Sun Jun  8 10:44:35 GMT-0800 1997
// Filename:      .../sig/code/Filter/Pan2/Pan2.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _PANTWO_H_INCLUDED
#define _PANTWO_H_INCLUDED

#include "Filter.h"
#include "Delay.h"


class Pan2 : public Filter {
   public:
                         Pan2           (void);
                        ~Pan2           ();
      void               action         (void);
      sampleType         getHeadSize    (void);
      sampleType         getSignal      (void);
      sampleType         getSpeedOfSound(void);
      sampleType         getX           (void);
      sampleType         getY           (void);
      sampleType         output         (int index);
      void               setHeadSize    (sampleType aSize);
      void               setSpeedOfSound(sampleType aSpeed);
      void               setX           (sampleType anX);
      void               setY           (sampleType aY);
 
   protected:
      sampleType         outputValueLeft;
      sampleType         outputValueRight;
      sampleType         distanceToLeftEar;
      sampleType         distanceToRightEar;
      sampleType         sampleDelay;
      sampleType         leftDelayIndex;
      sampleType         rightDelayIndex;
      Delay              delayline;
};


#endif  // _PANTWO_H_INCLUDED



// md5sum:	d3ee5a62284ec900aefb605b0f22e6d3  Pan2.h
