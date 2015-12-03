//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Oct  9 10:42:38 GMT-0800 1997
// Last Modified: Thu Oct  9 10:42:43 GMT-0800 1997
// Filename:      ...sig/code/base/History/History.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _HISTORY_H_INCLUDED
#define _HISTORY_H_INCLUDED


template<class type>
class History {
   public:
                   History          (int aSize);
                  ~History          ();

      void         fillArray        (type* anArray, long numElements,
                                            long youngestIndex = 0) const;
      long         getSize          (void) const;
      void         initialize       (type aValue);
      void         insert           (type anElement);
      type&        operator[]       (long anIndex);
      void         setSize          (long aSize);
      void         zero             (void);

   protected:
      type*        storage;
      long         storageSize;
      long         writeIndex;

      type         history          (long anIndex) const;
};


#include "History.cpp"



#endif  // _HISTORY_H_INCLUDED



// md5sum:	14dd6c471f762f6a87575bffff2c6905  History.h
