//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 19:42:53 PST 1997
// Last Modified: Wed Dec 17 23:54:07 GMT-0800 1997
// Last Modified: Sun Jun 11 13:13:49 PDT 2000 (added setSize() function)
// Filename:      ...sig/code/base/Block/Block.h
// Syntax:        C++ 
//
// Description:   An array for signal processing.
//

#ifndef _BLOCK_H_INCLUDED
#define _BLOCK_H_INCLUDED


template<class type>
class Block {
   public:
                     Block            (void);
                     Block            (int blockSize);
                     Block            (Block<type>& aBlock);
                     Block            (const type* anArray, int arraySize);
                    ~Block            ();

      int            getSize          (void) const;
      void           zero             (int minIndex = -1, int maxIndex = -1);

      type&          operator[]       (int arrayIndex);
      type           operator[]       (int arrayIndex) const;
      Block<type>&   operator=        (const Block<type>& aBlock);
      Block<type>&   operator+=       (const Block<type>& aBlock);
      Block<type>&   operator-=       (const Block<type>& aBlock);
      Block<type>&   operator*=       (const Block<type>& aBlock);
      Block<type>&   operator/=       (const Block<type>& aBlock);

      Block<type>    operator+        (const Block<type>& aBlock) const;
      Block<type>    operator+        (type aNumber) const;
      Block<type>    operator-        (const Block<type>& aBlock) const;
      Block<type>    operator-        (void) const;
      Block<type>    operator-        (type aNumber) const;
      Block<type>    operator*        (const Block<type>& aBlock) const;
      Block<type>    operator*        (type aNumber) const;
      Block<type>    operator/        (const Block<type>& aBlock) const;
 
      void           setSize          (int aSize);

   protected:
      int            size;
      type          *array;

};



#endif  // _BLOCK_H_INCLUDED



// md5sum:	ea44d77f198954b66793af593ecd4f09  Block.h
