//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 19:42:53 PST 1997
// Last Modified: Tue Feb 11 23:19:31 PST 1997
// Last Modified: Fri May 28 15:15:47 PDT 1999 (zero() off-by one fixed)
// Last Modified: Sun Jun 11 13:13:49 PDT 2000 (added setSize() function)
// Filename:      ...sig/code/base/Block/Block.cc
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _BLOCK_CPP_INCLUDED
#define _BLOCK_CPP_INCLUDED


#include "Block.h"
#include <iostream.h>
#include <stdlib.h>


//////////////////////////////
//
// Block::Block --
//

template<class type>
Block<type>::Block(void) {
   size = 0;
   array = NULL;
}

template<class type>
Block<type>::Block(int blockSize) {
   array = new type[blockSize];
   size = blockSize;
}

template<class type>
Block<type>::Block(Block<type>& aBlock) {
   size = aBlock.size;
   array = new type[size];
   memcpy(array, aBlock.array, sizeof(type)*size);
}

template<class type>
Block<type>::Block(const type* anArray, int arraySize) {
   size = arraySize;
   array = new type[size];
   memcpy(array, anArray, sizeof(type)*size);
}



//////////////////////////////
//
// Block::~Block --
//

template<class type>
Block<type>::~Block() {
   if (size != 0) {
      delete [] array;
   }
}



//////////////////////////////
//
// Block::getSize --
//

template<class type>
int Block<type>::getSize(void) const {
   return size;
}



//////////////////////////////
//
// Block::zero --
//

template<class type>
void Block<type>::zero(int minIndex, int maxIndex) {
   if (minIndex == -1) minIndex = 0;
   if (maxIndex == -1) maxIndex = getSize()-1;

   if (minIndex < 0 || maxIndex < 0 || minIndex > maxIndex || 
       maxIndex >= getSize()) {
      cerr << "Error in Zero function: min = " << minIndex 
           << " max = " << maxIndex << " size = " << getSize() << endl;
      exit(1);
   }

   for (int i=minIndex; i<=maxIndex; i++) {
      array[i] = 0;
   }
}



//////////////////////////////
//
// Block::operator[] --
//

template<class type>
type& Block<type>::operator[](int elementIndex) {
   if (elementIndex >= getSize()) {
      cerr << "Error: accessing invalid array location " 
           << elementIndex 
           << " Maximum is " << getSize()-1 << endl;
      exit(1);
   }
   return array[elementIndex];
}

template<class type>
type Block<type>::operator[](int elementIndex) const {
   if (elementIndex >= getSize()) {
      cerr << "Error: accessing invalid array location " 
           << elementIndex 
           << " Maximum is " << getSize()-1 << endl;
      exit(1);
   }
   return array[elementIndex];
}



//////////////////////////////
//
// Block::operator= --
//

template<class type>
Block<type>& Block<type>::operator=(const Block<type>& aBlock) {
   if (size != aBlock.size) {
      if (size != 0) {
         delete [] array;
      }
      size = aBlock.size;
      array = new type[size];
   }
   memcpy(array, aBlock.array, sizeof(type)*size);

   return *this;
}



//////////////////////////////
//
// Block::operator+= --
//

template<class type>
Block<type>& Block<type>::operator+=(const Block<type>& aBlock) {
   if (size != aBlock.size) {
      cerr << "Error: different size blocks " << size << " and " 
           << aBlock.size << endl;
      exit(1);
   }

   for (int i=0; i<size; i++) {
      array[i] += aBlock.array[i];
   }

   return *this;
}



//////////////////////////////
//
// Block::operator+ --
//

template<class type>
Block<type> Block<type>::operator+(const Block<type>& aBlock) const {
   if (size != aBlock.size) {
      cerr << "Error: different size blocks " << size << " and " 
           << aBlock.size << endl;
      exit(1);
   }

   Block<type> bBlock(*this);
   bBlock += aBlock;
   return bBlock;
}


template<class type>
Block<type> Block<type>::operator+(type aNumber) const {
   Block<type> aBlock(*this);
   for (int i=0; i<size; i++) {
      aBlock[i] += aNumber;
   }
   return aBlock;
}



//////////////////////////////
//
// Block::operator-= --
//

template<class type>
Block<type>& Block<type>::operator-=(const Block<type>& aBlock) {
   if (size != aBlock.size) {
      cerr << "Error: different size blocks " << size << " and " 
           << aBlock.size << endl;
      exit(1);
   }

   for (int i=0; i<size; i++) {
      array[i] -= aBlock.array[i];
   }

   return *this;
}



//////////////////////////////
//
// Block::operator- --
//

template<class type>
Block<type> Block<type>::operator-(const Block<type>& aBlock) const {
   if (size != aBlock.size) {
      cerr << "Error: different size blocks " << size << " and " 
           << aBlock.size << endl;
      exit(1);
   }

   Block<type> bBlock(*this);
   bBlock -= aBlock;
   return bBlock;
}

// unary operator
template<class type>
Block<type> Block<type>::operator-(void) const {
   Block<type> aBlock(*this);
   for (int i=0; i<size; i++) {
      aBlock[i] = -aBlock[i];
   }
   return aBlock;
}

template<class type>
Block<type> Block<type>::operator-(type aNumber) const {
   Block<type> aBlock(*this);
   for (int i=0; i<size; i++) {
      aBlock[i] -= aNumber;
   }
   return aBlock;
}



//////////////////////////////
//
// Block::operator*= --
//

template<class type>
Block<type>& Block<type>::operator*=(const Block<type>& aBlock) {
   if (size != aBlock.size) {
      cerr << "Error: different size blocks " << size << " and " 
           << aBlock.size << endl;
      exit(1);
   }

   for (int i=0; i<size; i++) {
      array[i] *= aBlock.array[i];
   }

   return *this;
}



//////////////////////////////
//
// Block::operator* --
//

template<class type>
Block<type> Block<type>::operator*(const Block<type>& aBlock) const {
   if (size != aBlock.size) {
      cerr << "Error: different size blocks " << size << " and " 
           << aBlock.size << endl;
      exit(1);
   }

   Block<type> bBlock(*this);
   bBlock *= aBlock;
   return bBlock;
}


template<class type>
Block<type> Block<type>::operator*(type aNumber) const {
   Block<type> aBlock(*this);
   for (int i=0; i<size; i++) {
      aBlock[i] *= aNumber;
   }
   return aBlock;
}



//////////////////////////////
//
// Block::operator/= --
//

template<class type>
Block<type>& Block<type>::operator/=(const Block<type>& aBlock) {
   if (size != aBlock.size) {
      cerr << "Error: different size blocks " << size << " and " 
           << aBlock.size << endl;
      exit(1);
   }

   for (int i=0; i<size; i++) {
      array[i] /= aBlock.array[i];
   }

   return *this;
}



//////////////////////////////
//
// Block::operator/ --
//

template<class type>
Block<type> Block<type>::operator/(const Block<type>& aBlock) const {
   if (size != aBlock.size) {
      cerr << "Error: different size blocks " << size << " and " 
           << aBlock.size << endl;
      exit(1);
   }

   Block<type> bBlock(*this);
   bBlock /= aBlock;
   return bBlock;
}



//////////////////////////////
//
// Block::setSize --
//

template<class type>
void Block<type>::setSize(int aSize) {
   if (aSize < 0) {
      cout << "Error: Cannot have a negative block size." << endl;
   }

   if (array != NULL) {
      delete [] array;
   }
   size = aSize;
   if (size == 0) {
      array = NULL;
      return;
   }

   array = new type[aSize];
}


#endif  // _BLOCK_CPP_INCLUDED



// md5sum:	bbf10866c6753ec9b75f686e70c9cd87  Block.cpp
