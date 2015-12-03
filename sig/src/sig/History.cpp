//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Oct  9 10:53:57 GMT-0800 1997
// Last Modified: Thu Oct  9 10:54:01 GMT-0800 1997
// Filename:      ...sig/code/base/History/History.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _HISTORY_CC_INCLUDED
#define _HISTORY_CC_INCLUDED


#include "History.h"
#include <iostream.h>


//////////////////////////////
//
// History::History
//

template<class type>
History<type>::History(int aSize) {
   if (aSize <= 0) {
      cerr << "Error: cannot have a non-positive history size: " 
           << aSize << endl;
      exit(1);
   }
   storageSize = aSize;
   storage = new type[storageSize];
   writeIndex = 0;
}



//////////////////////////////
//
// History::~History
//

template<class type>
History<type>::~History() {
   if (storage != NULL) {
      delete [] storage;
   }
}



//////////////////////////////
//
// History::fillArray
//	default value: youngestIndex = 0;
//

template<class type>
void History<type>::fillArray(type* anArray, 
      long numElements, long youngestIndex) const {
   if (youngestIndex > 0) youngestIndex = -youngestIndex;
   for (int i=0; i<numElements; i++) {
      anArray[i] = history(youngestIndex-numElements+i+1);
   }
}



//////////////////////////////
//
// History::getSize
//

template<class type>
long History<type>::getSize(void) const {
   return storageSize;
}



//////////////////////////////
//
// History::initialize
//

template<class type>
void History<type>::initialize(type aValue) {
   for (int i=0; i<getSize(); i++) {
      storage[i] = aValue;
   }
}



//////////////////////////////
//
// History::insert
//

template<class type>
void History<type>::insert(type anElement) {
   writeIndex++;
   if (writeIndex >= getSize()) {   // should never be greater than getSize()
      writeIndex = 0;
   }
   storage[writeIndex] = anElement;
}



//////////////////////////////
//
// History::operator[]
//

template<class type>
type& History<type>::operator[](long anIndex) {
   static long index;
   if (anIndex > 0) anIndex = -anIndex;      // anIndex always negative
   index =  writeIndex + anIndex;
   if (index < 0) index += getSize();        // index in range [0..getSize()-1]
   return storage[index];
}



//////////////////////////////
//
// History::setSize
//

template<class type>
void History<type>::setSize(long aSize) {
   if (aSize <= 0) {
      cerr << "Error: cannot have a non-positive history size: " 
           << aSize << endl;
      exit(1);
   }
   delete [] storage;
   storage = new type[aSize];
   storageSize = aSize;
   writeIndex = 0;
}



//////////////////////////////
//
// History::zero
//

template<class type>
void History<type>::zero(void) {
   initialize(0);
}



///////////////////////////////////////////////////////////////////////////
//
// Private functions
//


//////////////////////////////
//
// History::history -- like operator[]
//

template<class type>
type History<type>::history(long anIndex) const {
   static long index;
   if (anIndex > 0) anIndex = -anIndex;      // anIndex always negative
   index =  writeIndex + anIndex;
   if (index < 0) index += getSize();        // index in range [0..getSize()-1]
   return storage[index];
}



#endif  // _HISTORY_CC_INCLUDED



// md5sum:	6eeaa6b558b900967cb454e36db68bbb  History.cpp
