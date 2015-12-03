//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Apr 21 21:18:35 GMT-0800 1997
// Last Modified: Thu Sep 11 04:17:44 GMT-0800 1997
// Filename:      ...sig/code/base/Vector/Vector.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _VECTOR_H_INCLUDED
#define _VECTOR_H_INCLUDED


#include "ComplexD.h"
#include <iostream.h>

template<class type>
class Vector {
   public:
                      Vector           (void);
                      Vector           (int aDimension);
                      Vector           (type xx, type yy);
                      Vector           (type xx, type yy, type zz);
                      Vector           (const Vector<type>& aVector);
                     ~Vector           ();
      int             getDimension     (void) const;
      double          norm             (void);
      type&           operator[]       (int index);
      type            operator[]       (int index) const;
      Vector<type>&   operator=        (const Vector<type>& aVector);
      Vector<type>&   operator+=       (const Vector<type>& aVector);
      Vector<type>    operator+        (const Vector<type>& aVector) const;
      Vector<type>&   operator-=       (const Vector<type>& aVector);
      Vector<type>    operator-        (const Vector<type>& aVector) const;
      Vector<type>    operator-        (void) const;
      Vector<type>&   operator*=       (const Vector<type>& aVector);
      Vector<type>&   operator*=       (type aNumber);
      Vector<type>    operator*        (const Vector<type>& aVector) const;
      Vector<type>    operator*        (type aNumber) const;
      Vector<type>&   operator/=       (const Vector<type>& aVector);
      Vector<type>&   operator/=       (type aNumber);
      Vector<type>    operator/        (const Vector<type>& aVector) const;
      Vector<type>    operator/        (type aNumber) const;
      int             operator==       (Vector<type>& aVector);
      void            print            (void);
      void            setDimension     (int aDimension);
      type&           x                (void);
      type&           y                (void);
      type&           z                (void);
      void            zero             (type aValue = 0);

   protected:
      int dimension;
      type* points;

};

template<class type>
ostream& operator<<(ostream& output, const Vector<type>& aVector);

template<class type>
Vector<type> operator*(type aNumber, Vector<type>& aVector);

#include "Vector.cpp"



///////////////////////////////////////////////////////////////////////////
//
// Vector manipulation functions
//

template<class type>
type normSquare(const Vector<type>& aVector);

template<class type>
double norm(const Vector<type>& aVector);

/*
ComplexD ip(const Vector<ComplexD>& vector1, const Vector<ComplexD>& vector2);
*/

template<class type>
type ip(const Vector<type>& vector1, const Vector<type>& vector2);


template<class type>
Vector<type> midpoint(const Vector<type>& a, const Vector<type>& b);



#endif  // _VECTOR_H_INCLUDED



// md5sum:	f3d029137dd7d9c72d723d1a891461d9  Vector.h
