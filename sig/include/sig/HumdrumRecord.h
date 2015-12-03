//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon May 18 13:52:59 PDT 1998
// Last Modified: Mon May 18 13:52:59 PDT 1998
// Last Modified: Fri May  5 13:13:32 PDT 2000 (added sub-spine access)
// Filename:      ...sig/maint/code/museinfo/HumdurmRecord/HumdrumRecord.cpp
// Syntax:        C++ 
// $Smake:        cc -ansi -Isig/include -m486 -O3 -o %b %f -lg++ && strip %b
//

#ifndef _HUMDRUMRECORD_H_INCLUDED
#define _HUMDRUMRECORD_H_INCLUDED

#include "Collection.h"
#include "Enum_exclusiveInterpretation.h"
#include "Enum_humdrumRecord.h"
#include <iostream.h>

class HumdrumRecord {
   public:
                        HumdrumRecord      (void);
                        HumdrumRecord      (const char* aLine);
                        HumdrumRecord      (const HumdrumRecord& aRecord);
                       ~HumdrumRecord      ();
  
      void              addField           (int index, const char* aField,
                                              int anInterp = E_unknown);
      void              appendField        (const char* aField, 
                                              int anInterp = E_unknown);
      void              changeField        (int aField, const char* aString);
      int               equalFieldsQ       (void);
      int               equalFieldsQ       (int anInterp);
      int               equalFieldsQ       (const char* anInterp);
      int               equalFieldsQ       (int anInterp, const char*
                                              compareString);
      int               equalFieldsQ       (const char* anInterp,
                                              const char* compareString);
      int               getExInt           (int fieldIndex) const;
      const char*       getExIntString     (int fieldIndex) const;
      int               getFieldCount      (void) const;
      const char*       getLine            (void); 
      int               getTokenCount      (int fieldIndex);
      int               getToken           (char* buffer, int fieldIndex, 
                                              int tokenIndex);
      int               getType            (void);
      HumdrumRecord&    operator=          (const HumdrumRecord& aRecord);
      HumdrumRecord&    operator=          (const HumdrumRecord* aRecord);
      HumdrumRecord&    operator=          (const char* aRecord);
      const char*       operator[]         (int index) const;
      void              setExInt           (int fieldIndex, int interpretation);
      void              setExInt           (int fieldIndex, 
                                              const char* interpretation);
      void              setLine            (const char* aString); 

   protected:
      int               type;           // category of humdrum record
      char*             recordString;   // record string
      int               modifiedQ;      // boolen for if need to make Rstring
      Collection<char*> recordFields;   // data for humdrum text record
      Collection<int>   interpretation; // exclusive interpretation of data
      
      // private functions
      int               determineFieldCount(const char* aLine) const;
      int               determineType      (const char* aLine) const;
      void              makeRecordString   (void);
      void              storeRecordFields  (void);
};
   

ostream& operator<<(ostream& out, HumdrumRecord& aRecord);



#endif  // _HUMDRUMRECORD_H_INCLUDED


// md5sum:	6ac1a52cbae96ac46679c3cdb9b14dbd  HumdrumRecord.h
