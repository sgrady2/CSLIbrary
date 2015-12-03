//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue May 14 20:21:29 PDT 1996
// Last Modified: Sat Apr  4 16:47:49 GMT-0800 1998
// Filename:      ...sig/doc/examples/sig/sigfile/sndbit/sndbit.cpp
// Syntax:        C++; sig
//
// Description:  With this program you can mask bits in a 16-bit/sample
// soundfile, and you can also reorder the bits in the output sound file.
//
// To pass a bit to the output soundfile specify a '1' in the desired bit
// location of the -m option. To stop a bit from being passed to the output 
// soundfile, specify a '0'.  For example, to pass the 4 most significant 
// bits of the sound samples, the following command would be used:
//	sndbit -m1111000000000000 in.snd out.snd
//
// To verify that the ordering option is done properly, put the bits in 
// a random order, and then reverse the process:
//	sndbit -o0123456789abcdef in.snd out.snd     # reverses bits
//      sndbit -o0123456789abcdef out.snd in2.snd    # reverses bits again
// Now in.snd and in2.snd should be the same.
//
// Bits can be repeated multiple times in the output ordering.  For example,
// the following command will generate a sequence of 0's and -1's according
// to the value of the most-significant bit of the input sound samples:
// 	sndbit -m1000000000000000 -offffffffffffffff in.snd out.snd
//
  

#include "Options.h"
#include "sig.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream.h>
 

// function declarations:
void   checkOptions(Options& opts);
void   example(void);
void   exitUsage(const char* command);
void   help(void);
ushort makeBitMask(const char*);
ushort mixbits(const int* mixingString, const char* maskingString, 
          ushort sample);
void   printbinaryshort(ushort);


// global variables:    
Options options;         // for handling program input arguments


////////////////////////////////////////////////////////////////////////////////


int main (int argc, char *argv[]) {
   if (argc == 1) {
      exitUsage(argv[0]);
   } 

   options.define("m|mask=s:1111111111111111");
   options.define("o|order=s");
   options.define("h|help=b");
   options.define("author=b");
   options.define("version=b");
   options.define("example=b");
   options.define("usage=b");

   options.process(argc, argv);
   checkOptions(options);


   SoundFileIn insound(options.getArgument(1));
   if (insound.getBitsPerSample() != 16) {
      cout << "Error: This program can only process 16-bit samples." << endl;
      cout << "   sample size in input soundfile is " 
           << insound.getBitsPerSample() << "." << endl;
      exit(1);
   }
   SoundFileOut outsound(options.getArgument(2), insound.getHeader());


   const char* patternMaskStr  = options.getString("mask");
   const char* patternOrderStr = options.getString("order");
   ushort twoByteMask;          // for filtering bits from sound samples
   twoByteMask = makeBitMask(patternMaskStr);


   int orderingArray[16];
   if (strlen(patternOrderStr) != 0) {
      for (int i=0; i<16; i++) {
         if (isalpha(patternOrderStr[i])) {
            orderingArray[i] = tolower(patternOrderStr[i]) -'a' + 10;
            if (orderingArray[i] >= 16) {
               cout << "Error: invalid letter in ordering string.\n";
               exitUsage(options.getCommand());
            }
         } else if (isdigit(patternOrderStr[i])) {
            orderingArray[i] = patternOrderStr[i] -'0';
         } else {
            cout << "Error: invalid character in ordering string.\n";
            exitUsage(options.getCommand());
         }
      }
   }


   int sampleCount;             // total number of sound samples in input/output
   sampleCount = insound.getSamples() * insound.getChannels();
   ushort currentSample;        // current sample to process
   if (strlen(patternOrderStr) == 0) {          // not moving bits around
      for (int i=0; i<sampleCount; i++) {
         currentSample = insound.extractSample16Bit();   
         currentSample &= twoByteMask;
         outsound.insertSample16Bit(currentSample);
      }
   } else {                                     // moving bits
      for (int i=0; i<sampleCount; i++) {
         currentSample = insound.extractSample16Bit();   
         currentSample &= twoByteMask;
         outsound.insertSample16Bit(mixbits(orderingArray, patternMaskStr, 
            currentSample));
      }
   }


   return 0;
}



////////////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// checkOptions -- make sure that all program options are ok.
//

void checkOptions(Options& opts) {
   if (opts.getBoolean("help")) {
      help();
      exit(1);
   }
   if (opts.getBoolean("usage")) {
      exitUsage(opts.getCommand());
   }
   if (opts.getBoolean("example")) {
      example();
      exit(1);
   }
   if (opts.getBoolean("author")) {
      cout << "Written by Craig Stuart Sapp, "
           << "craig@ccrma.stanford.edu, May 1996" << endl;
      exit(1);
   }
   if (opts.getBoolean("version")) {
      cout << "sndbit version: Tue Apr  7 20:29:29 PDT 1998." << endl;
      cout << SIG_VERSION << endl;
      exit(1);
   }
   if (opts.getArgCount() != 2) {
      cout << "Error: must specify input and output soundfiles." << endl;
      exitUsage(opts.getCommand());
   }

   if (strlen(opts.getString("mask")) != 16) {
      cout << "Error: must specify 16 bits in masking pattern.\n";
      exitUsage(opts.getCommand());
   }

   const char *orderString = opts.getString("order");
   if (!(strlen(orderString) == 16 || strlen(orderString) == 0)) {
      cout << "Error: must specify 16 bits in ordering pattern.\n";
      cout << "   Length is " << strlen(orderString) << endl;
      exitUsage(opts.getCommand());
   }

}



//////////////////////////////
//
// example
//

void example(void) {
   cout << 
   "# sndbit example:                                            \n"
   "#   Here is an example which will quantize a soundfile to    \n"
   "#   16 loudness levels:                                      \n"
   "       osc \"0 0 1 440\" \"0 0 1 1 2 0\" -d 2 infile.snd     \n"
   "       sndbit -m 1111000000000000 infile.snd outfile.snd     \n"
   "#                                                            \n"
   "#   Here are the least significant bits amplified:           \n"
   "       sndbit -m 0000000000001111 -o3210ffffffffffff \\      \n"
   "          infile.snd outfile.snd                             \n"
   << endl;
}



//////////////////////////////
//
// exitUsage
//

void exitUsage(const char* command) {
   cout << "\nUsage: " << command 
        << " [-m maskingBitPattern][-o orderingBitPattern] inFile outFile\n";
   cout << "   Removes bits/reorders bits in a soundfile.\n"
        << "   The default options will not change the input.\n";
   cout << "   Options are:\n";
   cout << "      -m maskingBitPattern  = 0 removes a bit, 1 keeps a bit.\n";
   cout << "                              Default is 1111111111111111\n";
   cout << "      -o orderingBitPattern = Default is fedcba9876543210\n";
   cout << "      -h for further help.\n";
   cout << endl;

   exit(1);
}



//////////////////////////////
//
// help
//

void help(void) {
   cout << 
   "Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>\n"
   "Creation Date: Tue May 14 20:21:29 PDT 1996\n"
   "Last Modified: Sat Apr  4 18:16:15 GMT-0800 1998 \n"
   "\n"
   "Description:  With this program you can mask bits in a 16-bit/sample\n"
   "soundfile, and you can also reorder the bits in the output sound file.\n"
   "\n"
   "To pass a bit to the output soundfile specify a '1' in the desired bit\n"
   "location of the -m option. To stop a bit from being passed to the output \n"
   "soundfile, specify a '0'.  For example, to pass the 4 most significant \n"
   "bits of the sound samples, the following command would be used:\n"
   "sndbit -m1111000000000000 in.snd out.snd\n"
   "\n"
   "To verify that the ordering option is done properly, put the bits in \n"
   "a random order, and then reverse the process:\n"
   "     sndbit -ofedcba9786543210 in.snd out.snd\n"
   "     sndbit -ofedcba9786543210 out.snd in2.snd\n"
   "Now in.snd and in2.snd should be the same.\n"
   "\n"
   "Bits can be repeated multiple times in the output ordering.  For example,\n"
   "the following command will generate a sequence of 0's and -1's according\n"
   "to the value of the most-significant bit of the input sound samples:\n"
   "	sndbit -m1000000000000000 -offffffffffffffff in.snd out.snd\n"
   << endl;
}



//////////////////////////////
//
// makeBitMask
//

ushort makeBitMask(const char* str) {
   int i;
   int length;
   ushort value = 0;
   length = strlen(str);
   for (i=0; i<16; i++) {
      if (str[i] == '1') 
         value = (value << 1) | 0x0001;
      else if (str[i] == '0') 
         value = value << 1;
      else {
         fprintf(stderr, "Invalid masking pattern character: %c.", str[i]);
         fprintf(stderr, "  Must be a '1' or a '0'.\n");
      }
   }
   return value;
}



//////////////////////////////
//
// mixbits
//

ushort mixbits(const int* mixingString, const char* maskingString, 
      ushort sample) {
   int origPos, newPos, move;
   ushort origBit, newBit;
   ushort value = 0;
   int i;

   for (i=0; i<16; i++) {
      origPos = mixingString[i];
      if (maskingString[15 - origPos] == '0') continue;
      origBit = (sample & (0x0001 << origPos)); 
      newPos = 15 - i;
      move = newPos - origPos;
      newBit = (move >= 0) ? (origBit << move) : (origBit >> -move);
      value = value | newBit;
   }
   return value;
}



///////////////////////////////
//
// printbinaryshort -- useful, but not used anymore.
//

void printbinaryshort(ushort input) {
   int i;
   for (i=15; i>=0; i--) {
      if ((input & (0x0001 << i)) == 0)
         printf("0");
      else
         printf("1");
   }
   printf("\n");
}




