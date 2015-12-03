//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Feb  7 00:44:33 PST 1997
// Last Modified: Thu Dec 18 18:39:40 GMT-0800 1997
// Last Modified: Sun Jun 11 12:55:07 PDT 2000 (improved memory management)
// Filename:      .../sig/code/utilities/transforms/transforms.cpp
// Syntax:        C++ 
//
// Description:   Basic signal <-> frequency transforms.
//

#ifndef _TRANSFORMS_CPP_INCLUDED
#define _TRANSFORMS_CPP_INCLUDED

#include "transforms.h"
#include "Block.h"
#include "ComplexD.h"
#include <math.h>
#include <iostream.h>
#include <stdlib.h>

// Private helper functions for this file:
#define READ_TRANSFORMS_PRIVATE
   #include "transforms-private.h"
#undef READ_TRANSFORMS_PRIVATE


//////////////////////////////
//
// FFT -- Fast Fourier Transform O(N Log N)
//   Returns the complex spectrum of the given complex input signal.
//   Length of Block must be a power of 2.
//

Block<ComplexD>& FFT(Block<ComplexD>& outputSpectrum, 
      const Block<ComplexD>& inputSignal) {
   int N = inputSignal.getSize();
 
   if (!poweroftwo(N)) {
      cerr << "You can only take the FFT of a block with length being" 
           << " a power of 2.\nRequested transform length: " << N << endl;
      exit(1);
   }

   outputSpectrum = inputSignal;
   fft_destructive(outputSpectrum);
   return outputSpectrum;
}



//////////////////////////////
//
// IFFT -- Inverse Fast Fourier Transform O(N Log N).
//   Returns the complex signal of the given complex input spectrum.
//   Length of Block must be a power of 2.
//

Block<ComplexD>& IFFT(Block<ComplexD>& outputSignal, 
      const Block<ComplexD>& inputSpectrum) {
   int N = inputSpectrum.getSize();
 
   if (!poweroftwo(N)) {
      cerr << "You can only take the IFFT of a block with length being" 
           << " a power of 2.\nRequested transform length: " << N << endl;
      exit(1);
   }

   outputSignal = inputSpectrum;
   ifft_destructive(outputSignal);
   return outputSignal;
}



//////////////////////////////
//
// DirectDFT -- Direct Discrete Fourier Transform O(N^2).
//   Returns the complex spectrum of the given complex input signal.
//   Quickly becomes slower than the FFT if the length of the input
//   block is greater than about 100.
//

Block<ComplexD>& DirectDFT(Block<ComplexD>& outputSpectrum, 
      const Block<ComplexD>& inputSignal) {
   int N = inputSignal.getSize();
   const Block<ComplexD>& x = inputSignal;
   outputSpectrum.setSize(N);
   Block<ComplexD>& X = outputSpectrum;
   ComplexD sum;
   int k, n;
   ComplexD kernel(0,0);
   double mtpn = -2 * 3.14159265359 / N;


   for (k=0; k<N; k++) {
      sum.zero();
      for (n=0; n<N; n++) {
        kernel.im() = mtpn * k * n;
        sum += x[n] * kernel.cexp();
      }
      X[k] = sum;
   }
  
   return outputSpectrum;
}



////////////////////////////// 
//
// DFT -- Selects the DirectDFT or the FFT as appropriate.
//

Block<ComplexD>& DFT(Block<ComplexD>& outputSpectrum, 
      const Block<ComplexD>& inputSignal) {
   int N = inputSignal.getSize();
   if (poweroftwo(N) && N > 63) {
      return FFT(outputSpectrum, inputSignal);
   } else {
      return DirectDFT(outputSpectrum, inputSignal);
   }
}



//////////////////////////////
//
// DirectIDFT -- Direct Inverse Discrete Fourier Transform O(N^2).
//   Returns the complex spectrum of the given complex input signal.
//   Quickly becomes slower than the IFFT if the length of the input
//   block is greater than about 100.
//

Block<ComplexD>& DirectIDFT(Block<ComplexD>& outputSignal, 
      const Block<ComplexD>& inputSpectrum) {
   int N = inputSpectrum.getSize();
   const Block<ComplexD>& X = inputSpectrum;
   outputSignal.setSize(N);
   Block<ComplexD>& x = outputSignal;
   ComplexD sum;
   int k, n;
   ComplexD kernel(0,0);
   double tpn = 2.0 * 3.14159265359 / (double)N;

   for (n=0; n<N; n++) {
      sum.zero();
      for (k=0; k<N; k++) {
        kernel.im() = tpn * k * n;
        sum += X[k] * kernel.cexp();
      }
      x[n] = sum/N;
   }
  
   return outputSignal;
}



////////////////////////////// 
//
// IDFT -- Selects the DirectIDFT or the IFFT as appropriate.
//

Block<ComplexD>& IDFT(Block<ComplexD>& outputSignal, 
      const Block<ComplexD>& inputSpectrum) {
   int N = inputSpectrum.getSize();
   if (poweroftwo(N) && N > 63) {
      return IFFT(outputSignal, inputSpectrum);
   } else {
      return DirectIDFT(outputSignal, inputSpectrum);
   }
}



////////////////////////////// 
//
// RealFFT -- Calculates the complex spectrum of a real signal.
//     Uses a N/2 length FFT and therfore more efficient
//     than the plain FFT for purely real signals. Can accept 
//     Block<float> or Block<double>.  Input signal length must 
//     be a power of 2.
//

template<class type>
Block<ComplexD>& RealFFT(Block<ComplexD>& outputSpectrum, 
      const Block<type>& realSignal) {
   int& N = realSignal.getSize()/2;

   if (!poweroftwo(N)) {
      cerr << "You can only take the RealFFT of a block with length being" 
           << " a power of 2.\nRequested transform length: " << N << endl;
      exit(1);
   }

   Block<type>& x = realSignal;
   outputSpectrum.setSize(2*N);
   Block<type>& X = outputSpectrum;
   type pi = 4.0 * atan(1.0);
   int n, k;


   // Pre-twiddle: Interpret x as N interleaved complex samples
   Block<ComplexD> y(N);
   for (n=0; n<N-1; n++) {
      y[n].re() = x[2*n];
      y[n].im() = x[2*n+1];
   }

   // Compute the FFT of y(n): Y(k) = R(k) + j I(k)
   Block<ComplexD> Y;
   Y = FFT(y);

   // Post-twiddle: 
   for (k=0; k<N; k++) {
      X[k].re() = (Y[k].re() + Y[N-k].re())/2 + cos(pi*k/(2*N)*(Y[k].im() + 
                Y[N-k].im())) - sin(pi*k/(2*N)*(Y[k].re() - Y[N-k].re()));
      X[k].im() = (Y[k].im() + Y[N-k].im())/2 
                - sin(pi*k/(2*N)*(Y[k].im() + Y[N-k].im())) 
                - cos(pi*k/(2*N)*(Y[k].re() + Y[N-k].re()));
   }
   for (k=N; k<2*N; k++) {
      X[k].re() = X[N-k-1];
      X[k].im() = -X[N-k-1];
   }
               
   return outputSpectrum;
}



//////////////////////////////
//
// DirectMDCT -- Modified Discrete Cosine Transform O(N^2).
//    Can handle Block<float> and Block<double> as input.
//

template<class type>
Block<type>& DirectMDCT(Block<type>& outputSpectrum, 
      const Block<type>& inputSignal) {
   int N = inputSignal.getSize();
   Block<type>& x = inputSignal;
   outputSpectrum.setSize(N/2);
   Block<type>& X = outputSpectrum;
   type n0  = ((type)N / 2.0 + 1.0) / 2.0;
   type tpn = 2.0 * (4.0 * atan(1.0)) / N;
   type sum;
   int k, n;

   if (N%2 != 0) {
      cerr << "Error: must have an even number of elements to do an MDCT." 
           << endl;
      exit(1);
   }


   for (k=0; k<N/2; k++) {
      sum = 0;
      for (n=0; n<N; n++) {
         sum += x[n] * cos(tpn*(n+n0)*(k+0.5));
      }
      X[k] = sum;
   }


   return outputSpectrum;
}



//////////////////////////////
//
// FastMDCT -- Modified Discrete Cosine Transform (N Log N).
//    Can handle Block<float> and Block<double> as input.
//

template<class type>
Block<type>& FastMDCT(Block<type>& outputSpectrum, 
      const Block<type>& inputSignal) {
   Block<type>& x = inputSignal;
   int N = inputSignal.getSize();
   outputSpectrum.setSize(N/2);
   Block<type>& X = outputSpectrum;
   type pi = 4.0 * atan(1.0);
   type tpn0 = 2.0 * pi * (N/2.0+1.0)/2.0;
   int n, k;

   // Pre-twiddle   $$ y(n) = x(n)\cos(\pi n/N) - j x(n)\sin(\pi n/N) $$
   Block<ComplexD> y(N);
   for (n=0; n<N; n++) {
      y[n].re() = x[n] * cos(pi*n/N);
      y[n].im() = -x[n] * sin(pi*n/N);
   }
      

   // Compute the FFT of y(n)
   Block<ComplexD> Y;
   Y = FFT(y);                    // only going to need first half of spectrum


   // Post-twiddle  $$ X(k) = \re{Y(k)}\cos(2\pi n_0(k+1/2)/N) +
   //                            \im{Y(k)}\sin(2\pi n_0(k+1/2)/N)
   for (k=0; k<N/2; k++) {
      X[k] = Y[k].re() * cos(tpn0*(k+0.5)/N) + Y[k].im() * sin(tpn0*(k+0.5)/N);
   }

   return outputSpectrum;
}



//////////////////////////////
//
// MDCT -- Selects the FastMDCT or DirectMDCT as appropriate.
//

template<class type>
Block<type>& MDCT(Block<type>& outputSpectrum, 
      const Block<type>& inputSignal) {
   int N = inputSignal.getSize();

   if (poweroftwo(N) && N > 127) {
      return FastMDCT(outputSpectrum, inputSignal);
   } else {
      return DirectMDCT(outputSpectrum, inputSignal);
   }
}
      


//////////////////////////////
//
// DirectIMDCT -- Inverse Modified Discrete Cosine Transform O(N^2).
//    Can handle Block<float> and Block<double> as input. 
//

template<class type>
Block<type>& DirectIMDCT(Block<type>& outputSignal, 
      const Block<type>& inputSpectrum) {
   int N = inputSpectrum.getSize()*2;
   Block<type>& X = inputSpectrum;
   outputSignal.setSize(N);
   Block<type>& x = outputSignal;
   type n0  = ((type)N/2.0+1.0)/2.0;
   type pi = 4.0 * atan(1.0);
   type tpn = 2.0 * pi/N;
   int k, n;
   type sum;


   for (n=0; n<N; n++) {
      sum = 0;
      for (k=0; k<N/2; k++) {
         sum += X[k] * cos(tpn*(n+n0)*(k+0.5));
      }
      x[n] = 2.0/N * sum;
   }


   return outputSignal;
}
    


//////////////////////////////
//
// FastIMDCT -- Inverse Modified Discrete Cosine Transform O(N Log N).
//    Can handle Block<float> and Block<double> as input.
//

template<class type>
Block<type>& FastIMDCT(Block<type>& outputSignal, 
      const Block<type>& inputSpectrum) {
   int N = inputSpectrum.getSize()*2;
   Block<type> fullSpectrum(N);
   Block<type>& X = fullSpectrum;
   outputSignal.setSize(N);
   Block<type>& x = outputSignal;
   type n0  = ((type)N/2.0+1.0)/2.0;
   type pn = (4.0 * atan(1.0))/N;
   type tpn0 = 2.0 * pn * n0;
   int n, k;

   // recreate full spectrum
   for (n=0; n<N/2; n++) {
      X[n] = inputSpectrum[n];
   }
   for (n=N/2; n<N; n++) {
      X[n] = -inputSpectrum[N-n-1];
   }

   // Pre-twiddle:  $$ Z(k) = X(k)\cos(\pi kn_0/N) + j X(k)\sin(\pi kn_0/N) $$
   Block<ComplexD> Z(N);
   for (k=0; k<N; k++) {
      Z[k].re() = X[k] * cos(tpn0 * k);
      Z[k].im() = X[k] * sin(tpn0 * k);
   }

   // Compute the IFFT of Z(k)
   Block<ComplexD> z;
   z = IFFT(Z);
   
   // Post-twiddle:  $$x(n) = 2 \re{z(n)}\cos(2\pi(n+n_0)/2N) - 
   //                    2 \im{z(n)}\sin(2\pi(n+n_0)/2N) $$
   for (n=0; n<N; n++) {
      x[n] = 2.0*(z[n].re() * cos(pn*(n+n0)) - z[n].im() * sin(pn*(n+n0)));
   }

   return outputSignal;
}



//////////////////////////////
//
// IMDCT -- Selects the DirectIMDCT or FastIMDCT as apprpriate.
//

template<class type>
Block<type>& IMDCT(Block<type>& outputSignal, 
      const Block<type>& inputSpectrum) {
   int N = inputSpectrum.getSize();
   
   if (poweroftwo(N) && N > 127) {
      return FastIMDCT(outputSignal, inputSpectrum);
   } else {
      return DirectIMDCT(outputSignal, inputSpectrum);
   }
}



#endif  // _TRANSFORMS_CPP_INCLUDED



// md5sum:	69cd38a4d6488eaccad38e603427f478  transforms.cpp
