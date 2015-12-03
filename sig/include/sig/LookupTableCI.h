//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 27 00:51:22 GMT-0800 1997
// Last Modified: Sun Apr 27 00:51:22 GMT-0800 1997
// Filename:      .../sig/code/base/LookupTable/LookupTableCI/LookupTableCI.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../../include && rm -f %b.o
//

#ifndef _LOOKUPTABLECI_H_INCLUDED
#define _LOOKUPTABLECI_H_INCLUDED

#define LOOKUP_CONSTANT (0)
#define LOOKUP_LINEAR   (1)

#include "sigTypes.h"


class LookupTableCI {
   public:
                         LookupTableCI     (int size = 0);
                         LookupTableCI     (sampleType* aTable, int size);
                         LookupTableCI     (LookupTableCI& aTable);
                        ~LookupTableCI     ();

      sampleType*        base              (void) const;
      void               borrow            (sampleType* aTable, int size);
      void               borrow            (LookupTableCI& aTable);
      int                borrowQ           (void) const;
      long               getSize           (void) const;
      LookupTableCI&     operator=         (LookupTableCI& aTable);
      LookupTableCI&     operator+=        (LookupTableCI& aTable);
      LookupTableCI&     operator-=        (LookupTableCI& aTable);
      LookupTableCI&     operator*=        (LookupTableCI& aTable);
      LookupTableCI&     operator*=        (double amplitude);
      LookupTableCI&     operator*=        (int amplitude);
      sampleType&        operator[]        (long readIndex);
      sampleType         operator[]        (double readIndex);
      void               setSize           (long size);
      void               zero              (sampleType aValue = 0);

   protected:
      long               maxSize;
      sampleType*        tableData;
      char               borrowTableQ;

      sampleType&        read              (long index);
      sampleType         read              (double index) const;
};



#endif  _LOOKUPTABLECI_H_INCLUDED



// md5sum:	6b3b5f1e86dc554b03f94144148d8996  LookupTableCI.h
