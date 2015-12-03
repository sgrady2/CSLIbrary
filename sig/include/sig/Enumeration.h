//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Jun  8 20:19:26 PDT 1998
// Last Modified: Mon Jun  8 21:35:18 PDT 1998
// Filename:      ...humdrum++/src/Enumeration.h
// Syntax:        C++ 
//

#ifndef _ENUMERATION_H_INCLUDED
#define _ENUMERATION_H_INCLUDED


#include "EnumerationData.h"
#include "Collection.h"
#include <iostream.h>


class Enumeration {
   public:
                  Enumeration            (void);
                  Enumeration            (const Enumeration& anEnumeration);
                 ~Enumeration            ();

      void        add                    (int aValue, const char* aString, int 
                                            allocType = ENUM_TRANSIENT_ALLOC);
      void        add                    (const EnumerationData& aDatum);
      int         associate              (const char* aName, int
                                            allocType = ENUM_TRANSIENT_ALLOC);
      const char* getAssociation         (int aValue);
      int         getAssociation         (const char* aName);
      const char* getName                (int aValue);
      int         getValue               (const char* aName);
      int         memberQ                (int aValue);
      int         memberQ                (const char* aName);
      int         memberQ                (const EnumerationData& aDatum);
      int         getFreeValue           (void);
      void        print                  (ostream& out);
      void        printByValue           (ostream& out);
      void        printByName            (ostream& out);
      void        setNullName            (const char* aName, int 
                                            allocType = ENUM_TRANSIENT_ALLOC);

      
   protected:
      Collection<EnumerationData> associations;
      Collection<EnumerationData*> sortByValue;
      Collection<EnumerationData*> sortByName;
      int sortQ;

   // protected functions:
      void        checksort              (void);
      void        sort                   (void);

      static int data_compare(const void* a, const void* b);
      static int data_compare_by_value(const void* a, const void* b);
      static int data_compare_by_name(const void* a, const void* b);
      static int data_compare_by_value_only(const void* a,const void* b);
      static int data_compare_by_name_only(const void* a, const void* b);

};


ostream& operator<<(ostream& out, const EnumerationData& aDatum);
ostream& operator<<(ostream& out, const EnumerationData* aDatum);


#endif  // _ENUMERATIONDATA_H_INCLUDED



// md5sum:	1b17d220a92334c27f24a28beaab08be  Enumeration.h
