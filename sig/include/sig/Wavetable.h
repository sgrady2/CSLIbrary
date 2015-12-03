//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon May  5 22:55:50 GMT-0800 1997
// Last Modified: Mon May  5 22:55:53 GMT-0800 1997
// Filename:      ...sig/src/Filter/WaveTable/WaveTable.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _WAVETABLE_H_INCLUDED
#define _WAVETABLE_H_INCLUDED


#include "Filter.h"
#include "Array.h"
#include "Collection.h"


class Partial {
   public:
      float partial;
      float amplitude;
      float phase;
};


class WaveTable : public Filter {
   public:
                         WaveTable        (int size = 1024);
                        ~WaveTable        ();
      void               action           (void);
      void               normalize        (sampleType maxAmp);
      sampleType         output           (int index);
      void               reverse          (void);
      void               setSize          (long aSize);
      void               setTable         (sampleType *anArray, int aSize);
      void               setTable         (Array<sampleType> anArray);
      void               setTableFromFile (char* filename, int fileType);
      void               setTableHarmonics(char* harmonicString);
      void               setTableHarmonics(Collection<Partial> partialArray);
      void               setTableRamp     (long tableSize = NONE);
      void               setTableSinewave (long tableSize = NONE);
      void               setTableSquare   (long tableSize = NONE);
      void               setTableTriangle (long tableSize = NONE);
      void               shift            (long shiftAmount);

 
   protected:
      sampleType         outputValue;
      double             index;
      Array<sampleType>  waveTable;
      double             magicNumber;
      double             increment;
};



#endif  _WAVETABLE_H_INCLUDED



// md5sum:	ed445c9f69b1dc3aec2ed264ae50900a  Wavetable.h
