//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon May 18 13:52:59 PDT 1998
// Last Modified: Thu Jun 25 22:05:02 PDT 1998
// Last Modified: Thu Jul  1 16:20:41 PDT 1999
// Last Modified: Thu Apr 13 18:34:28 PDT 2000 (added generic interpretation)
// Last Modified: Fri May  5 13:12:21 PDT 2000 (added sub-spine access)
// Filename:      HumdrumRecord.cpp
// Syntax:        C++ 
// $Smake:        cc -g -c -I../include %f && rm %b.o
//

#include "HumdrumRecord.h"
#include "Convert.h"


#ifndef VISUAL
   #include <strstream.h>     /* in Windows 95, have to include strstrea.h */
#else 
   #include <strstrea.h>
#endif
#include <string.h>


//////////////////////////////
//
// HumdrumRecord::HumdrumRecord
//

HumdrumRecord::HumdrumRecord(void) {
   type = E_unknown;
   recordString = new char[1];
   recordString[0] = '\0';
   modifiedQ = 0;
   recordFields.allowGrowth();
   recordFields.setSize(0);   
   interpretation.allowGrowth();
   interpretation.setSize(0);
}


HumdrumRecord::HumdrumRecord(const char* aLine) {
   type = determineType(aLine);
   recordString = new char[1];
   recordString[0] = '\0';
   modifiedQ = 0;
   interpretation.allowGrowth();
   interpretation.setSize(0);
   recordFields.allowGrowth();
   recordFields.setSize(0);
   setLine(aLine);
}


HumdrumRecord::HumdrumRecord(const HumdrumRecord& aRecord) {
   type = aRecord.type;
   recordString = new char[strlen(aRecord.recordString)+1];
   strcpy(recordString, aRecord.recordString);
   modifiedQ = aRecord.modifiedQ;
   interpretation.allowGrowth();
   interpretation.setSize(aRecord.interpretation.getSize());
   recordFields.allowGrowth();
   recordFields.setSize(aRecord.recordFields.getSize());
   for (int i=0; i<aRecord.recordFields.getSize(); i++) {
      interpretation[i] = aRecord.interpretation[i];
      recordFields[i] = new char[strlen(aRecord.recordFields[i]) + 1];
      strcpy(recordFields[i], aRecord.recordFields[i]);
   }
}



//////////////////////////////
//
// HumdrumRecord::~HumdrumRecord
//

HumdrumRecord::~HumdrumRecord() {
   if (recordString != NULL) {
      delete [] recordString;
      recordString = NULL;
   }
   for (int i=0; i<recordFields.getSize(); i++) {
      delete [] recordFields[i];
      recordFields[i] = NULL;
   }
   recordFields.setSize(0);
}



//////////////////////////////
//
// HumdrumRecord::addField
//     defaule value: anInterp = E_unknown
//	

void HumdrumRecord::addField(int index, const char* aField, int anInterp) {
   if (index < 0) {
      index = getFieldCount();
   }      
   recordFields[recordFields.getSize()] = NULL;
   interpretation[interpretation.getSize()] = E_unknown;

   for (int i=getFieldCount()-2; i>=0; i--) {
      if (i >= index) {
         recordFields[i+1] = recordFields[i]; 
         interpretation[i+1] = interpretation[i]; 
      } else {
         break;
      }
   }

   // add the field
   recordFields[index] = new char[strlen(aField)+1];
   strcpy(recordFields[index], aField);
   interpretation[index] = anInterp;

   modifiedQ = 1;
}



//////////////////////////////
//
// HumdrumRecord::appendField
//     default value: anInterp = E_unknown
//	

void HumdrumRecord::appendField(const char* aField, int anInterp) {
   addField(-1, aField, anInterp);
}



//////////////////////////////
//
// HumdrumRecord::changeField -- changes the contents of a field
//

void HumdrumRecord::changeField(int aField, const char* aString) {
   // don't change self
   if (aString == recordFields[aField]) {
      return;
   }

   delete [] recordFields[aField];
   recordFields[aField] = new char[strlen(aString) + 1];
   strcpy(recordFields[aField], aString);
   
   modifiedQ = 1;
}



//////////////////////////////
//
// HumdrumRecord::equalFieldsQ -- returns true if all record fields
//	are identical
//

int HumdrumRecord::equalFieldsQ(void) {
   int output = 1;
   for (int i=1; i<getFieldCount(); i++) {
      if (strcmp(recordFields[i], recordFields[0]) != 0) {
         output = 0;
         break;
      }
   }
 
   return output;
}



//
// just checks to see if all exclusive interpretation fields are the same
//

int HumdrumRecord::equalFieldsQ(int anInterp) {
   int output = 1;
   int index = 0;
   while (index < getFieldCount() && getExInt(index) != anInterp) {
      index++;
   }
   for (int i=index+1; i<getFieldCount(); i++) {
      if (getExInt(i) == anInterp) {
         if (strcmp(recordFields[index], recordFields[i]) != 0) {
            output = 0;
            break;
         }
      }
   }

   return output;
}


int HumdrumRecord::equalFieldsQ(const char* anInterp) {
   return equalFieldsQ(Convert::exint.getValue(anInterp));
}



int HumdrumRecord::equalFieldsQ(int anInterp, const char* compareString) {
   int output = 1;
   for (int i=0; i<getFieldCount(); i++) {
      if (getExInt(i) == anInterp) {
         if (strcmp(recordFields[i], compareString) != 0) {
            output = 0;
            break;
         }
      }
   }
   return output;
}
 

int HumdrumRecord::equalFieldsQ(const char* anInterp, 
      const char* compareString) {
   return equalFieldsQ(Convert::exint.getValue(anInterp), compareString);
}



//////////////////////////////
//
// HumdrumRecord::getExInt -- returns the exclusive interpretation
//	assigned to the given data field.
//

int HumdrumRecord::getExInt(int fieldIndex) const {
   if (fieldIndex >= interpretation.getSize()) {
      cerr << "Error: accessing too large a field number: " << fieldIndex 
           << endl;
      exit(1);
   }

   return interpretation[fieldIndex];
}


const char* HumdrumRecord::getExIntString(int fieldIndex) const {
   if (fieldIndex >= interpretation.getSize()) {
      cerr << "Error: accessing too large a field number: " << fieldIndex 
           << endl;
      exit(1);
   }
   
   return Convert::exint.getName(interpretation[fieldIndex]);
}



//////////////////////////////
//
// HumdrumRecord::getFieldCount -- returns the number of data
//	fields in the record.
//

int HumdrumRecord::getFieldCount(void) const {
   return recordFields.getSize();
}



//////////////////////////////
//
// HumdrumRecord::getLine -- returns a pointer to data record
//

const char* HumdrumRecord::getLine(void) {
   if (modifiedQ == 0) {
      return recordString;
   } else {
      makeRecordString();
      return recordString;
   }
}



//////////////////////////////
//
// HumdrumRecord::getTokenCount -- returns the number of 
//    tokens in the particular spine (1 + the number of spaces)
//

int HumdrumRecord::getTokenCount(int fieldIndex) {
   int count = 1;
   const char* string = (*this)[fieldIndex];
   int index = 0;
   while (index < 1000 && string[index] != '\0') {
      if (string[index] == ' ') {
         count++;
      }
      index++;
   }
   return count;
}



//////////////////////////////
//
// HumdrumRecord::getToken -- returns the number of 
//    size of the filled buffer.  Puts the token in buffer.
//

int HumdrumRecord::getToken(char* buffer, int fieldIndex, int tokenIndex) {
   int location = 0;
   const char* string = (*this)[fieldIndex];
   char temp[strlen(string) + 1];
   strcpy(temp, string);
   char * current = NULL;
   current = strtok(temp, " ");
   while (current != NULL && location < tokenIndex) {
      current = strtok(NULL, " ");
      location++;
   }

   if (current == NULL) {
      buffer[0] = '\0';
      return 0;
   }
   
   int length = strlen(current);
   strcpy(buffer, current);
   return length;
}



//////////////////////////////
//
// HumdrumRecord::getType -- returns the type of the record.
//

int HumdrumRecord::getType(void) {
   return type;
}



//////////////////////////////
//
// HumdrumRecord::operator=
//

HumdrumRecord& HumdrumRecord::operator=(const HumdrumRecord& aRecord) {
   // don't copy onto self
   if (&aRecord == this) {
      return *this;
   }   

   int i;
   type = aRecord.type;
   if (recordString != NULL) {
      delete [] recordString;
      recordString = NULL;
   }
   if (aRecord.recordString != NULL) {
      recordString = new char[strlen(aRecord.recordString)+1];
      strcpy(recordString, aRecord.recordString);
   }
   modifiedQ = aRecord.modifiedQ;
   interpretation.setSize(aRecord.interpretation.getSize());
   if (recordFields.getSize() != 0) {
      for (i=0; i<recordFields.getSize(); i++) {
         if (recordFields[i] != NULL) {
            delete [] recordFields[i];
            recordFields[i] = NULL;
         }
      }
   }

   recordFields.setSize(aRecord.recordFields.getSize());
   for (i=0; i<getFieldCount(); i++) {
      interpretation[i] = aRecord.interpretation[i];
      recordFields[i] = new char[strlen(aRecord.recordFields[i]) + 1];
      strcpy(recordFields[i], aRecord.recordFields[i]);
   }
 
   return *this;
}


HumdrumRecord& HumdrumRecord::operator=(const HumdrumRecord* aRecord) {
   *this = *aRecord;
   return *this;
}


HumdrumRecord& HumdrumRecord::operator=(const char* aLine) {
   setLine(aLine);   
   return *this;
}



//////////////////////////////
//
// HumdrumRecord::operator[]
//

const char* HumdrumRecord::operator[](int index) const {
   if (index > getFieldCount()) {
      cerr << "Error trying to access field: " << index << endl;
      exit(1);
   } else {
      return recordFields[index];
   }
}



//////////////////////////////
//
// HumdrumRecord::setExInt -- assigns the exclusive interpretation
//	to the specified data field.
//

void HumdrumRecord::setExInt(int index, int anInterpretation) {
   if (index >= interpretation.getSize()) {
      cerr << "Error: accessing too large a field number: " << index 
           << " in line: " << getLine() << endl;
      exit(1);
   }
   
   interpretation[index] = anInterpretation;
}


void HumdrumRecord::setExInt(int index, const char* interpString) {
   if (index >= interpretation.getSize()) {
      cerr << "Error: accessing too large a field number: " << index 
           << " in line: " << getLine() << endl;
      exit(1);
   }

   static int interptypecounter = 10000;

   int interptype = Convert::exint.getValue(interpString);
   if (interptype == E_unknown || interptype == E_UNKNOWN_EXINT) {
      Convert::exint.add(interptypecounter++, interpString);
   }

   interpretation[index] = Convert::exint.getValue(interpString);
}



//////////////////////////////
//
// HumdrumRecord::setLine -- sets the record to a (new) string
//

void HumdrumRecord::setLine(const char* aLine) {
   if (recordString != NULL) {
      delete [] recordString;
      recordString = NULL;
   }
   recordString = new char[strlen(aLine)+1];
   strcpy(recordString, aLine);
   modifiedQ = 0;
   int i;
   if (recordFields.getSize() != 0) {
      for (i=0; i<recordFields.getSize(); i++) {
         delete recordFields[i];
         recordFields[i] = NULL;
      }
      recordFields.setSize(0);
   }
   type = determineType(recordString);
   int fieldCount = determineFieldCount(recordString);

   // allow for null records
   if (fieldCount == 0) {
      fieldCount = 1;
   }

   interpretation.setSize(fieldCount);
   for (i=0; i<fieldCount; i++) {
      interpretation[i] = E_unknown;
   }


   // chop up the input line into data fields to store in array.
   int start = 0;
   int stop = 0;
   i = 0;
   char* temp;
   while (aLine[i] != '\0') {
      if (aLine[i] != '\t') {
         stop++;
      } else {
         temp = new char[stop-start+1];
         strncpy(temp, &aLine[start], stop-start);
         temp[stop-start] = '\0';
         recordFields[recordFields.getSize()] = temp;
         start = stop = i+1;
      }
      i++;
   }
   temp = new char[stop-start+1];
   strncpy(temp, &aLine[start], stop-start);
   temp[stop-start] = '\0';
   recordFields[recordFields.getSize()] = temp;
}


///////////////////////////////////////////////////////////////////////////
//
// private functions
//


//////////////////////////////
//
// HumdrumRecord::makeRecordString
//

void HumdrumRecord::makeRecordString(void) {
   strstream temp;
   char *tab = "\t";
   for (int i=0; i<recordFields.getSize()-1; i++) {
      temp << recordFields[i] << tab;
   }
   temp << recordFields[recordFields.getSize()-1] << ends;

   if (recordString != NULL) {
      delete [] recordString;
   }
   recordString = new char[strlen(temp.str()) + 1];
   strcpy(recordString, temp.str());
   modifiedQ = 0;
}



///////////////////////////////////////////////////////////////////////////
//
// generic functions
//

/////////////////////////////
//
// HumdrumRecord::determineType
//

int HumdrumRecord::determineType(const char* aRecord) const {
   int index = 0;
   
   if (aRecord[index] == '\0') {
      return E_humrec_empty;
   }

   // see if a data interpretation or tandem interpretation
   if (aRecord[index] == '*') {
      if (aRecord[index + 1] == '*') {
         return E_humrec_data_interpretation;
      } else {
         return E_humrec_data_tandem;
      }
   }

   // check if a kern measure
   if (aRecord[0] == '=') {
      return E_humrec_data_kern_measure;
   }

   // if first character is not a '!', then must be data
   if (aRecord[index] != '!') {
      return E_humrec_data;
   } 

   // if only one '!', then this is a local comment in the data
   if (aRecord[index+1] != '!') {
      return E_humrec_data_comment;
   }
      
   // if two '!', then a global comment
   if (aRecord[index+2] != '!') {
      return E_humrec_global_comment;
   } 

   // if three or more '!', then is a file comment
   return E_humrec_comment;
}



//////////////////////////////
//
// determinefieldCount
//

int HumdrumRecord::determineFieldCount(const char* aLine) const {
   int count = 1;
   if (aLine[0] == '\0') {
      return 0;
   }

   int i=0;
   while (aLine[i] != '\0') {
      if (aLine[i] == '\t') {
         count++;
         if (aLine[i+1] == '\0') {
            cerr << "Error: trailing tab character in line: " << aLine << endl;
            exit(1);
         }
      }
      i++;
   }

   return count;
}


///////////////////////////////////////////////////////////////////////////

//////////////////////////////
//
// operator<<
//

ostream& operator<<(ostream& out, HumdrumRecord& aRecord) {
   out << aRecord.getLine();
   return out;
}



// md5sum:	e66761974af68b6fc9e3bd091e41fb32  HumdrumRecord.cpp
