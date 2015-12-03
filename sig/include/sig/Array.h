//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 19:42:53 PST 1997
// Last Modified: Sun May 11 20:33:13 GMT-0800 1997
// Last Modified: Wed Jul  7 11:44:50 PDT 1999 (added setAll() function)
// Filename:      .../sig/code/base/Array/Array.h
// Web Address:   http://www-ccrma.stanford.edu/~craig/improv/include/Array.h
// Syntax:        C++ 
//
// Description:   An array which can grow dynamically.  Array is derived from 
//                the Collection class and adds various mathematical operators
//                to the Collection class.  The Array template class is used for
//                storing numbers of any type which can be added, multiplied
//                and divided into one another.
//

#ifndef _ARRAY_H_INCLUDED
#define _ARRAY_H_INCLUDED

#include "Collection.h"


template<class type>
class Array : public Collection<type> {
   public:
                     Array             (void);
                     Array             (int arraySize);
                     Array             (Array<type>& aArray);
                     Array             (int arraySize, type *anArray);
                    ~Array             ();

      void           setAll            (type aValue);
      type           sum               (void);
      type           sum               (int lowIndex, int hiIndex);
      void           zero              (int minIndex = -1, int maxIndex = -1);

      Array<type>&   operator=         (const Array<type>& aArray);
      Array<type>&   operator+=        (const Array<type>& aArray);
      Array<type>&   operator-=        (const Array<type>& aArray);
      Array<type>&   operator*=        (const Array<type>& aArray);
      Array<type>&   operator/=        (const Array<type>& aArray);

      Array<type>    operator+         (const Array<type>& aArray) const;
      Array<type>    operator+         (type aNumber) const;
      Array<type>    operator-         (const Array<type>& aArray) const;
      Array<type>    operator-         (void) const;

      Array<type>    operator-         (type aNumber) const;
      Array<type>    operator*         (const Array<type>& aArray) const;
      Array<type>    operator*         (type aNumber) const;
      Array<type>    operator/         (const Array<type>& aArray) const;
};


#include "Array.cpp"   /* necessary for templates */


#endif  // _ARRAY_H_INCLUDED


// md5sum:        f38d82e1fde6a4cd08136afaaf337843  Array.h
