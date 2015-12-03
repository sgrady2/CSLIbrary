//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun May 11 21:57:56 GMT-0800 1997
// Last Modified: Sun May 11 21:57:59 GMT-0800 1997
// Filename:      .../sig/code/Filter/Osc/Osc.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _OSC_H_INCLUDED
#define _OSC_H_INCLUDED


#include "Filter.h"
#include "PeriodTable.h"


#define OSC_HIGH_QUALITY   (0)
#define OSC_MEDIUM_QUALITY (1)
#define OSC_LOW_QUALITY    (2)

class Osc : public Filter {
   public:
                      Osc              (sampleType anAmplitude = 1.0);
                     ~Osc              ();
      void            action           (void);
      void            doHighQuality    (void);
      void            doMediumQuality  (void);
      void            doLowQuality     (void);
      sampleType      getAmplitude     (void);
      sampleType      getFrequency     (void);
      sampleType      output           (int index);
      void            setAmplitude     (sampleType anAmplitude);
      void            setPhase         (sampleType aNormalizedPhase);
 
   protected:
      sampleType      amplitude;       // used if no connection inputs[1]
      sampleType      outputValue;
      double          phaseSum;        // phase increment of frequency
      float           srate;
      int             quality;
      double          tableIncrement;

      static int         initSineTable;
      static PeriodTable sineTable;
};



#endif  // _OSC_H_INCLUDED



// md5sum:	4f82f69cfa081d1f9a15e9fbeaf96150  Osc.h
