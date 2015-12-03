//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Jun  8 21:45:27 PDT 1998
// Last Modified: Tue Jun 23 14:06:21 PDT 1998
// Filename:      ...humdrum++/src/Enumeration.cpp
// Syntax:        C++ 
// $Smake:        cc -O3 -g -c -Wall -I../include %f && rm %b.o
//

#include "Enumeration.h"
#include "function_information.h"


//////////////////////////////
//
// Enumeration::Enumeration
//

Enumeration::Enumeration(void) {
   function_information("Enumeration::Enumeration(void)")

   associations.allowGrowth();
   associations.setSize(0);
   sortByValue.allowGrowth();
   sortByValue.setSize(0);
   sortByName.allowGrowth();
   sortByName.setSize(0);

   EnumerationData aDatum(E_unknown, "", ENUM_FIXED_ALLOC);
   associations.append(aDatum);
   EnumerationData* pointer = &associations[0];
   sortByValue.append(pointer);
   sortByName.append(pointer);

   sortQ = 1;
}


Enumeration::Enumeration(const Enumeration& aSet) {
   function_information("Enumeration::Enumeration(const Enumeration&)")

   int size = aSet.associations.getSize();
   associations.allowGrowth();
   associations.setSize(size);
   sortByValue.allowGrowth();
   sortByValue.setSize(size);
   sortByName.allowGrowth();
   sortByName.setSize(size);
   sortQ = aSet.sortQ;

   for (int i=0; i<size; i++) {
      associations[i] = aSet.associations[i];
      sortByValue[i] = aSet.sortByValue[i];
      sortByName[i] = aSet.sortByName[i];
   }
}



//////////////////////////////
//
// Enumeration::~Enumeration
//

Enumeration::~Enumeration() {
   function_information("Enumeration::~Enumeration")

   // nothing
}



//////////////////////////////
//
// Enumeration::add
//	default value: allocType = ENUM_TRANSIENT_ALLOC
//

void Enumeration::add(int aValue, const char* aName, int allocType) {
   function_information("Enumeration::add(int, const char*, int)")

   EnumerationData aDatum(aValue, aName, allocType);
   add(aDatum);
}


void Enumeration::add(const EnumerationData& aDatum) {
   function_information("Enumeration::add(const EnumerationData&)")

   associations[associations.getSize()] = aDatum;
   sortByValue[sortByValue.getSize()] = NULL;
   sortByName[sortByName.getSize()] = NULL;
   sortQ = 0;
}



//////////////////////////////
//
// Enumeration::associate
//	default value: allocType = ENUM_TRANSIENT_ALLOC
//

int Enumeration::associate(const char* aName, int allocType) {
   function_information("Enumeration::associate")

   int associationValue = getFreeValue();
   add(associationValue, aName, allocType);
   return associationValue;
}



//////////////////////////////
//
// Enumeration::getAssociation
//

const char* Enumeration::getAssociation(int aValue) {
   function_information("Enumeration::getAssociation(int)")

   return getName(aValue);
}


int Enumeration::getAssociation(const char* aName) {
   function_information("Enumeration::getAssociation(const char*)")

   return getValue(aName);
}



//////////////////////////////
//
// Enumeration::getName
//

const char* Enumeration::getName(int aValue) {
   function_information("Enumeration::getName")

   checksort();
   EnumerationData aDatum(aValue, "", ENUM_FIXED_ALLOC);
   EnumerationData* pointer = &aDatum;
   void* searchResult;
   searchResult = bsearch(&pointer, sortByValue.getBase(), 
         sortByValue.getSize(), sizeof(EnumerationData*), 
         data_compare_by_value_only);

   if (searchResult == NULL) {
      return "";
   } else {
      return (*(EnumerationData**)searchResult)->getName();
   }
}



//////////////////////////////
//
// Enumeration::getValue
//

int Enumeration::getValue(const char* aName) {
   function_information("Enumeration::getValue")

   checksort();
   EnumerationData aDatum(E_unknown, aName, ENUM_FIXED_ALLOC);
   EnumerationData* pointer = &aDatum;
   void* searchResult;
   searchResult = bsearch(&pointer, sortByName.getBase(), sortByName.getSize(),
         sizeof(EnumerationData*), data_compare_by_name_only);

   if (searchResult == NULL) {
      return E_unknown;
   } else {
      return (*(EnumerationData**)searchResult)->getValue();
   }

   return E_unknown;         // for dumb compilers
}



//////////////////////////////
//
// Enumeration::memberQ
//

int Enumeration::memberQ(int aValue) {
   function_information("Enumeration::memberQ(int)")

   checksort();
   EnumerationData aDatum(aValue, "", ENUM_FIXED_ALLOC);
   EnumerationData* pointer = &aDatum;
   void* searchResult;
   searchResult = bsearch(&pointer, sortByValue.getBase(), 
         sortByValue.getSize(), sizeof(EnumerationData*), 
         data_compare_by_value_only);

   if (searchResult == NULL) {
      return 0;
   } else {
      return 1;
   }
}


int Enumeration::memberQ(const char* aName) {
   function_information("Enumeration::memberQ(const char*)")

   checksort();
   EnumerationData aDatum(E_unknown, aName, ENUM_FIXED_ALLOC);
   EnumerationData* pointer = &aDatum;
   void* searchResult;
     searchResult = bsearch(&pointer, sortByName.getBase(), 
           sortByName.getSize(), sizeof(EnumerationData*), 
           data_compare_by_name_only);

   if (searchResult == NULL) {
      return 0;
   } else {
      return 1;
   }
}


int Enumeration::memberQ(const EnumerationData& aDatum) {
   function_information("Enumeration::memberQ(const EnumerationData&)")

   checksort();
   void* searchResult;
   const EnumerationData* pointer = &aDatum;
   searchResult = bsearch(&pointer, sortByValue.getBase(), 
         sortByValue.getSize(), sizeof(EnumerationData*), 
         data_compare);

   if (searchResult == NULL) {
      return 0;
   } else {
      return 1;
   }
}
   


//////////////////////////////
//
// Enumeration::getFreeValue
//

int Enumeration::getFreeValue(void) {
   function_information("Enumeration::getFreeValue")

   checksort();
   return sortByValue[sortByValue.getSize()-1]->getValue() + 1;
}



//////////////////////////////
//
// Enumeration::print
//
   
void Enumeration::print(ostream& out) {
   function_information("Enumeration::print")

   // don't print the null enumeration
   for (int i=1; i<associations.getSize(); i++) {
      out << associations[i] << '\n';
   }
}



//////////////////////////////
//
// Enumeration::printByName
//
   
void Enumeration::printByName(ostream& out) {
   function_information("Enumeration::printByName")

   checksort();
   // don't print the null enumeration
   for (int i=0; i<sortByName.getSize(); i++) {
      if (sortByName[i]->getValue() != E_unknown) {
         out << sortByName[i] << '\n';
      }
   }
}



//////////////////////////////
//
// Enumeration::printByValue
//
   
void Enumeration::printByValue(ostream& out) {
   function_information("Enumeration::printByValue")

   checksort();
   // don't print the null enumeration
   for (int i=0; i<sortByValue.getSize(); i++) {
      if (sortByValue[i]->getValue() != E_unknown) {
         out << sortByValue[i] << '\n';
      }
   }
}



//////////////////////////////
//
// Enumeration::setNullName
//	default value: allocType = ENUM_TRANSIENT_ALLOC
//
   
void Enumeration::setNullName(const char* aName, int allocType) {
   function_information("Enumeration::setNullName")

   associations[0].setName(aName, allocType);
}



////////////////////////////////////////////////////////////////////////////
//
// protected functions
//


//////////////////////////////
//
// Enumeration::checksort
//

void Enumeration::checksort(void) {
   function_information("Enumeration::checksort")

   if (!sortQ) {
      sort();
   }
}



//////////////////////////////
//
// Enumeration::sort
//

void Enumeration::sort(void) {
   function_information("Enumeration::sort")

   EnumerationData* pointer;
   for (int i=0; i<associations.getSize(); i++) {
      pointer = &associations[i];
      sortByValue[i] = pointer;
      sortByName[i] = pointer;
   }

   qsort(sortByValue.getBase(), sortByValue.getSize(), sizeof(EnumerationData*),
         data_compare_by_value);

   qsort(sortByName.getBase(), sortByName.getSize(), sizeof(EnumerationData*),
         data_compare_by_name);

   sortQ = 1;
}



////////////////////////////////////////////////////////////////////////////
//
// helping functions
//


//////////////////////////////
//
// Enumeration::data_compare
//

int Enumeration::data_compare(const void* a, const void* b) {
   function_information("Enumeration::data_compare")

   EnumerationData& valuea = *((EnumerationData*)a);
   EnumerationData& valueb = *((EnumerationData*)b);
   return valuea.compare(valueb);
}


//////////////////////////////
//
// Enumeration::data_compare_by_value
//

int Enumeration::data_compare_by_value(const void* a, 
      const void* b) {
   function_information("Enumeration::data_compare_by_value")

   EnumerationData& valuea = **((EnumerationData**)a);
   EnumerationData& valueb = **((EnumerationData**)b);
   return valuea.compareByValue(valueb);
}



//////////////////////////////
//
// Enumeration::data_compare_by_name
//

int Enumeration::data_compare_by_name(const void* a, 
      const void* b) {
   function_information("Enumeration::data_compare_by_name")

   EnumerationData& valuea = **((EnumerationData**)a);
   EnumerationData& valueb = **((EnumerationData**)b);
   return valuea.compareByName(valueb);
}



//////////////////////////////
//
// Enumeration::data_compare_by_value_only
//

int Enumeration::data_compare_by_value_only(const void* a, 
      const void* b){
   function_information("Enumeration::data_compare_by_value_only")

   EnumerationData& valuea = **((EnumerationData**)a);
   EnumerationData& valueb = **((EnumerationData**)b);
   return valuea.compare(valueb.getValue());
}



//////////////////////////////
//
// Enumeration::data_compare_by_name_only
//

int Enumeration::data_compare_by_name_only(const void* a, 
      const void* b) {
   function_information("Enumeration::data_compare_by_name_only")

   EnumerationData& valuea = **((EnumerationData**)a);
   EnumerationData& valueb = **((EnumerationData**)b);
   return valuea.compare(valueb.getName());
}




// md5sum:	40f947fb223f27b20478971bf7b32ece  Enumeration.cpp
