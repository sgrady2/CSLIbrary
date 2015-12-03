//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon May 18 13:43:47 PDT 1998
// Last Modified: Mon May 18 13:43:47 PDT 1998
// Filename:      /linux2/users/craig/class/254/harm/Humdrum.cc
// Syntax:        C++ 
// $Smake:        cc -ansi -m486 -O3 -o %b %f -lg++ && strip %b
//

#ifndef _HUMDRUMFILE_H_INCLUDED
#define _HUMDRUMFILE_H_INCLUDED

#include "HumdrumRecord.h"
#include "Collection.h"
#include <iostream.h>

class HumdrumFile {
   public:
                             HumdrumFile      (void);
                             HumdrumFile      (const HumdrumFile& aHumdrumFile);
                             HumdrumFile      (const char* filename);
                            ~HumdrumFile      ();

      void                   appendLine       (const char* aLine);
      void                   appendLine       (HumdrumRecord& aRecord);
      void                   appendLine       (HumdrumRecord* aRecord);
      void                   appendCompositeDuration (void);
      void                   assignExInt      (void);
      void                   clear            (void);
      HumdrumFile            extract          (int aField);
      const char*            getLine          (int index);
      int                    getNumLines      (void);
      HumdrumRecord&         getRecord        (int index);
      double                 getTiedDuration  (int linenum, int field, 
                                                 int token = 0);
      HumdrumFile&           operator=        (const HumdrumFile& aFile);
      void                   read             (const char* filename);
      void                   read             (istream& inStream);
      HumdrumFile            removeNullRecords(void);
      HumdrumRecord&         operator[]       (int index);
      void                   write            (const char* filename);
      void                   write            (ostream& outstream);

   protected:
      Collection<HumdrumRecord*>  records;

   private:
      void HumdrumFile::processRecordsForDuration(HumdrumFile& infile);
      double determineDuration(const HumdrumRecord& aRecord, int& durint);
};



ostream& operator<<(ostream& out, HumdrumFile& aHumdrumFile);

#endif // _HUMDRUMFILE_H_INCLUDED 


// md5sum:	9eecd41cbfbc4be79239b1069a0dd164  HumdrumFile.h
