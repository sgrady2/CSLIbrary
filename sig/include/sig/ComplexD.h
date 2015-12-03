//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Feb  6 23:56:35 PST 1997
// Last Modified: Sun Aug 31 16:17:24 GMT-0800 1997
// Filename:      ...sig/code/base/ComplexD/ComplexD.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../include && rm -f %b.o
//

#ifndef _COMPLEX_H_INCLUDED
#define _COMPLEX_H_INCLUDED


#include <iostream.h>


class ComplexD {
   public:
                     ComplexD           (void);
                     ComplexD           (double realPart, double imagPart = 0);
                     ComplexD           (const ComplexD& aNumber);
                    ~ComplexD           ();

      double         abs               (void) const;
      double         arg               (void) const;
      ComplexD       cexp              (double aPower = 1) const;
      ComplexD       conj              (void) const;
      ComplexD       conjugate         (void) const;
      double&        im                (void);
      double&        operator[]        (int index);
      double         operator[]        (int index) const;
      ComplexD&      operator=         (const ComplexD& aNumber);
      ComplexD&      operator=         (double aNumber);
      ComplexD&      operator+=        (const ComplexD& aNumber);
      ComplexD&      operator-=        (const ComplexD& aNumber);
      ComplexD&      operator*=        (const ComplexD& aNumber);
      ComplexD&      operator/=        (const ComplexD& aNumber);
      ComplexD       operator+         (void) const;
      ComplexD       operator+         (const ComplexD& aNumber) const;
      ComplexD       operator+         (double aNumber) const;
      ComplexD       operator-         (void) const;
      ComplexD       operator-         (ComplexD aNumber) const;
      ComplexD       operator-         (double aNumber) const;
      ComplexD       operator*         (ComplexD aNumber) const;
      ComplexD       operator*         (double aNumber) const;
      ComplexD       operator/         (ComplexD aNumber) const;
      ComplexD       operator/         (double aNumber) const;
      double&        re                (void);
      void           zero              (void);


   private:
      double     number[2];

      // nothing
};

ostream& operator<<(ostream& output, ComplexD aNumber);



#endif  // _COMPLEX_H_INCLUDED



// md5sum:	9abd07abb20f173d0d886e789607d61f  ComplexD.h
