//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Feb  7 00:44:33 PST 1997
// Last Modified: Thu Feb 12 20:17:59 GMT-0800 1998
// Filename:      ...sig/code/utilities/transforms/transforms-private.cc
// Syntax:        C++ 
// $Smake:        cc -g -c %b.cc -I../../../include && rm -f %b.o
//
// Description: Private helper functions for the Transforms.cc file.
//     Many of these functions are taken from:
//     Ofranidis, Sophocles J.  "Introduction to Signal Processing", 
//          Prentice Hall, 1996.  page 520.
//


#ifdef READ_TRANSFORMS_PRIVATE

#ifndef _TRANSFORMS_PRIVATE_H_INCLUDED
#define _TRANSFORMS_PRIVATE_H_INCLUDED


#include "Block.h"
#include "ComplexD.h"
#include <math.h>

void     fft_destructive(Block<ComplexD>& X);
void     ifft_destructive(Block<ComplexD>& X);
void     shuffle(Block<ComplexD>& X);
void     dftmerge(Block<ComplexD>& X);
void     swap(ComplexD& a, ComplexD& b);
int      bitrev(int n, int B);
ComplexD makeComplex(double rePart, double imagPart);
ComplexD cexp(ComplexD Z);
int      poweroftwo(long number);


///////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// fft_destructive
//

void fft_destructive(Block<ComplexD>& X) {
   shuffle(X);   // bit reversal
   dftmerge(X);  // merging of DFTs
}



//////////////////////////////
//
// ifft_destructive
//

void ifft_destructive(Block<ComplexD>& X) {
   int N = X.getSize();
   int k;

   for (k=0; k<N; k++) {
      X[k] = X[k].conj();               // conjugate input
   }

   fft_destructive(X);                  // computer FFT of conjugate
 
   for (k=0; k<N; k++) {
      X[k] = X[k].conj() / (double)N;   // conjugate and divide by N
   }
}



//////////////////////////////
//
// shuffle
//

void shuffle(Block<ComplexD>& X) {
   int n, r;
   int N = X.getSize();
   int B = 1;                    // number of bits

   while ( (N >> B) > 0 ) {
      B++;
   }

   B--;                          // N = 2**B

   for (n=0; n<N; n++) {
      r = bitrev(n, B);          // bit-reversed version of n
      if (r<n) continue;         // swap only half of the n's
      swap(X[n], X[r]);          // swap values
   }
}



//////////////////////////////
//
// swap -- Swap the location of two Complex numbers.
//

void swap(ComplexD& a, ComplexD& b) {
   ComplexD temp(a);   
   a = b;
   b = temp;
}



//////////////////////////////
//
// bitrev -- reverse bits in a number n with B number of bits
//

#define two(x) (1 << (x))	 /* 2**x by left-shifting */

int bitrev(int n, int B) {
   int m, r;
   for (r=0, m=B-1; m>=0; m--) { // B is the number of digits
      if ((n>>m)==1) {           // if 2**m term is present
         r += two(B-1-m);        // add 2**(B-1-m) to r
         n -= two(m);            // subtract 2**m from n
      }
   }
   return r;
}



//////////////////////////////
//
// makeComplex -- make a Complex number given the real and imaginary parts.
//

ComplexD makeComplex(double realPart, double imagPart) {
   ComplexD output(realPart, imagPart);
   return output;
}



//////////////////////////////
//
// cexp -- complex exponential
//

ComplexD cexp(ComplexD Z) {
   double R = exp(Z.re());
   ComplexD output(R * cos(Z.im()), R * sin(Z.im()));
   return output;
}


//////////////////////////////
//
// dftmerge -- put DFTs back together 
//

void dftmerge(Block<ComplexD>& XF) {
   int N = XF.getSize();
   double pi = 4.0 * atan(1.0);
   int k, i, p, q, M;
   ComplexD A, B, V, W;

   M = 2;
 
   while (M <= N) {                            // two (M/2)-DFT's into one M-DFT
      W = cexp(makeComplex(0.0, -2 * pi / M)); // order-M twiddle factor
      V = makeComplex(1., 0.);                 // successive powers of W
      for (k=0; k<M/2; k++) {                  // index for an (M/2)-DFT
         for (i=0; i<N; i+=M) {                // ith butterfly; increments by M
            p = k+i;                           // absoute indices for
            q = p + M / 2;                     //    ith butterfly
            A = XF[p];
            B = XF[q] * V;                     // V = W**k
            XF[p] = A + B;                     // butterfly operations
            XF[q] = A - B;
         }
         V = V * W;                            // V = VW = W**(k+1)
      }    
      M = 2 * M;                               // next stage
   }
}



//////////////////////////////
//
// poweroftwo -- returns if input is a power of two.
//      a negative number is not a power of 2.
//      algorithm: count the number of binary digits, if there is
//           exatly one non-zero digit, then assume a power of two.
//

int poweroftwo(long number) {   
   int sum = 0;
   for (unsigned int i=0; i<sizeof(long)*8; i++) {
      sum += (number >> i) & 0x00000001;
   }
   return (sum == 1);
}



#endif  // _TRANSFORMS_PRIVATE_H_INCLUDED

#endif  // READ_TRANSFORMS_PRIVATE
// md5sum:	1b17269601d1135391c6081349391f4b  transforms-private.h
