//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Nov  2 10:25:09 PST 1996
// Last Modified: Sat May 24 19:02:27 GMT-0800 1997
// Filename:      ...sig/code/Filter/SoundFileOut/SoundFileOut.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#include "SoundFileOut.h"
#include "helperFunctions.h"
#include <iostream.h>
#include <stdlib.h>
#include <string.h>


//////////////////////////////
//
// SoundFileOut::SoundFileOut
//

SoundFileOut::SoundFileOut(char* aFileName, SoundHeader& header) {
   filename = new char[strlen(aFileName)+1];
   strcpy(filename, aFileName);
   outSample = 0;
   setName("SoundFileOut");
   #ifdef VISUAL            /* for stupid LF-CR prevention in DOS */
      outFile.open(filename, ios::out | ios::noreplace | ios::binary);
   #else
      outFile.open(filename, ios::out | ios::noreplace);
   #endif
   if (!outFile.is_open()) {
      cerr << "Error: cannot write soundfile " << filename 
           << ". Perhaps the file already exists?" << endl;
      exit(1);
   }
   sndHeader = header;
   sndHeader.modifyFormat(aFileName);
   outFile << sndHeader;
   outFile.seekp(sndHeader.getDataByteOffset(), ios::beg);
   sampleCount = 0;
   brandname = FILTER;
   determineSampleWritingFunction();
}



//////////////////////////////
//
// SoundFileOut::~SoundFileOut
//


SoundFileOut::~SoundFileOut() {
   close();
}



//////////////////////////////
//
// SoundFileOut::action
//

void SoundFileOut::action(void) {
   for (int i=0; i<sndHeader.getChannels(); i++) {
      writeSample(outFile, inputs[i]);
   }
   sampleCount += sndHeader.getChannels();
}



//////////////////////////////
//
// SoundFileOut::close
//

void SoundFileOut::close(void) {
   if (sndHeader.getSamples() != sampleCount/sndHeader.getChannels()) {
      switch (sndHeader.getOutputType()) {
         case TYPE_SND:
            // write the number of bytes in the data:
            outFile.seekp(8, ios::beg);   
            outFile.writeBigEndian((long)
               (sampleCount/sndHeader.getChannels()*sndHeader.getBlockAlign()));
            break;
         case TYPE_WAV_PCM:
            outFile.seekp(8, ios::beg);
            // write the number of bytes in the file minus a few:
            outFile.seekp(4, ios::beg);   
            outFile.writeLittleEndian((long)
               (36 + sampleCount/sndHeader.getChannels() * 
                  sndHeader.getBlockAlign()));
            // write the number of sound data bytes:
            outFile.seekp(40, ios::beg);
            outFile.writeLittleEndian((long)
               (sampleCount/sndHeader.getChannels()*sndHeader.getBlockAlign()));
            break;
         default:
           cerr << "Unknown output soundfile type: "
                << sndHeader.getOutputType() << endl;
           exit(1);
      }
   }
   outFile.close();
   sampleCount = 0;
}



//////////////////////////////
//
// SoundFileOut::getFilename -- will never return a NULL pointer
//

const char* SoundFileOut::getFilename(void) const {
   if (filename == NULL) {
      return "";
   } else {
      return filename;
   }
}



//////////////////////////////
//
// SoundFileOut::insertSample16Bit
//

void SoundFileOut::insertSample16Bit(short sample) {
   switch (endianType) {
      case SIG_BIG_ENDIAN:
         outFile.writeBigEndian(sample);
         break;
      case SIG_LITTLE_ENDIAN:
         outFile.writeLittleEndian(sample);
         break;
   }
   sampleCount++;
}
     


//////////////////////////////
//
// SoundFileOut::output
//

sampleType SoundFileOut::output(int channel) {
   return inputs[channel];
}



///////////////////////////////////////////////////////////////////////////
//
// private functions:
//

uchar linear16ToMulaw8(short aValue);

// big endian samples
void  soundWriteSample8M_B      (FileIO& outFile, sampleType aSample);
void  soundWriteSample8L_B      (FileIO& outFile, sampleType aSample);
void  soundWriteSample16L_B     (FileIO& outFile, sampleType aSample);
void  soundWriteSample24L_B     (FileIO& outFile, sampleType aSample);
void  soundWriteSample32L_B     (FileIO& outFile, sampleType aSample);
void  soundWriteSample32F_B     (FileIO& outFile, sampleType aSample);
void  soundWriteSample64F_B     (FileIO& outFile, sampleType aSample);

// little endian samples
void  soundWriteSample8M_L      (FileIO& outFile, sampleType aSample);
void  soundWriteSample8L_L      (FileIO& outFile, sampleType aSample);
void  soundWriteSample16L_L     (FileIO& outFile, sampleType aSample);
void  soundWriteSample24L_L     (FileIO& outFile, sampleType aSample);
void  soundWriteSample32L_L     (FileIO& outFile, sampleType aSample);
void  soundWriteSample32F_L     (FileIO& outFile, sampleType aSample);
void  soundWriteSample64F_L     (FileIO& outFile, sampleType aSample);



//////////////////////////////
//
// SoundFileIn::determineSampleWritingFunction
//

void SoundFileOut::determineSampleWritingFunction(void) {
   switch (sndHeader.getOutputType()) {
      case TYPE_SND:                           // bigendian samples
         endianType = SIG_BIG_ENDIAN;
         switch (sndHeader.getNextFormat()) {
            case SND_FORMAT_MULAW_8:
               writeSample = soundWriteSample8M_B;
               break;
            case SND_FORMAT_LINEAR_8:
               writeSample = soundWriteSample8L_B;
               break;
            case SND_FORMAT_LINEAR_16:
               writeSample = soundWriteSample16L_B;
               break;
            case SND_FORMAT_LINEAR_24:
               writeSample = soundWriteSample24L_B;
               break;
            case SND_FORMAT_LINEAR_32:
               writeSample = soundWriteSample32L_B;
               break;
            case SND_FORMAT_FLOAT:
               writeSample = soundWriteSample32F_B;
               break;
            case SND_FORMAT_DOUBLE:
               writeSample = soundWriteSample64F_B;
               break;
            default:
               cerr << "Error: unknown output soundfile format: "
                    << sndHeader.getNextFormat() << endl;
               exit(1);
         }
         break;
      case TYPE_WAV_PCM:                      // little endian samples
         endianType = SIG_LITTLE_ENDIAN;
         switch (sndHeader.getNextFormat()) {
            case SND_FORMAT_MULAW_8:
               writeSample = soundWriteSample8M_L;
               break;
            case SND_FORMAT_LINEAR_8:
               writeSample = soundWriteSample8L_L;
               break;
            case SND_FORMAT_LINEAR_16:
               writeSample = soundWriteSample16L_L;
               break;
            case SND_FORMAT_LINEAR_24:
               writeSample = soundWriteSample24L_L;
               break;
            case SND_FORMAT_LINEAR_32:
               writeSample = soundWriteSample32L_L;
               break;
            case SND_FORMAT_FLOAT:
               writeSample = soundWriteSample32F_L;
               break;
            case SND_FORMAT_DOUBLE:
               writeSample = soundWriteSample64F_L;
               break;
            default:
               cerr << "Error: unknown output soundfile format: "
                    << sndHeader.getNextFormat() << endl;
               exit(1);
         }
         break;
      default:
         cerr << "Error: unknown soundfile type: "
              << sndHeader.getOutputType() << endl;
         exit(1);
   }
}



//////////////////////////////
//
// soundWriteSample8M -- read channel samples as
//      8-bit Mulaw values
//

void soundWriteSample8M_B(FileIO& outFile, sampleType aSample) {
   outFile.writeBigEndian(linear16ToMulaw8(SampleToShort(aSample)));
}


void soundWriteSample8M_L(FileIO& outFile, sampleType aSample) {
   outFile.writeLittleEndian(linear16ToMulaw8(SampleToShort(aSample)));
}



//////////////////////////////
//
// soundWriteSample8L
//

void soundWriteSample8L_B(FileIO& outFile, sampleType aSample) {
   outFile.writeBigEndian(SampleToChar(aSample));
}


void soundWriteSample8L_L(FileIO& outFile, sampleType aSample) {
   outFile.writeBigEndian(SampleToChar(aSample));
}



//////////////////////////////
//
// soundWriteSample16L
//

void soundWriteSample16L_B(FileIO& outFile, sampleType aSample) {
   outFile.writeBigEndian(SampleToShort(aSample));
}


void soundWriteSample16L_L(FileIO& outFile, sampleType aSample) {
   outFile.writeLittleEndian(SampleToShort(aSample));
}



//////////////////////////////
//
// soundWriteSample24L
//

void soundWriteSample24L_B(FileIO& outFile, sampleType aSample) {
   static long output;
   output = SampleToLong(aSample);
   outFile.writeBigEndian((uchar)(output >> 12));
   outFile.writeBigEndian((uchar)(output >>  8));
   outFile.writeBigEndian((uchar)(output >>  4));
}


void soundWriteSample24L_L(FileIO& outFile, sampleType aSample) {
   static long output;
   output = SampleToLong(aSample);
   outFile.writeBigEndian((uchar)(output >>  4));
   outFile.writeBigEndian((uchar)(output >>  8));
   outFile.writeBigEndian((uchar)(output >> 12));
}



//////////////////////////////
//
// soundWriteSample32L
//

void soundWriteSample32L_B(FileIO& outFile, sampleType aSample) {
   outFile.writeBigEndian(SampleToLong(aSample));
}


void soundWriteSample32L_L(FileIO& outFile, sampleType aSample) {
   outFile.writeBigEndian(SampleToLong(aSample));
}



//////////////////////////////
//
// soundWriteSample32F
//

void soundWriteSample32F_B(FileIO& outFile, sampleType aSample) {
   outFile.writeBigEndian((float)aSample);
}


void soundWriteSample32F_L(FileIO& outFile, sampleType aSample) {
   outFile.writeLittleEndian((float)aSample);
}



//////////////////////////////
//
// soundWriteSample64F
//

void soundWriteSample64F_B(FileIO& outFile, sampleType aSample) {
   outFile.writeBigEndian((double)aSample);
}


void soundWriteSample64F_L(FileIO& outFile, sampleType aSample) {
   outFile.writeLittleEndian((double)aSample);
}



//////////////////////////////
//
// linear16ToMulaw8
//
// reference: http://www.omnigroup.com/MailArchive/next-prog/1994/0042.html
//

#define BIAS 0x84 /* define the add-in bias for 16 bit samples */
#define CLIP 32635

uchar linear16ToMulaw8(short aSample) {

   static int exponentLookupTable[256] = {
      0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,
      4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
      5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
      5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
      6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
      6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
      6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
      6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
   };

   static int sign, exponent, mantissa, magnitude;
   static uchar output;

   // put the sample into sign-magnitude form
   sign = (aSample >> 8) & 0x80;                  // set aside the sign 
   magnitude = sign ? -aSample : aSample;         // get magnitude
   if (magnitude > CLIP) magnitude = CLIP;        // clip the magnitude 

   // Convert from 16 bit linear to ulaw. 
   magnitude += BIAS;
   exponent   = exponentLookupTable[(uchar)((magnitude >> 7) & 0xFF)];
   mantissa   = (magnitude >> (exponent + 3)) & 0x0F;
   output     = ~(sign | (exponent << 4) | mantissa);
   
   return output;
}



// md5sum:	028a08957b4c15f9a94e3a506b9ae48d  SoundFileOut.cpp
