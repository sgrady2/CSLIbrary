//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Nov  8 21:08:39 GMT-0800 1997
// Last Modified: Sat Nov  8 21:08:45 GMT-0800 1997
// Filename:      ...sig/code/LookupTable/LookupTableLI/LookupTableLI.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../../include && rm -f %b.o
//

#ifndef _LOOKUPTABLELI_H_INCLUDED
#define _LOOKUPTABLELI_H_INCLUDED


#include "LookupTableCI.h"
#include "sigTypes.h"


class LookupTableLI : public LookupTableCI {
   public:
                         LookupTableLI           (int size = 0);
                         LookupTableLI           (sampleType* aTable, int size);
                         LookupTableLI           (LookupTableLI& aTable);
                        ~LookupTableLI           ();

   protected:

      sampleType         read                    (double index) const;
};



#endif  // _LOOKUPTABLELI_H_INCLUDED



// md5sum:	06d778a644d477ae48600868f228ae4d  LookupTableLI.h
