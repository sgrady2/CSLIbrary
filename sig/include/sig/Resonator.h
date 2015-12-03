//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed May 14 13:38:42 GMT-0800 1997
// Last Modified: Wed May 14 21:10:15 GMT-0800 1997
// Filename:      ...sig/code/Filter/Resonator/Resonator.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _RESONATOR_H_INCLUDED
#define _RESONATOR_H_INCLUDED


#include "Filter.h"
#include "Delay.h"


class Resonator : public Filter {
   public:
                         Resonator        (sampleType aFrequency = 0.0,
                                             sampleType aBandwidth = 100.0);
                        ~Resonator        ();
      void               action           (void);
      sampleType         getBandwidth     (void);
      sampleType         getFrequency     (void);
      sampleType         getOmega         (void);
      sampleType         getPoleRadius    (void);
      sampleType         output           (int index);
      void               setBandwidth     (sampleType aBandwidth);
      void               setBandwidthForm (void);
      void               setFrequency     (sampleType aFrequency);
      void               setHzForm        (void);
      void               setOmega         (sampleType anOmega);
      void               setOmegaForm     (void);
      void               setPoleRadius    (sampleType aPoleRadius);
      void               setRadiusForm    (void);
 
   protected:
      Delay              out;
      sampleType         radius;
      sampleType         angularFrequency;
      sampleType         frequency;
      char               frequencyForm;
      char               radiusForm;

      sampleType         makeAng          (sampleType aRadius);
      sampleType         makeR            (void);
      void               updateFrequency  (void);

};



#endif  // _RESONATOR_H_INCLUDED



// md5sum:	0e0bfa3d04e805cb1d1813b92e6adf0d  Resonator.h
