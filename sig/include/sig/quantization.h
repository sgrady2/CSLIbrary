//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Jan 19 20:25:24 PST 1997
// Last Modified: Sun Apr 12 22:18:04 PDT 1998
// Filename:      ...sig/code/utilities/quantization.h
// Syntax:        C++
// $Smake:        cc -O3 -o %b %f -lg++ && strip %b
//

#ifndef _QUANTIZATION_H_INCLUDED
#define _QUANTIZATION_H_INCLUDED

#include <iostream.h>

struct linearcode {
   int   bits;        // number of bits used to quantize number
   int   sign;        // sign represents the highest bit of code
   long  code;        // code without the sign bit (max 32 bits)
};

typedef struct linearcode LinearCode;

struct floatcode {
   int Rs;            // number of scale (exponent) bits
   int Rm;            // number of mantissa bits
   long scale;        // storage space for the scale bits (max 32 bits)
   int sign;          // sign represents the highest bit of the mantissa
   long mantissa;     // mantissa without the sign bit (max 32 bits)
};

typedef struct floatcode FloatCode;


//////////////////////////////
//
// printCode -- prints a quantized number code in base-2 form.
//	The sign is the first digit.
//

void printCode(LinearCode x);



//////////////////////////////
//
// printFloatCode -- prints a quantized number code in base-2 form
//	with the scale shown at the beginning in parenthesis.
//	the sign bit is the first digit of the mantissa.
//

void printFloatCode(FloatCode x);



//////////////////////////////
//
// QuantizeMidtread
//

LinearCode QuantizeMidtread(int bits, double input);



//////////////////////////////
//
// DequantizeMidtread
//

double DequantizeMidtread(LinearCode x);



//////////////////////////////
//
// QuantizeMidrise
//

LinearCode QuantizeMidrise(int bits, double input);



//////////////////////////////
//
// DequantizeMidrise
//

double DequantizeMidrise(LinearCode x);



//////////////////////////////
//
// FindLeadingZeros -- counts the number of most significant
//	zeros in a code.
//

int FindLeadingZeros(LinearCode x);



//////////////////////////////
//
// QuantizeFloatMidtread
//

FloatCode QuantizeFloatMidtread(int Rs, int Rm, double input);



//////////////////////////////
//
// DequantizeFloatMidtread
//

double DequantizeFloatMidtread(FloatCode x);



#endif  // _QUANTIZATION_H_INCLUDED
// md5sum:	34e3ddf7974778eadcc2ad2dd0daef4a  quantization.h
