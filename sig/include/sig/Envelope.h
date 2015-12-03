//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri May  2 15:18:35 GMT-0800 1997
// Last Modified: Thu Nov 27 21:29:49 GMT-0800 1997
// Filename:      ...sig/code/Generator/Envelope/Envelope.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _ENVELOPE_H_INCLUDED 
#define _ENVELOPE_H_INCLUDED 


#include "Generator.h"
#include "EnvelopeString.h"


class Envelope : public Generator, public EnvelopeString {
   public:
                      Envelope           (const char *aString = "(0 0; 1 0)", 
                                            int durationInSamples = 0);
                     ~Envelope           ();
                      
      void            action             (void);
      int             activeQ            (void);
      long            getDurationSamples (void);
      double          getDurationSeconds (void);
      long            getEnvelopeSamples (void);
      sampleType      output             (int channel);
      void            printState         (void);
      void            setDuration        (long samples);
      void            setDuration        (int samples);
      void            setDuration        (double seconds);
      void            setDurationSamples (long samples);
      void            setDurationSeconds (double seconds);
      void            trigger            (int startPoint = 0);

   protected:
      long            currPoint;        // current point
      long            currSample;       // current sample
      Array<long>     sampleTimes;      // points as samples
      int             active;           // true if generating envelope
      char            interpType;       // current interpolation type
      sampleType      outputValue;      // value to be sent out of the object
      sampleType      linearIncrement;
      sampleType      geometricIncrement;

      // private functions:
      void            calculateNextSample (void);
};



#endif  // _ENVELOPE_H_INCLUDED 



// md5sum:	cce30d6cb9b4596be6c7186a82f7aaf3  Envelope.h
