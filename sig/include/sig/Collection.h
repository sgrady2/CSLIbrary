//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 19:42:53 PST 1997
// Last Modified: Tue Apr 22 20:28:16 GMT-0800 1997
// Filename:      ...sig/code/base/Collection/Collection.h
// Web Address:   http://www-ccrma.stanford.edu/~craig/improv/include/Collection.h
// Syntax:        C++ 
//
// Description:   A dynamic array which can grow as necessary.
//                This class can hold any type of item, but the
//                derived Array class is specifically for collections
//                of numbers.
//

#ifndef _COLLECTION_H_INCLUDED
#define _COLLECTION_H_INCLUDED


template<class type>
class Collection {
   public:
                Collection        (void);
                Collection        (int arraySize);
                Collection        (int arraySize, type *aCollection);
                Collection        (Collection<type>& aCollection);
               ~Collection        ();

      void      allowGrowth       (int status = 1);
      void      append            (type& element);
//    void      append            (type element);
      void      append            (type* element);
      type     *getBase           (void);
      long      getAllocSize      (void) const;
      long      getSize           (void) const;
      type     *pointer           (void);
      void      setAllocSize      (long aSize);
      void      setGrowth         (long growth);
      void      setSize           (long newSize);
      type&     operator[]        (int arrayIndex);
      type      operator[]        (int arrayIndex) const;
      void      grow              (long growamt = -1);


   protected:
      long      size;             // actual array size
      long      allocSize;        // maximum allowable array size
      type     *array;            // where the array data is stored
      char      allowGrowthQ;     // allow/disallow growth
      long      growthAmount;     // number of elements to grow by if index
				  //    element one beyond max size is accessed
      long maxSize;               // the largest size the array is allowed 
                                  //    to grow to, if 0, then ignore max
  
      void      shrinkTo          (long aSize);
};


#include "Collection.cpp"



#endif  // _COLLECTION_H_INCLUDED



// md5sum:        9338aad092c485a6ca0421bb81b0252c  Collection.h
