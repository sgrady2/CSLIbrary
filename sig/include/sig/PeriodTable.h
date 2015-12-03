//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Sep 15 12:16:30 GMT-0800 1997
// Last Modified: Mon Sep 15 12:16:36 GMT-0800 1997
// Filename:      ...sig/code/LookupTable/PeriodTable/PeriodTable.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../../include && rm -f %b.o
//

#ifndef _PERIODTABLE_H_INCLUDED
#define _PERIODTABLE_H_INCLUDED


#include "LookupTable.h"
#include "sigTypes.h"
#include "sigDefines.h"


class PeriodTable : public LookupTable {
   public:
                         PeriodTable   (int size = 0);
                         PeriodTable   (sampleType* aTable, int size);
                         PeriodTable   (PeriodTable& aTable);
                         PeriodTable   (char* ostString, int size = 1024);
                        ~PeriodTable   ();
                      
      void               add           (int periods, double amp, 
                                          double phase = 0);
      void               add           (char* oscString);
      void               flip          (void);
      void               leftShift     (int amount);
      void               print         (void);
      void               ramp          (double amplitude = 1);
      void               remove        (int periods, double amp, 
                                          double phase = 0);
      void               reverse       (void);
      void               rightShift    (int amount);
      void               subtract      (char* oscString);
      void               sine          (double amplitude = 1);
      void               square        (double amplitude = 1);
      void               triangle      (double amplitude = 1); 
};



#endif  // _PERIODTABLE_H_INCLUDED



// md5sum:	eee022b15716ec59d4fd5dbadab4e51d  PeriodTable.h
