//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Feb  6 23:56:35 PST 1997
// Last Modified: Sun Aug 31 16:29:08 GMT-0800 1997
// Filename:      ...sig/code/base/ComplexD/ComplexD.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#include "ComplexD.h"
#include <math.h>
#include <iostream.h>
#include <stdlib.h>


//////////////////////////////
//
// ComplexD::ComplexD
//

ComplexD::ComplexD(void) {
   // nothing: contents initially undefined
}

// default second parameter, bNumber, is 0 for imaginary part:
ComplexD::ComplexD(double aNumber, double bNumber) {
   re() = aNumber;
   im() = bNumber;
}

ComplexD::ComplexD(const ComplexD& aNumber) {
   re() = aNumber.number[0];
   im() = aNumber.number[1];
}



//////////////////////////////
//
// ComplexD::~ComplexD
//

ComplexD::~ComplexD() {
   // nothing
}



//////////////////////////////
//
// ComplexD::abs -- returns the absolute value of the complex number.
//

double ComplexD::abs(void) const {
   return sqrt(number[0]*number[0]+number[1]*number[1]);
}



//////////////////////////////
//
// ComplexD::arg -- returns the argument (angle) of the complex number.
//

double ComplexD::arg(void) const {
   return atan2(number[1], number[0]);
}



//////////////////////////////
//
// ComplexD::cexp -- returns the e to the *this, complex exponential
//	default value:   aPower = 1
//

ComplexD ComplexD::cexp(double aPower) const {
   double R = exp(aPower * number[0]);
   ComplexD output(R * cos(aPower * number[1]), R * sin(aPower * number[1]));
   return output;
}



//////////////////////////////
//
// ComplexD::conj
//

ComplexD ComplexD::conj(void) const {
   ComplexD output;
   output.re() = number[0];
   output.im() = -number[1];
   return output;
}



//////////////////////////////
//
// ComplexD::conjugate
//

ComplexD ComplexD::conjugate(void) const {
   ComplexD output;
   output.re() = number[0];
   output.im() = -number[1];
   return output;
}



//////////////////////////////
//
// ComplexD::im
//

double& ComplexD::im(void) {
   return number[1];
}



//////////////////////////////
//
// ComplexD::operator[]
//

double& ComplexD::operator[](int index) {
   if (index == 0) return re();
   else if (index == 1) return im();
   else {
      cerr << "Invalid index: " << index << "-- can only be 0=re, 1=im." << endl;
      exit(1);
   }
 
   // include the following line for stuipd Visual C++ 5.0 compiler.
   // this line should never be reached.
   return re();
}



//////////////////////////////
//
// ComplexD::operator[] const
//

double ComplexD::operator[](int index) const {
   if (index == 0) return number[0];
   else if (index == 1) return number[1];
   else {
      cerr << "Invalid index: " << index << "-- can only be 0=re, 1=im." << endl;
      exit(1);
   }

   // include the following line for stuipd Visual C++ 5.0 compiler.
   // this line should never be reached.
   return 0.0;
}



//////////////////////////////
//
// ComplexD::operator=
//

ComplexD& ComplexD::operator=(const ComplexD& aNumber) {
   // do nothing if assigning to self:
   if (this == &aNumber) {
      return *this;
   }

   re() = aNumber.number[0];
   im() = aNumber.number[1];
   return *this;
}

ComplexD& ComplexD::operator=(double aNumber) {
   re() += aNumber;
   return *this;
}



//////////////////////////////
//
// ComplexD::operator+=
//

ComplexD& ComplexD::operator+=(const ComplexD& aNumber) {
   re() += aNumber.number[0];
   im() += aNumber.number[1];
   return *this;
}



//////////////////////////////
//
// ComplexD::operator-=
//

ComplexD& ComplexD::operator-=(const ComplexD& aNumber) {
   re() -= aNumber.number[0];
   im() -= aNumber.number[1];
   return *this;
}



//////////////////////////////
//
// ComplexD::operator*=
//

ComplexD& ComplexD::operator*=(const ComplexD& aNumber) {
   double tempRe = re() * aNumber.number[0] - im() * aNumber.number[1];
   im() = aNumber.number[0] * im() + aNumber.number[1] * re();
   re() = tempRe;
   return *this;
}
   
   

//////////////////////////////
//
// ComplexD::operator/=
//

ComplexD& ComplexD::operator/=(const ComplexD& aNumber) {
   double denom = aNumber.number[0] * aNumber.number[0] + 
      aNumber.number[1] * aNumber.number[1];
   double tempRe = (re() * aNumber.number[0] + im() * aNumber.number[1])/denom;
   im() = (aNumber.number[0] * im() - re() * aNumber.number[1])/denom;
   re() = tempRe;
   return *this;
}



//////////////////////////////
//
// ComplexD::operator+
//

// unary plus sign
ComplexD ComplexD::operator+(void) const {
   return *this;
}


ComplexD ComplexD::operator+(const ComplexD& aNumber) const {
   ComplexD output(*this);
   output += aNumber;
   return output;
}


// real number addition
ComplexD ComplexD::operator+(double aNumber) const {
   ComplexD output(*this);
   output.re() += aNumber;
   return output;
}



//////////////////////////////
//
// ComplexD::operator-
//

// unary minus sign
ComplexD ComplexD::operator-(void) const {
   ComplexD output;
   output.re() = -number[0];
   output.im() = -number[1];
   return output;
}


ComplexD ComplexD::operator-(ComplexD aNumber) const {
   ComplexD output(*this);
   output -= aNumber;
   return output;
}


// real number subtraction
ComplexD ComplexD::operator-(double aNumber) const {
   ComplexD output(*this);
   output.re() -= aNumber;
   return output;
}



//////////////////////////////
//
// ComplexD::operator*
//

ComplexD ComplexD::operator*(ComplexD aNumber) const {
   ComplexD output(*this);
   output *= aNumber;
   return output;
}


// real number multiplication
ComplexD ComplexD::operator*(double aNumber) const {
   ComplexD output(*this);
   output.re() *= aNumber;
   output.im() *= aNumber;
   return output;
}



//////////////////////////////
//
// ComplexD::operator/
//

ComplexD ComplexD::operator/(ComplexD aNumber) const {
   ComplexD output(*this);
   output /= aNumber;
   return output;
}


// real number division:
ComplexD ComplexD::operator/(double aNumber) const {
   ComplexD output(*this);
   output.re() /= aNumber;
   output.im() /= aNumber;
   return output;
}



//////////////////////////////
//
// ComplexD::re
//

double& ComplexD::re(void) {
   return number[0];
}



//////////////////////////////
//
// ComplexD::zero -- sets complex number to zero
//

void ComplexD::zero(void) {
   im() = 0.0;
   re() = 0.0;
}



////////////////////////////////////////////////////////////////////////////

//////////////////////////////
//
// operator<<
//

ostream& operator<<(ostream& output, ComplexD aNumber) {
   output << '[' << aNumber.re() << ", " << aNumber.im() << ']';
   return output;
}


// md5sum:	aeae9edfa385cb8ecc52aee398d25872  ComplexD.cpp
