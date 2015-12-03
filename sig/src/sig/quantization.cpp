//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Jan 19 20:25:24 PST 1997
// Last Modified: Sun Apr 12 22:18:04 PDT 1998
// Filename:      ...sig/code/utilities/quantization.cpp
// Syntax:        C++
// $Smake:        cc -O3 -o %b %f -lg++ && strip %b
//

#include <iostream.h>
#include "quantization.h"


//////////////////////////////
//
// printCode -- prints a quantized number code in base-2 form.
//	The sign is the first digit.
//

void printCode(LinearCode x) {
   if (x.sign)
      cout << '1';
   else
      cout << '0';

   for (int i = x.bits-2; i>=0; i--) {
      if (x.code & (1 << i)) 
         cout << '1';
      else 
         cout << '0';
   }
}



//////////////////////////////
//
// printFloatCode -- prints a quantized number code in base-2 form
//	with the scale shown at the beginning in parenthesis.
//	the sign bit is the first digit of the mantissa.
//

void printFloatCode(FloatCode x) {
   int i;
   cout << '(';
   for (i = x.Rs - 1; i >=0; i--) {
      if (x.scale & (1 << i))
         cout << '1';
      else
         cout << '0';
   }
   cout << ')';

   if (x.sign)
      cout << '1';
   else
      cout << '0';
  
   for (i = x.Rm - 2; i>=0; i--) {
      if (x.mantissa & (1 << i)) 
         cout << '1';
      else 
         cout << '0';
   }
}



//////////////////////////////
//
// QuantizeMidtread
//

LinearCode QuantizeMidtread(int bits, double input) {
   double absoluteInput;
   LinearCode output;
   output.bits = bits;

   // set the sign bit:
   if (input < 0.0) {
      output.sign = 1;
      absoluteInput = -input; 
   } else {
      output.sign = 0;
      absoluteInput = input;
   }
   
   if (input >= 1.0 || input <= -1.0) {   // checking for overflow:
      output.code = (1 << (bits-1)) - 1;
   } else {
      output.code = (long)((((1 << bits) - 1)*absoluteInput + 1)/2.0);
   }

   return output;   
}



//////////////////////////////
//
// DequantizeMidtread
//

double DequantizeMidtread(LinearCode x) {
   double sign = (x.sign == 0) ? 1.0 : -1.0;
   return sign * 2.0 * x.code / ((1 << x.bits) - 1.0);
}



//////////////////////////////
//
// QuantizeMidrise
//

LinearCode QuantizeMidrise(int bits, double input) {
   double absoluteInput;
   LinearCode output;
   output.bits = bits;

   // set the sign bit:
   if (input < 0.0) {
      output.sign = 1;
      absoluteInput = -input;
   } else {
      output.sign = 0;
      absoluteInput = input;
   }

   if (input >= 1.0 || input <= -1.0) {   // checking for overflow:
      output.code = (1 << (bits-1)) - 1;
   } else {
      output.code = (long)((1 << (bits-1)) * absoluteInput);
  }

   return output;
}



//////////////////////////////
//
// DequantizeMidrise
//

double DequantizeMidrise(LinearCode x) {
   double sign = (x.sign == 0) ? 1.0 : -1.0;
   return sign * (x.code + 0.5) / (1 << (x.bits - 1));
}



//////////////////////////////
//
// FindLeadingZeros -- counts the number of most significant
//	zeros in a code.
//

int FindLeadingZeros(LinearCode x) {
   int digit = x.bits - 1;
   int counter = 0;

   while (!(x.code & (1 << (digit-1))) && digit > 0) {
      counter++;
      digit--;
   }

   return counter;
}


//////////////////////////////
//
// QuantizeFloatMidtread
//

FloatCode QuantizeFloatMidtread(int Rs, int Rm, double input) {
   // Rs = number of scale (exponent) bits
   // Rm = number of mantissa bits

   FloatCode output;
   output.Rs = Rs;
   output.Rm = Rm;
 
// Step I: Quantize the input as an R bit code where R = 2**Rs-1+Rm.

   LinearCode temp;
   temp = QuantizeMidtread((1 << Rs) - 1 + Rm, input);
   output.sign = temp.sign;

// Step II: Count the number of leading zero in |code|.  If the number
//      of leading zeros is less than 2**Rs-1 then set the scale equal
//      to 2**Rs-1 minus the number of leading zeros; otherwise set
//      the scale equal to zero.

   int leadingZeros = FindLeadingZeros(temp);

   if (leadingZeros < (1 << Rs) - 1) {
      output.scale = (1 << Rs) - 1 - leadingZeros;
   } else {
      output.scale = 0;
   }

// Step III: If scale equals zero then set the first mantissa bit
//      equal to sign and set the remaining Rm-1 bits equal to the bits
//      following the 2**Rs-1 leading zeros in |code|; otherwise set the first
//      mantissa bit equal to s and set the remaining Rm-1 bits equal to the
//      bits following the leading zeros OMITTING THE LEADING ONE.

   if (output.scale == 0) {
      output.mantissa = temp.code;
   } else {
      output.mantissa = temp.code >> (temp.bits - 1 - leadingZeros - Rm);
      // remove leading 1 (note sign is stored elsewhere):
      output.mantissa ^= (1 << (Rm-1));
   }

   return output;
}



//////////////////////////////
//
// DequantizeFloatMidtread
//

double DequantizeFloatMidtread(FloatCode x) {

// Step I: Create an R bit code where R = 2**Rs-1+Rm from the mantissa
//      and scale factor where s is the first mantissa bit and |code|...
   LinearCode temp;
   temp.bits = (1 << x.Rs) - 1 + x.Rm;
   temp.sign = x.sign;

// A: has 2**Rs-1-scale leading zeros
// B: followed by the remaining Rm-1 mantissa bits if scale is zero, 
//     otherwise followed by a one and then the remaining mantissa bits.
// C: followed by a one and as many trailing zeros as will fit if scale is
//     greater than one.

   int leadingZeros = (1 << x.Rs) - 1 - x.scale;

   if (x.scale == 0) {
      temp.code = x.mantissa;
   } else {
      // first add the leading and trailing 1's:
      temp.code = (1 << (temp.bits - 2 - leadingZeros)) +
                  (1 << (temp.bits - 2 - leadingZeros - x.Rm));
      // now fill in the blank between the two 1's:
      temp.code |= x.mantissa << (temp.bits - 1 - leadingZeros - x.Rm);
   }

// Step II: Dequantize the R bit code into the number.

   return DequantizeMidtread(temp);   
}



// md5sum:	3d7311cc3a3eb11b705e75a47c8a98ec  quantization.cpp
