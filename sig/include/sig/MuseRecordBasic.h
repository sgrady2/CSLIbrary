//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Jun 30 11:51:01 PDT 1998
// Last Modified: Tue Jun 30 11:51:05 PDT 1998
// Filename:      ...humdrum++/src/MuseRecordBasic.cpp
// Syntax:        C++ 
// $Smake:        cc -ansi -Isig/include -m486 -O3 -o %b %f -lg++ && strip %b
//

#ifndef _MUSERECORDBASIC_H_INCLUDED
#define _MUSERECORDBASIC_H_INCLUDED

#include "Collection.h"
#include "Enum_muserec.h"
#include <iostream.h>

class MuseRecordBasic {
   public:
                        MuseRecordBasic    (void);
                        MuseRecordBasic    (const char* aLine);
                        MuseRecordBasic    (const MuseRecordBasic& aRecord);
                       ~MuseRecordBasic    ();
  
      void              extract            (char* output, int start, int stop);
      char&             getColumn          (int index);
      int               getLength          (void) const;
      const char*       getLine            (void) const; 
      int               getType            (void) const;
      MuseRecordBasic&  operator=          (const MuseRecordBasic& aRecord);
      MuseRecordBasic&  operator=          (const MuseRecordBasic* aRecord);
      MuseRecordBasic&  operator=          (const char* aRecord);
      char&             operator[]         (int index);
      void              setLine            (const char* aString); 
      void              setType            (int aType);
      void              shrink             (void);

   protected:
      int               type;              // category of MuseRecordBasic record
      char*             recordString;      // record string
      int               length;            // length of string

};
   

ostream& operator<<(ostream& out, MuseRecordBasic& aRecord);



#endif  // _MUSERECORDBASIC_H_INCLUDED


// md5sum:	fe15ed43c5a70a2ec7a33dc446c0a490  MuseRecordBasic.h
