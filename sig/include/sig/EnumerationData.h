//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Jun  8 20:19:26 PDT 1998
// Last Modified: Mon Jun  8 21:35:18 PDT 1998
// Filename:      ...humdrum++/src/EnumerationData.h
// Syntax:        C++ 
//

#ifndef _ENUMERATIONDATA_H_INCLUDED
#define _ENUMERATIONDATA_H_INCLUDED

#include "Enum_basic.h"
#include <iostream.h>


#define ENUM_FIXED_ALLOC        0
#define ENUM_TRANSIENT_ALLOC    1


class EnumerationData {
   public:
                  EnumerationData         (void);
                  EnumerationData         (int aValue, const char* aName, int
                                             allocType = ENUM_TRANSIENT_ALLOC);
                  EnumerationData         (const EnumerationData& aDatum);
                 ~EnumerationData         ();
 
      int         allocatedQ              (void) const;
      int         compare                 (int aValue) const;
      int         compare                 (const char* aName) const;
      int         compare                 (const EnumerationData& aDatum) const;
      int         compareByValue          (const EnumerationData& aDatum) const;
      int         compareByName           (const EnumerationData& aDatum) const;
      const char* getName                 (void) const;
      int         getValue                (void) const;
      EnumerationData& operator=          (const EnumerationData& aDatum);
      int         operator==              (const EnumerationData& aDatum);
      int         operator<               (const EnumerationData& aDatum);
      int         operator>               (const EnumerationData& aDatum);
      void        setRelation             (int aValue, const char* aName, int
                                             allocType = ENUM_TRANSIENT_ALLOC);
      void        setName                 (const char* aName, int allocType = 
                                             ENUM_TRANSIENT_ALLOC);
      void        setValue                (int aValue);

   protected:
      int value;
      char* name;
      int allocationType;

};

ostream& operator<<(ostream& out, const EnumerationData& aDatum);
ostream& operator<<(ostream& out, const EnumerationData* aDatum);


#endif  // _ENUMERATIONDATA_H_INCLUDED




// md5sum:	27a5e9a28cf60766aee2517b541062b0  EnumerationData.h
