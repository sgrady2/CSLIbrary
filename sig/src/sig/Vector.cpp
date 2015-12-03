//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Apr 21 21:18:35 GMT-0800 1997
// Last Modified: Mon Apr 21 21:18:35 GMT-0800 1997
// Filename:      ...sig/code/base/Vector/Vector.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _VECTOR_CPP_INCLUDED
#define _VECTOR_CPP_INCLUDED


#include "Vector.h"
#include <stdlib.h>
#include <iostream.h>


//////////////////////////////
//
// Vector::Vector
//

template<class type>
Vector<type>::Vector(void) {
   dimension = 0;
   points = NULL;
}


template<class type>
Vector<type>::Vector(int aDimension) {
   dimension = aDimension;
   points = new type[dimension];
}


template<class type>
Vector<type>::Vector(type xx, type yy) {
   dimension = 2;
   points = new type[dimension];
   points[0] = xx;
   points[1] = yy;
}


template<class type>
Vector<type>::Vector(type xx, type yy, type zz) {
   dimension = 3;
   points = new type[dimension];
   points[0] = xx;
   points[1] = yy;
   points[2] = zz;
}


template<class type>
Vector<type>::Vector(const Vector<type>& aVector) {
   dimension = aVector.getDimension();
   points = new type[dimension];
   for (int i=0; i<dimension; i++) {
      points[i] = aVector.points[i];
   }
}



//////////////////////////////
//
// Vector::~Vector
//

template<class type>
Vector<type>::~Vector() {
   if (dimension != 0) 
      delete [] points;
}



//////////////////////////////
//
// Vector::getDimension
//

template<class type>
int Vector<type>::getDimension(void) const {
   return dimension;
}



//////////////////////////////
//
// Vector::norm -- returns the length of a Vector
//

template<class type>
double Vector<type>::norm(void) {
   double output = 0;
   for (int i=0; i<getDimension(); i++) {
      output += points[i] * points[i];
   }
   if (output < 0) return sqrt(-output);
   else return sqrt(output);
}



//////////////////////////////
//
// Vector::operator[]
//

template<class type>
type& Vector<type>::operator[](int index) {
   if (index < 0 || index >= getDimension()) {
      cerr << "Error: invalid array reference: " << index 
           << " range is 0 to " << getDimension() << endl;
      exit(1);
   }

   return points[index];
}



//////////////////////////////
//
// Vector::operator[] const
//

template<class type>
type Vector<type>::operator[](int index) const {
   if (index < 0 || index >= getDimension()) {
      cerr << "Error: invalid array reference: " << index 
           << " range is 0 to " << getDimension() << endl;
      exit(1);
   }

   return points[index];
}



//////////////////////////////
//
// Vector::operator=
//

template<class type>
Vector<type>& Vector<type>::operator=(const Vector<type>& aVector) {
   if (&aVector == this) return *this;        // Assigning to self
   
   if (dimension == aVector.getDimension()) {
      // do nothing to storage size
   } else if (dimension != 0) { 
      delete [] points;
      points = new type[aVector.getDimension()];
      dimension = aVector.getDimension();
   }

   for (int i=0; i<dimension; i++) {
      points[i] = aVector.points[i];
   }

   return *this;
}



//////////////////////////////
//
// Vector::operator+=
//

template<class type>
Vector<type>& Vector<type>::operator+=(const Vector<type>& aVector) {
   if (getDimension() != aVector.getDimension()) {
      cerr << "Error: different size vectors cannot be added." << endl;
      exit(1);
   }

   for (int i=0; i<getDimension(); i++) {
      points[i] += aVector.points[i];
   }

   return *this;
}



//////////////////////////////
//
// Vector::operator+
//

template<class type>
Vector<type> Vector<type>::operator+(const Vector<type>& aVector) const {
   if (getDimension() != aVector.getDimension()) {
      cerr << "Error: different size vectors cannot be added." << endl;
      exit(1);
   }

   Vector<type> output(*this);
   output += aVector;
   return output;
}



//////////////////////////////
//
// Vector::operator-=
//

template<class type>
Vector<type>& Vector<type>::operator-=(const Vector<type>& aVector) {
   if (getDimension() != aVector.getDimension()) {
      cerr << "Error: different size vectors cannot be added." << endl;
      exit(1);
   }

   for (int i=0; i<getDimension(); i++) {
      points[i] -= aVector.points[i];
   }

   return *this;
}



//////////////////////////////
//
// Vector::operator-
//

template<class type>
Vector<type> Vector<type>::operator-(const Vector<type>& aVector) const {
   if (getDimension() != aVector.getDimension()) {
      cerr << "Error: different size vectors cannot be added." << endl;
      exit(1);
   }

   Vector<type> output(*this);
   output -= aVector;
   return output;
}


// uniary minus sign
template<class type>
Vector<type> Vector<type>::operator-(void) const {
   Vector<type> output(*this);
   for (int i=0; i<getDimension(); i++) {
      output.points[i] = -output.points[i];
   }
   return output;
}



//////////////////////////////
//
// Vector::operator*= -- pointwise multiplication.
//

template<class type>
Vector<type>& Vector<type>::operator*=(const Vector<type>& aVector) {
   if (getDimension() != aVector.getDimension()) {
      cerr << "Error: different size vectors cannot be multiplied." << endl;
      exit(1);
   }

   for (int i=0; i<getDimension(); i++) {
      points[i] *= aVector.points[i];
   }
   return *this;
}


// multiplying by a scalar:
template<class type>
Vector<type>& Vector<type>::operator*=(type aNumber) {
   for (int i=0; i<getDimension(); i++) {
      points[i] *= aNumber;
   }
   return *this;
}


//////////////////////////////
//
// Vector::operator* -- pointwise multiplication
//

template<class type>
Vector<type> Vector<type>::operator*(const Vector<type>& aVector) const {
   if (getDimension() != aVector.getDimension()) {
      cerr << "Error: different size vectors cannot be multiplied." << endl;
      exit(1);
   }

   Vector<type> output(*this);
   output *= aVector;
   return output;
}


// multiplication by a scalar:
template<class type>
Vector<type> Vector<type>::operator*(type aNumber) const {
   Vector<type> output(*this);
   for (int i=0; i<output.getDimension(); i++) {
      output.points[i] *= aNumber;
   }
   return output;
}



//////////////////////////////
//
// Vector::operator/= -- pointwise division.
//

template<class type>
Vector<type>& Vector<type>::operator/=(const Vector<type>& aVector) {
   if (getDimension() != aVector.getDimension()) {
      cerr << "Error: different size vectors cannot be multiplied." << endl;
      exit(1);
   }

   for (int i=0; i<getDimension(); i++) {
      points[i] /= aVector.points[i];
   }
   return *this;
}


// division by a scalar:
template<class type>
Vector<type>& Vector<type>::operator/=(type aNumber) {
   for (int i=0; i<getDimension(); i++) {
      points[i] /= aNumber;
   }
   return *this;
}



//////////////////////////////
//
// Vector::operator/ -- pointwise division.
//

template<class type>
Vector<type> Vector<type>::operator/(const Vector<type>& aVector) const {
   if (getDimension() != aVector.getDimension()) {
      cerr << "Error: different size vectors cannot be multiplied." << endl;
      exit(1);
   }

   Vector<type> output(*this);
   output /= aVector;
   return output;
}


// division by a scalar:
template<class type>
Vector<type> Vector<type>::operator/(type aNumber) const {
   Vector<type> output(*this);
   output /= aNumber;
   return output;
}



//////////////////////////////
//
// Vector::operator==
//

template<class type>
int Vector<type>::operator==(Vector<type>& aVector) {
   if (getDimension() != aVector.getDimension()) {
      return 0;
   }
   for (int i=0; i<getDimension(); i++) {
      if (points[i] != aVector[i]) {
         return 0;
      }
   }
 
   return 1;
}



//////////////////////////////
//
// Vector::print
//

template<class type>
void Vector<type>::print(void) {
   cout << *this << endl;
}


//////////////////////////////
//
// Vector::setDimension
//

template<class type>
void Vector<type>::setDimension(int aDimension) {
   if (aDimension < 0) {
      cerr << "Invalid size for vector: " << aDimension << endl;
      exit(1);
   }
   int oldDimension = dimension;
   dimension = aDimension;

   type* temp;
   temp = new type[dimension];

   for (int i=0; i<dimension && i<oldDimension; i++) {
      temp[i] = points[i];
   }
   if (oldDimension != 0) {
      delete [] points;
   }
   points = temp;
}



//////////////////////////////
//
// Vector::x
//

template<class type>
type& Vector<type>::x(void) {
   return points[0];
}



//////////////////////////////
//
// Vector::y
//

template<class type>
type& Vector<type>::y(void) {
   return points[1];
}



//////////////////////////////
//
// Vector::z
//

template<class type>
type& Vector<type>::z(void) {
   return points[2];
}



//////////////////////////////
//
// Vector::zero
//	default value: aValue = 0
//

template<class type>
void Vector<type>::zero(type aValue) {
   for (int i=0; i<getDimension(); i++) {
      points[i] = aValue;
   }
}


///////////////////////////////////////////////////////////////////////////



//////////////////////////////
//
// operator*
//

template<class type>
Vector<type> operator*(type aNumber, Vector<type>& aVector) {
   Vector<type> output(aVector);
   output *= aNumber;
   return output;
}


///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// operator<<
//

template<class type>
ostream& operator<<(ostream& output, const Vector<type>& aVector) {
   output << "(";
   for (int i=0; i<aVector.getDimension()-1; i++) {
      output << aVector.points[i] << ", ";
   }
   output << aVector.points[aVector.getDimension()-1] << ")";
 
   return output;
}


///////////////////////////////////////////////////////////////////////////



//////////////////////////////
//
// norm -- returns the length of a Vector
//

template<class type>
double norm(const Vector<type>& aVector) {
   double output = 0;
   for (int i=0; i<aVector.getDimension(); i++) {
      output += aVector[i] * aVector[i];
   }
   if (output < 0) return sqrt(-output);
   else return sqrt(output);
}



//////////////////////////////
//
// normSquare -- returns the squared norm (length) of a Vector
//

template<class type>
type normSquare(const Vector<type>& aVector) {
   type output = 0;
   for (int i=0; i<aVector.getDimension(); i++) {
      output += aVector[i] * aVector[i];
   }
   if (output < 0) return -output;
   else return output;
}



//////////////////////////////
//
// ip -- returns the inner product of a Vector
//

/*
ComplexD ip(const Vector<ComplexD>& vector1, const Vector<ComplexD>& vector2) {
   if (vector1.getDimension() != vector2.getDimension()) {
      cerr << "Error: vectors must have same dimension to calculate"
           << " inner product." << endl;
      exit(1);
   }

   ComplexD output = 0;
   for (int i=0; i<vector1.getDimension(); i++) {
      output += vector1[i] * vector2[i].conjugate();
   }
   return output;
}
*/


template<class type>
type ip(const Vector<type>& vector1, const Vector<type>& vector2) {
   if (vector1.getDimension() != vector2.getDimension()) {
      cerr << "Error: vectors must have same dimension to calculate"
           << " inner product." << endl;
      exit(1);
   }

   type output = 0;
   for (int i=0; i<vector1.getDimension(); i++) {
      output += vector1[i] * vector2[i];
   }
   return output;
}



//////////////////////////////
//
// midpoint -- returns the midpoint between two vectors
//

template<class type>
Vector<type> midpoint(const Vector<type>& a, const Vector<type>& b) {
   return (b+a)/2;
}



#endif  // _VECTOR_CC_INCLUDED



// md5sum:	eb64ebe104287e2f9b5035755e9277cd  Vector.cpp
