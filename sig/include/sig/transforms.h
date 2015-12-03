//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Feb  7 00:44:33 PST 1997
// Last Modified: Thu Dec 18 18:45:13 GMT-0800 1997
// Last Modified: Sun Jun 11 12:55:07 PDT 2000 (improved memory management)
// Filename:      .../sig/code/utilities/transforms/transforms.h
// Syntax:        C++ 
//
// Description:   Basic signal <-> frequency transforms.
//

#ifndef _TRANSFORMS_H_INCLUDED
#define _TRANSFORMS_H_INCLUDED

#include "Block.h"
#include "ComplexD.h"



//////////////////////////////
//
// DirectDFT -- Direct Discrete Fourier Transform O(N^2).
//   Returns the complex spectrum of the given complex input signal.
//   Quickly becomes slower than the FFT if the length of the input
//   block is greater than about 100.
//

Block<ComplexD>& DirectDFT(Block<ComplexD>& outputSpectrum, 
   const Block<ComplexD>& inputSignal);



//////////////////////////////
//
// FFT -- Fast Fourier Transform O(N Log N).
//   Returns the complex spectrum of the given complex input signal.
//   Length of Block must be a power of 2.
//

Block<ComplexD>& FFT(Block<ComplexD>& outputSpectrum,
   const Block<ComplexD>& inputSignal);



//////////////////////////////
//
// DFT -- Selects the DirectDFT or the FFT as appropriate.
//

Block<ComplexD>& DFT(Block<ComplexD>& outputSpectrum, 
   const Block<ComplexD>& inputSignal);



//////////////////////////////
//
// DirectIDFT -- Direct Inverse Discrete Fourier Transform O(N^2).
//   Returns the complex spectrum of the given complex input signal.
//   Quickly becomes slower than the IFFT if the length of the input
//   block is greater than about 100.
//

Block<ComplexD>& DirectIDFT(Block<ComplexD>& outputSignal, 
   const Block<ComplexD>& inputSpectrum);



//////////////////////////////
//
// IFFT -- Inverse Fast Fourier Transform O(N Log N).
//   Returns the complex signal of the given complex input spectrum.
//   Length of Block must be a power of 2.
//

Block<ComplexD>& IFFT(Block<ComplexD>& outputSignal, 
   const Block<ComplexD>& inputSpectrum);



//////////////////////////////
//
// IDFT -- Selects the DirectIDFT or the IFFT as appropriate.
//

Block<ComplexD>& IDFT(Block<ComplexD>& outputSignal, 
   const Block<ComplexD>& inputSpectrum);



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
   const Block<type>& realSignal);



//////////////////////////////
//
// DirectMDCT -- Modified Discrete Cosine Transform O(N^2).
//    Can handle Block<float> and Block<double> as input.
//

template<class type>
Block<type>& DirectMDCT(Block<type>& outputSignal, 
   const Block<type>& inputSignal);



//////////////////////////////
//
// FastMDCT -- Modified Discrete Cosine Transform (N Log N).
//    Can handle Block<float> and Block<double> as input.
//

template<class type>
Block<type>& FastMDCT(Block<type>& outputSpectrum, 
   const Block<type>& inputSignal);



//////////////////////////////
//
// MDCT -- Selects the FastMDCT or DirectMDCT as appropriate.
//

template<class type>
Block<type>& MDCT(Block<type>& outputSpectrum, const Block<type>& inputSignal);



//////////////////////////////
//
// DirectIMDCT -- Inverse Modified Discrete Cosine Transform O(N^2).
//    Can handle Block<float> and Block<double> as input.
//

template<class type>
Block<type>& DirectIMDCT(Block<type>& outputSignal, 
   const Block<type>& inputSpectrum);



//////////////////////////////
//
// FastIMDCT -- Inverse Modified Discrete Cosine Transform O(N Log N).
//    Can handle Block<float> and Block<double> as input.
//

template<class type>
Block<type>& FastIMDCT(Block<type>& outputSignal, 
   const Block<type>& inputSpectrum);



//////////////////////////////
//
// IMDCT -- Selects the DirectIMDCT or FastIMDCT as apprpriate.
//

template<class type>
Block<type>& IMDCT(Block<type>& outputSignal, const Block<type>& inputSpectrum);



#endif  // _TRANSFORMS_H_INCLUDED

// md5sum:	ff847ea4d67c2254b2de2f3fe8b3b0c4  transforms.h
