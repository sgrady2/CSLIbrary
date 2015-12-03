//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Nov  8 21:08:39 GMT-0800 1997
// Last Modified: Sat Nov  8 21:08:45 GMT-0800 1997
// Filename:      ...sig/code/base/LookupTable/LookupTable.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include %
//                -ILookupTableCI && rm -f %b.o
//

#ifndef _LOOKUPTABLE_H_INCLUDED
#define _LOOKUPTABLE_H_INCLUDED


#include "LookupTableCI.h"
#include "sigTypes.h"


class LookupTable : public LookupTableCI {
   public:
                         LookupTable             (int size = 0);
                         LookupTable             (sampleType* aTable, int size);
                         LookupTable             (const LookupTable& aTable);
                        ~LookupTable             ();

      void               doLinearInterpolation   (void);
      void               doConstantInterpolation (void);
      LookupTable&       operator=               (LookupTable& aTable);

   protected:
      int                interpolationType;

      sampleType         read                    (double index) const;
};



#endif  // _LOOKUPTABLE_H_INCLUDED



// md5sum:	afec362047efb807b56cbd4daa318114  LookupTable.h
