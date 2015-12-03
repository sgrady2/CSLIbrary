//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Jun  8 20:19:26 PDT 1998
// Last Modified: Mon Jun  8 21:11:00 PDT 1998
// Last Modified: Thu Jul  1 16:19:16 PDT 1999
// Filename:      ...humdrum++/src/EnumerationData.cpp
// Syntax:        C++ 
// $Smake:        cc -O3 -g -c -Wall -I../include %f && rm %b.o
//

#include "EnumerationData.h"
#include "function_information.h"
#include <string.h>
#include <iostream.h>
#include <stdlib.h>


//////////////////////////////
//
// EnumerationData::EnumerationData
//

EnumerationData::EnumerationData(void) {
   function_information("EnumerationData::EnumerationData(void)")

   value = E_unknown;
   name = NULL;
   allocationType = ENUM_TRANSIENT_ALLOC;
}


EnumerationData::EnumerationData(int aValue, const char* aName, int allocType) {
   function_information("EnumerationData::EnumerationData(int, const char*, int)")

   value = aValue;
   allocationType = allocType;
   if (allocatedQ()) {
      name = new char[strlen(aName)+1];
      strcpy(name, aName);
   } else {
      name = (char*)aName;
   }
}


EnumerationData::EnumerationData(const EnumerationData& aDatum) {
   function_information("EnumerationData::EnumerationData(const EnumerationData&)")

   value = aDatum.value;
   allocationType = aDatum.allocationType;
   if (allocatedQ()) {
      name = new char[strlen(aDatum.name)+1];
      strcpy(name, aDatum.name);
   } else {
      name = aDatum.name;
   }
}



//////////////////////////////
//
// EnumerationData::~EnumerationData
//

EnumerationData::~EnumerationData(void) {
   function_information("EnumerationData::~EnumerationData")

   value = E_unknown;
   if (allocatedQ()) {
      delete [] name;
      name = NULL;
      allocationType = ENUM_TRANSIENT_ALLOC;
   } else {
      name = NULL;
      allocationType = ENUM_TRANSIENT_ALLOC;
   }
}
    


//////////////////////////////
//
// EnumerationData::allocatedQ -- return true if the internal
//    data is privately allocated, returns false if the data
//    is allocated externally.
//

int EnumerationData::allocatedQ(void) const {
   function_information("EnumerationData::allocatedQ")

   if (allocationType == ENUM_FIXED_ALLOC) {
      return 0;
   } else if (allocationType == ENUM_TRANSIENT_ALLOC) {
      return 1;
   } else {
      cerr << "Error: unknown allocation type: " << allocationType << endl;
      exit(1);
   }
}



//////////////////////////////
//
// EnumerationData::compare
//

int EnumerationData::compare(int aValue) const {
   function_information("EnumerationData::compare(int)")

   if (getValue() < aValue) {
      return -1;
   } else if (getValue() > aValue) {
      return +1;
   } else {
      return 0;
   }
}


int EnumerationData::compare(const char* aName) const {
   function_information("EnumerationData::compare(const char*)")

   if (aName == NULL) {
      cerr << "Error: there is no string in the enumaration datum" << endl;
      exit(1);
   }
   return strcmp(name, aName);
}


int EnumerationData::compare(const EnumerationData& aDatum) const {
   function_information("EnumerationData::compare(const EnumerationData&)")

   int comparison = compare(aDatum.getValue());
   if (comparison == 0) {
      // break tie with comparison of name
      return compare(aDatum.getName());
   } else {
      return comparison;
   }
}



//////////////////////////////
//
// EnumerationData::compareByName
//

int EnumerationData::compareByName(const EnumerationData& aDatum) const {
   function_information("EnumerationData::compareByName")

   return compare(aDatum.getName());
}



//////////////////////////////
//
// EnumerationData::compareByValue
//

int EnumerationData::compareByValue(const EnumerationData& aDatum) const {
   function_information("EnumerationData::compareByValue")

   return compare(aDatum.getValue());
}



//////////////////////////////
//
// EnumerationData::getName
//

const char* EnumerationData::getName(void) const {
   function_information("EnumerationData::getName")

   return name;
}



//////////////////////////////
//
// EnumerationData::getValue
//

int EnumerationData::getValue(void) const {
   function_information("EnumerationData::getValue")

   return value;
}



//////////////////////////////
//
// EnumerationData::operator= 
//

EnumerationData& EnumerationData::operator=(const EnumerationData& aDatum) {
   function_information("EnumerationData::operator=")

   // don't copy onto self
   if (&aDatum == this) {
      return *this;
   }

   value = aDatum.value;
   if (allocatedQ() && name != NULL) {
      delete [] name;
      name = NULL;
   }

   allocationType = aDatum.allocationType;
   if (allocatedQ()) {
      name = new char[strlen(aDatum.name)+1];
      strcpy(name, aDatum.name);
   } else {
      name = aDatum.name;
   }

   return *this;
}



//////////////////////////////
//
// EnumerationData::operator==
//

int EnumerationData::operator==(const EnumerationData& aDatum) {
   function_information("EnumerationData::operator==")

   if (compare(aDatum) == 0) {
      return 1;
   } else { 
      return 0;
   }
}



//////////////////////////////
//
// EnumerationData::operator<
//

int EnumerationData::operator<(const EnumerationData& aDatum) {
   function_information("EnumerationData::operator<")

   if (compare(aDatum) == -1) {
      return 1;
   } else { 
      return 0;
   }
}



//////////////////////////////
//
// EnumerationData::operator>
//

int EnumerationData::operator>(const EnumerationData& aDatum) {
   function_information("EnumerationData::operator>")

   if (compare(aDatum) == 1) {
      return 1;
   } else { 
      return 0;
   }
}



//////////////////////////////
//
// EnumerationData::setRelation
//	default value: allocType = ENUM_TRANSIENT_ALLOC
//

void EnumerationData::setRelation(int aValue, const char* aName, int allocType) {
   function_information("EnumerationData::setRelation")

   value = aValue;
   if (allocatedQ()) {
      delete [] name;
      name = NULL;
   }
   allocationType = allocType;
   if (allocatedQ()) {
      name = new char[strlen(aName)+1];
      strcpy(name, aName);
   } else {
      name = (char*)aName;
   }
}



//////////////////////////////
//
// EnumerationData::setName
//	default value: allocType = ENUM_TRANSIENT_DATA
//

void EnumerationData::setName(const char* aName, int allocType) {
   function_information("EnumerationData::setName")

   if (allocatedQ() && name != NULL) {
      delete [] name;
      name = NULL;
   }
   allocationType = allocType;
   if (allocatedQ()) {
      name = new char[strlen(aName)+1];
      strcpy(name, aName);
   } else {
      name = (char*)aName;
   }
}



//////////////////////////////
//
// EnumerationData::setValue
//

void EnumerationData::setValue(int aValue) {
   function_information("EnumerationData::setValue")

   value = aValue;
}



///////////////////////////////////////////////////////////////////////////
//
// friendly functions
//



////////////////////
//
// operator<<
//

ostream& operator<<(ostream& out, const EnumerationData& aDatum) {
   function_information("operator<<(ostream&, const EnumerationData&)")

   // don't print the null enumeration
   if (aDatum.getValue() != E_unknown) {
      out << aDatum.getValue() << ":\t" << aDatum.getName();
   }
   return out;
}


ostream& operator<<(ostream& out, const EnumerationData* aDatum) {
   function_information("operator<<(ostream&, const EnumerationData*)")

   // don't print the null enumeration
   if (aDatum->getValue() != E_unknown) {
      out << aDatum->getValue() << ":\t" << aDatum->getName();
   }
   return out;
}


// md5sum:	0e9c17976c50efb30bbd7fb3535927a0  EnumerationData.cpp
