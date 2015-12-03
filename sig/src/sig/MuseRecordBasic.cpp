//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Jun 30 21:44:58 PDT 1998
// Last Modified: Tue Jun 30 21:45:02 PDT 1998
// Last Modified: Thu Jul  1 16:28:54 PDT 1999
// Filename:      ...humdrum++/src/MuseRecordBasic.cpp
// Syntax:        C++
// $Smake:        cc -g -c -I../include %f && rm %b.o
//

#include "MuseRecordBasic.h"
#include <string.h>


//////////////////////////////
//
// MuseRecordBasic::MuseRecordBasic
//

MuseRecordBasic::MuseRecordBasic(void) {
   recordString = new char[81];
   setLine("");
   setType(E_unknown);
}


MuseRecordBasic::MuseRecordBasic(const char* aLine) {
   recordString = new char[81];
   setLine(aLine);
   setType(aLine[0]);
}


MuseRecordBasic::MuseRecordBasic(const MuseRecordBasic& aRecord) {
   recordString = new char[81];
   setLine(aRecord.getLine());
   setType(aRecord.getType());
}



//////////////////////////////
//
// MuseRecordBasic::~MuseRecordBasic
//

MuseRecordBasic::~MuseRecordBasic() {
   if (recordString != NULL) {
      delete [] recordString;
      recordString = NULL;
   }
}



//////////////////////////////
//
// MuseRecordBasic::extract -- extracts the character columns from the
//	storage string.  Appends a null character to the end of the
//	copied string.
//

void MuseRecordBasic::extract(char* output, int start, int end) {
   output[0] = '\0';
   int i;
   for (i=0; i<=end-start; i++) {
      output[i] = getColumn(i+start);
   }
   output[i] = '\0';
}



//////////////////////////////
//
// MuseRecordBasic::getColumn -- same as operator[] but with an
//	offset of 1 rather than 0.
//

char& MuseRecordBasic::getColumn(int columnNumber) {
   int realindex = columnNumber - 1;
   if (realindex < 0 || realindex >= 80) {
      cerr << "Error trying to access column: " << columnNumber  << endl;
      exit(1);
   } else if (realindex >= length) {
      for (int i=length; i<=realindex; i++) {
         recordString[i] = ' ';
      }
      length = realindex + 1;
      recordString[length] = '\0';
   }

   return recordString[realindex];
}




//////////////////////////////
//
// MuseRecordBasic::getLength -- returns the size of the
//	character string being stored.  A number between
//	0 and 80.
//

int MuseRecordBasic::getLength(void) const {
   return length;
}



//////////////////////////////
//
// MuseRecordBasic::getLine -- returns a pointer to data record
//

const char* MuseRecordBasic::getLine(void) const {
   return recordString;
}



//////////////////////////////
//
// MuseRecordBasic::getType -- returns the type of the record.
//

int MuseRecordBasic::getType(void) const {
   return type;
}



//////////////////////////////
//
// MuseRecordBasic::operator=
//

MuseRecordBasic& MuseRecordBasic::operator=(const MuseRecordBasic& aRecord) {
   // don't copy onto self
   if (&aRecord == this) {
      return *this;
   }

   setLine(aRecord.getLine());
   setType(aRecord.getType());

   return *this;
}


MuseRecordBasic& MuseRecordBasic::operator=(const MuseRecordBasic* aRecord) {
   *this = *aRecord;
   return *this;
}


MuseRecordBasic& MuseRecordBasic::operator=(const char* aLine) {
   setLine(aLine);
   setType(aLine[0]);
   return *this;
}



//////////////////////////////
//
// MuseRecordBasic::operator[] -- character array offset from 0.
//

char& MuseRecordBasic::operator[](int index) {
   return getColumn(index+1);
}



//////////////////////////////
//
// MuseRecordBasic::setLine -- sets the record to a (new) string
//

void MuseRecordBasic::setLine(const char* aLine) {
   if (recordString == NULL) {
      cerr << "Error: expecting a non-null string space" << endl;
      exit(1);
   }

   length = strlen(aLine);
   if (length > 80) {
      cerr << "Error: line is too long: " << aLine << endl;
   }
   strcpy(recordString, aLine);
}



//////////////////////////////
//
// MuseRecordBasic::setType -- sets the type of the record
//

void MuseRecordBasic::setType(int aType) {
   switch (aType) {
      case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
         type = 'A';
         break;
      default:
         type = aType;
   }
}



//////////////////////////////
//
// MuseRecordBasic::shrink -- removes trailing spaces in a MuseData record
//

void MuseRecordBasic::shrink(void) {
   int i = length - 1;
   while (i >= 0 && recordString[i] == ' ') {
      recordString[i] = '\0';
      i--;
      length--;
   }
}



///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// operator<< 
//

ostream& operator<<(ostream& out, MuseRecordBasic& aRecord) {
   aRecord.shrink();
   out << aRecord.getLine();
   return out;
}




// md5sum:	17da8974becd492dfd9f275dfcc4004e  MuseRecordBasic.cpp
