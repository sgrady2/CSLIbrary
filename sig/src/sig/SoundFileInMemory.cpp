//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Mar  9 08:05:31 PST 1999
// Last Modified: Tue Mar  9 08:05:34 PST 1999
// Filename:      ...sig/code/Generator/SoundFileInMemory/SoundFileInMemory.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#include "SoundFileInMemory.h"
#include <iostream.h>
#include <stdlib.h>
#include <string.h>


int SoundFileInMemory::memoryUsage = 0;


//////////////////////////////
//
// SoundFileInMemory::SoundFileInMemory --
//    defaults: aFileName = "", start = NONE, dur = NONE
//

SoundFileInMemory::SoundFileInMemory(char* aFileName, float start, float dur) {
   setName("SoundFileInMemory");

   soundData = NULL;
   dataPointer = -1;
   soundDataSize = 0;
   borrowedQ = 0;

   filename = NULL;
   maxSampleIndex = -1;
   curSampleIndex = 0;
   begSampleIndex = 0;
   
   
   if (strlen(aFileName) > 0) {
      setFile(aFileName, start, dur);

      // set the size of the output array to the number of channels
      Output.setSize(sndHeader.getChannels());
      Output.allowGrowth(0);
   } else {
      Output.setSize(4);
      Output.allowGrowth(1);
   }
   Output.zero();

   runningQ = 1;
   loopingQ = 0;
  
   brandname = GENERATOR;
}



//////////////////////////////
//
// SoundFileInMemory::~SoundFileInMemory --
//

SoundFileInMemory::~SoundFileInMemory() { 
   if (filename != NULL) {
      delete [] filename;
      filename = NULL;
   }

   if (borrowedQ != 1 && soundData != NULL) {
      memoryUsage -= soundDataSize;
      delete [] soundData;
      soundDataSize = 0;
   }
}



//////////////////////////////
//
// SoundFileInMemory::action --
//

void SoundFileInMemory::action(void) {
   if (!runningQ) return;

   // read the soundfile samples into the output array

   if (curSampleIndex < 0 || curSampleIndex > maxSampleIndex) {
      for (int i=0; i<getChannels(); i++) {
         Output[i] = 0;
      }
      return;
   }

   fillOutputArray();

   curSampleIndex++;
   if (curSampleIndex > endSampleIndex && loopingQ) {
      curSampleIndex = begSampleIndex;
   }

}



//////////////////////////////
//
// SoundFileInMemory::borrow -- Borrow data from another 
//    similar object
//

void SoundFileInMemory::borrow(const SoundFileInMemory& aFile) {
   if (borrowedQ == 0 && soundData != NULL) {
      memoryUsage -= soundDataSize;
      delete [] soundData;
      soundData = NULL;
   }

   soundData = aFile.soundData;
   dataPointer = 0;
   soundDataSize = aFile.soundDataSize;
   borrowedQ = 1;
   sndHeader = aFile.sndHeader;
   if (filename != NULL) {
      delete [] filename;
      filename = NULL;
   }

   filename = new char[strlen(aFile.filename) + 1];
   strcpy(filename, aFile.filename);

   endianType = aFile.endianType;
   readSample = aFile.readSample;

}



//////////////////////////////
//
// SoundFileInMemory::extractSample16Bit -- reads a sample from the
//	soundfile.  The soundfile must be in 16 bit format or else
//	unreported errors will occur.  Also, you cannot use the
//	action function after calling this function since this
//	function does not keep track of the number of samples read
//	from the file.  This function is used only to extract the
//	integer sample values without converting them to floating
//	point samples.
//

short SoundFileInMemory::extractSample16Bit(void) {
   short sample;
   switch (endianType) {
      case SIG_BIG_ENDIAN:
         readBigEndian(sample);
         break;
      case SIG_LITTLE_ENDIAN:
         readLittleEndian(sample);
         break;
   }

   return sample;
}



//////////////////////////////
//
// SoundFileInMemory::getBitsPerSample --
//

int SoundFileInMemory::getBitsPerSample(void) {
   return sndHeader.getBitsPerSample();
}



//////////////////////////////
//
// SoundFileInMemory::getChannels --
//

int SoundFileInMemory::getChannels(void) {
   return sndHeader.getChannels();
}



//////////////////////////////
//
// SoundFileInMemory::getFilename -- 
//

const char* SoundFileInMemory::getFilename(void) const {
   if (filename == NULL) {
      return "";
   } else {
      return filename;
   }
}



//////////////////////////////
//
// SoundFileInMemory::getMemoryUsage -- return the number of bytes
//   being used for all SoundFileInMemory objects.
//

int SoundFileInMemory::getMemoryUsage(void) {
   return memoryUsage;
}


//////////////////////////////
//
// SoundFileInMemory::getHeader --
//

SoundHeader& SoundFileInMemory::getHeader(void) {
   return sndHeader;
}



//////////////////////////////
//
// SoundFileInMemory::getSamples  -- returns the number of samples
//	in the soundfile.  If a soundfile has more than one
//	channel, then all of the samples occuring at the
//	same time are considered to be ONE sample according
//	to the counting convention of this function.
//	In other words, if you know the sample rate for
//	the sound file, then dividing the count returned
//	by this function by the sampling rate will return the
//	duration of the soundfile.
//

long SoundFileInMemory::getSamples(void) {
   return maxSampleIndex + 1;
}



//////////////////////////////
//
// SoundFileInMemory::output --
//     default: channel = 0
//

sampleType SoundFileInMemory::output(int channel) {
   return Output[channel];
}



//////////////////////////////
//
// SoundFileInMemory::ownership -- returns true if this object
//     owns the sound memory
//

int SoundFileInMemory::ownership(void) const {
   return !borrowedQ;
}



//////////////////////////////
//
// SoundFileInMemory::printState --
//

void SoundFileInMemory::printState(void) {
   int oldFlag = sndHeader.setOutputType(TYPE_TEXT);
   cerr << "Filename: " << filename << endl;
   cerr << sndHeader;
   sndHeader.setOutputType(oldFlag);
}



//////////////////////////////
//
// SoundFileInMemory::reset --
//     default: sampleIndex = 0
//

void SoundFileInMemory::reset(int sampleIndex) {
   dataPointer = 0;

   if (sampleIndex < 0) {
      sndfile.seekp(sndHeader.getDataByteOffset()-1); 
   } else {
      sndfile.seekp((int)(sndHeader.getDataByteOffset() - 1 + 
         (sampleIndex + begSampleIndex) * sndHeader.getChannels() *
          sndHeader.getBitsPerSample() / 8.0));
   }
}



//////////////////////////////
//
// SoundFileInMemory::setFile --
//

void SoundFileInMemory::setFile(char* aFileName, float start, float dur) {
   if (filename != NULL) delete [] filename;
   filename = new char[strlen(aFileName)+1];
   strcpy(filename, aFileName);

   sndHeader.setHeader(filename);

   if (sndfile.is_open()) sndfile.close();
   #ifdef VISUAL      /* for stupid LF-CR prevention in DOS */
      sndfile.open(filename, ios::in | ios::binary);
   #else 
      sndfile.open(filename, ios::in);
   #endif
   if(!sndfile.is_open()) {
      cerr << "Error: sound file " << filename 
           << " could not be opened!" << endl;
      exit(1);
   }

   // determine the beginning and ending samples according to start and dur
   maxSampleIndex = sndHeader.getSamples() - 1; 
   if (maxSampleIndex < 0) {
      cerr << "Error: sound file " << filename << " has no data!" << endl;
      exit(1);
   }
   if (start >= 0) {
      begSampleIndex = (long)(start * sndHeader.getSrate());
   } else {
      begSampleIndex = 0;
   }
   if (begSampleIndex > maxSampleIndex) begSampleIndex = 0;
   if (dur >= 0) {
      endSampleIndex = begSampleIndex + (long)(dur * sndHeader.getSrate());
   } else {
      endSampleIndex = maxSampleIndex;
   }
   if (endSampleIndex > maxSampleIndex) {
      endSampleIndex = maxSampleIndex;
   }
 
   curSampleIndex = begSampleIndex;

   // set the file to the current sample
   
   sndfile.seekp((long)(sndHeader.getDataByteOffset() + curSampleIndex * 
      sndHeader.getChannels() * sndHeader.getBitsPerSample() / 8.0));

   // now load the samples into the soundData storage array in memory


   if (borrowedQ == 0 && soundData != NULL) {
      delete [] soundData;
      memoryUsage -= soundDataSize;
      if (memoryUsage < 0) {
         cout << "Warning in SoundFileInMemory: Weird memory management" 
              << endl;
      }
   }

   if (borrowedQ == 1) {
      borrowedQ = 0;
   }

   soundDataSize = getSamples() * getChannels() * (getBitsPerSample()/8);
cout << "SoundDataSize = " << soundDataSize << endl;

   soundData = new char[soundDataSize];
   sndfile.read(soundData, soundDataSize);
   dataPointer = 0;

   memoryUsage += soundDataSize;

   determineSampleReadingFunction();
}





///////////////////////////////////////////////////////////////////////////
//
// private functions
//



// The following functions are used by the readSample function.
// These functions have to be outside of the SoundFileInMemory class
// because Microsoft's Visual C++ 5.0 compiler cannot handle pointers
// to class member functions.

// big endian samples
sampleType         SMEM_soundReadSample8M_B        (SoundFileInMemory& x);
sampleType         SMEM_soundReadSample8L_B        (SoundFileInMemory& x);
sampleType         SMEM_soundReadSample16L_B       (SoundFileInMemory& x);
sampleType         SMEM_soundReadSample24L_B       (SoundFileInMemory& x);
sampleType         SMEM_soundReadSample32L_B       (SoundFileInMemory& x);
sampleType         SMEM_soundReadSample32F_B       (SoundFileInMemory& x);
sampleType         SMEM_soundReadSample64F_B       (SoundFileInMemory& x);

// little endian samples
sampleType        SMEM_soundReadSample8M_L         (SoundFileInMemory& x);
sampleType        SMEM_soundReadSample8L_L         (SoundFileInMemory& x);
sampleType        SMEM_soundReadSample16L_L        (SoundFileInMemory& x);
sampleType        SMEM_soundReadSample24L_L        (SoundFileInMemory& x);
sampleType        SMEM_soundReadSample32L_L        (SoundFileInMemory& x);
sampleType        SMEM_soundReadSample32F_L        (SoundFileInMemory& x);
sampleType        SMEM_soundReadSample64F_L        (SoundFileInMemory& x);



//////////////////////////////
//
// SoundFileInMemory::determineSampleReadingFunction --
//

void SoundFileInMemory::determineSampleReadingFunction(void) {
   switch (sndHeader.getInputType()) {
      case TYPE_SND:                        // big endian samples
         endianType = SIG_BIG_ENDIAN;
         switch (sndHeader.getNextFormat()) {
            case SND_FORMAT_MULAW_8:
               readSample = SMEM_soundReadSample8M_B;
               break;
            case SND_FORMAT_LINEAR_8:
               readSample = SMEM_soundReadSample8L_B;
               break;
            case SND_FORMAT_LINEAR_16:
               readSample = SMEM_soundReadSample16L_B;
               break;
            case SND_FORMAT_LINEAR_24:
               readSample = SMEM_soundReadSample24L_B;
               break;
            case SND_FORMAT_LINEAR_32:
               readSample = SMEM_soundReadSample32L_B;
               break;
            case SND_FORMAT_FLOAT:
               readSample = SMEM_soundReadSample32F_B;
               break;
            case SND_FORMAT_DOUBLE:
               readSample = SMEM_soundReadSample64F_B;
               break;
            default:
               cerr << "Error: unknown input soundfile format: " 
                    << sndHeader.getNextFormat() << endl;
               exit(1);
         }
         break;
      case TYPE_WAV_PCM:                    // little endian samples
         endianType = SIG_LITTLE_ENDIAN;
         switch (sndHeader.getNextFormat()) {
            case SND_FORMAT_MULAW_8:
               readSample = SMEM_soundReadSample8M_L;
               break;
            case SND_FORMAT_LINEAR_8:
               readSample = SMEM_soundReadSample8L_L;
               break;
            case SND_FORMAT_LINEAR_16:
               readSample = SMEM_soundReadSample16L_L;
               break;
            case SND_FORMAT_LINEAR_24:
               readSample = SMEM_soundReadSample24L_L;
               break;
            case SND_FORMAT_LINEAR_32:
               readSample = SMEM_soundReadSample32L_L;
               break;
            case SND_FORMAT_FLOAT:
               readSample = SMEM_soundReadSample32F_L;
               break;
            case SND_FORMAT_DOUBLE:
               readSample = SMEM_soundReadSample64F_L;
               break;
            default:
               cerr << "Error: unknown input soundfile format: " 
                    << sndHeader.getNextFormat() << endl;
               exit(1);
         }
         break;
      default: ;
         // the soundfile name is probably a dummy, and will not be used
         // cerr << "Error: unknown soundfile type: "
         //      << sndHeader.getInputType() << endl;
         // exit(1);
   }
}



//////////////////////////////
//
// SoundFileInMemory::fillOutputArray --
//

void SoundFileInMemory::fillOutputArray(void) {
   for (int i=0; i<sndHeader.getChannels(); i++) {
      Output[i] = readSample(*this);
   }
}



///////////////////////////////////////////////////////////////////////////
//
// Helper functions which are intended to be private
// These functions have to be outside of the SoundFileInMemory class
// because Microsoft's Visual C++ 5.0 compiler cannot handle pointers
// to class member functions.
//



//////////////////////////////
//
// SoundFileInMemory::mulaw8ToLinear16 -- converts 8-bit mulaw to 
//	16-bit linear quantization
//

short SoundFileInMemory::mulaw8ToLinear16(uchar index) {

   static const short MuLaw[256] = {
   // negative values
    (short)0x8284, (short)0x8684, (short)0x8a84, (short)0x8e84, 
    (short)0x9284, (short)0x9684, (short)0x9a84, (short)0x9e84, 
    (short)0xa284, (short)0xa684, (short)0xaa84, (short)0xae84, 
    (short)0xb284, (short)0xb684, (short)0xba84, (short)0xbe84, 
    (short)0xc184, (short)0xc384, (short)0xc584, (short)0xc784, 
    (short)0xc984, (short)0xcb84, (short)0xcd84, (short)0xcf84, 
    (short)0xd184, (short)0xd384, (short)0xd584, (short)0xd784, 
    (short)0xd984, (short)0xdb84, (short)0xdd84, (short)0xdf84, 
    (short)0xe104, (short)0xe204, (short)0xe304, (short)0xe404, 
    (short)0xe504, (short)0xe604, (short)0xe704, (short)0xe804, 
    (short)0xe904, (short)0xea04, (short)0xeb04, (short)0xec04, 
    (short)0xed04, (short)0xee04, (short)0xef04, (short)0xf004, 
    (short)0xf0c4, (short)0xf144, (short)0xf1c4, (short)0xf244, 
    (short)0xf2c4, (short)0xf344, (short)0xf3c4, (short)0xf444, 
    (short)0xf4c4, (short)0xf544, (short)0xf5c4, (short)0xf644, 
    (short)0xf6c4, (short)0xf744, (short)0xf7c4, (short)0xf844, 
    (short)0xf8a4, (short)0xf8e4, (short)0xf924, (short)0xf964, 
    (short)0xf9a4, (short)0xf9e4, (short)0xfa24, (short)0xfa64, 
    (short)0xfaa4, (short)0xfae4, (short)0xfb24, (short)0xfb64, 
    (short)0xfba4, (short)0xfbe4, (short)0xfc24, (short)0xfc64, 
    (short)0xfc94, (short)0xfcb4, (short)0xfcd4, (short)0xfcf4, 
    (short)0xfd14, (short)0xfd34, (short)0xfd54, (short)0xfd74, 
    (short)0xfd94, (short)0xfdb4, (short)0xfdd4, (short)0xfdf4, 
    (short)0xfe14, (short)0xfe34, (short)0xfe54, (short)0xfe74, 
    (short)0xfe8c, (short)0xfe9c, (short)0xfeac, (short)0xfebc, 
    (short)0xfecc, (short)0xfedc, (short)0xfeec, (short)0xfefc, 
    (short)0xff0c, (short)0xff1c, (short)0xff2c, (short)0xff3c, 
    (short)0xff4c, (short)0xff5c, (short)0xff6c, (short)0xff7c, 
    (short)0xff88, (short)0xff90, (short)0xff98, (short)0xffa0, 
    (short)0xffa8, (short)0xffb0, (short)0xffb8, (short)0xffc0, 
    (short)0xffc8, (short)0xffd0, (short)0xffd8, (short)0xffe0, 
    (short)0xffe8, (short)0xfff0, (short)0xfff8, (short)0x0000, 
   // positive values
    (short)0x7d7c, (short)0x797c, (short)0x757c, (short)0x717c, 
    (short)0x6d7c, (short)0x697c, (short)0x657c, (short)0x617c, 
    (short)0x5d7c, (short)0x597c, (short)0x557c, (short)0x517c, 
    (short)0x4d7c, (short)0x497c, (short)0x457c, (short)0x417c, 
    (short)0x3e7c, (short)0x3c7c, (short)0x3a7c, (short)0x387c, 
    (short)0x367c, (short)0x347c, (short)0x327c, (short)0x307c, 
    (short)0x2e7c, (short)0x2c7c, (short)0x2a7c, (short)0x287c, 
    (short)0x267c, (short)0x247c, (short)0x227c, (short)0x207c, 
    (short)0x1efc, (short)0x1dfc, (short)0x1cfc, (short)0x1bfc, 
    (short)0x1afc, (short)0x19fc, (short)0x18fc, (short)0x17fc, 
    (short)0x16fc, (short)0x15fc, (short)0x14fc, (short)0x13fc, 
    (short)0x12fc, (short)0x11fc, (short)0x10fc, (short)0x0ffc, 
    (short)0x0f3c, (short)0x0ebc, (short)0x0e3c, (short)0x0dbc, 
    (short)0x0d3c, (short)0x0cbc, (short)0x0c3c, (short)0x0bbc, 
    (short)0x0b3c, (short)0x0abc, (short)0x0a3c, (short)0x09bc, 
    (short)0x093c, (short)0x08bc, (short)0x083c, (short)0x07bc, 
    (short)0x075c, (short)0x071c, (short)0x06dc, (short)0x069c, 
    (short)0x065c, (short)0x061c, (short)0x05dc, (short)0x059c, 
    (short)0x055c, (short)0x051c, (short)0x04dc, (short)0x049c, 
    (short)0x045c, (short)0x041c, (short)0x03dc, (short)0x039c, 
    (short)0x036c, (short)0x034c, (short)0x032c, (short)0x030c, 
    (short)0x02ec, (short)0x02cc, (short)0x02ac, (short)0x028c, 
    (short)0x026c, (short)0x024c, (short)0x022c, (short)0x020c, 
    (short)0x01ec, (short)0x01cc, (short)0x01ac, (short)0x018c, 
    (short)0x0174, (short)0x0164, (short)0x0154, (short)0x0144, 
    (short)0x0134, (short)0x0124, (short)0x0114, (short)0x0104, 
    (short)0x00f4, (short)0x00e4, (short)0x00d4, (short)0x00c4, 
    (short)0x00b4, (short)0x00a4, (short)0x0094, (short)0x0084, 
    (short)0x0078, (short)0x0070, (short)0x0068, (short)0x0060, 
    (short)0x0058, (short)0x0050, (short)0x0048, (short)0x0040, 
    (short)0x0038, (short)0x0030, (short)0x0028, (short)0x0020, 
    (short)0x0018, (short)0x0010, (short)0x0008, (short)0x0000
   };

   return MuLaw[index];
}



//////////////////////////////
//
// SMEM_soundReadSample8M -- read channel samples as
//	8-bit Mulaw values.  Both of the following
//	functions do exactly the same thing.
//

sampleType SMEM_soundReadSample8M_B(SoundFileInMemory& x) {
   uchar sample;
   x.readBigEndian(sample);
   return (sampleType)SoundFileInMemory::mulaw8ToLinear16(sample) / 
         (long)0x8000;
}


sampleType SMEM_soundReadSample8M_L(SoundFileInMemory& x) {
   uchar sample;
   x.readLittleEndian(sample);
   return (sampleType)SoundFileInMemory::mulaw8ToLinear16(sample) / 
         (long)0x8000;
}



//////////////////////////////
//
// SMEM_soundReadSample8L -- both functions do exactly the same thing
//

sampleType SMEM_soundReadSample8L_B(SoundFileInMemory& x) {
   char sample;
   x.readBigEndian(sample);
   return (sampleType)sample / (long)0x80;
}

sampleType SMEM_soundReadSample8L_L(SoundFileInMemory& x) {
   char sample;
   x.readLittleEndian(sample);
   return (sampleType)sample / (long)0x80;
}



//////////////////////////////
//
// SMEM_soundReadSample16L --
//
 
sampleType SMEM_soundReadSample16L_B(SoundFileInMemory& x) {
   short sample;
   x.readBigEndian(sample);
   return (sampleType)sample / (long)0x8000;
}

 
sampleType SMEM_soundReadSample16L_L(SoundFileInMemory& x) {
   short sample;
   x.readLittleEndian(sample);
   return (sampleType)sample / (long)0x8000;
}



//////////////////////////////
//
// SMEM_soundReadSample24L --
//

sampleType SMEM_soundReadSample24L_B(SoundFileInMemory& x) {
   char temp[3];
   long sample;
   x.readLittleEndian(temp[0]);
   x.readLittleEndian(temp[1]);
   x.readLittleEndian(temp[2]);
   sample = temp[0];
   sample = (sample << 8) | temp[1];
   sample = (sample << 8) | temp[2];
   return (sampleType)sample / (long)0x800000;
}


sampleType SMEM_soundReadSample24L_L(SoundFileInMemory& x) {
   char temp[3];
   long sample;
   x.readLittleEndian(temp[2]);
   x.readLittleEndian(temp[1]);
   x.readLittleEndian(temp[0]);
   sample = temp[0];
   sample = (sample << 8) | temp[1];
   sample = (sample << 8) | temp[2];
   return (sampleType)sample / (long)0x800000;
}



//////////////////////////////
//
// SMEM_soundReadSample32L --
//

sampleType SMEM_soundReadSample32L_B(SoundFileInMemory& x) {
   long sample;
   x.readBigEndian(sample);
   return (sampleType)sample / (ulong)0x80000000;
}


sampleType SMEM_soundReadSample32L_L(SoundFileInMemory& x) {
   long sample;
   x.readLittleEndian(sample);
   return (sampleType)sample / (ulong)0x80000000;
}



//////////////////////////////
//
// SMEM_soundReadSample32F --
//

sampleType SMEM_soundReadSample32F_B(SoundFileInMemory& x) {
   float sample;
   x.readBigEndian(sample);
   return (sampleType)sample;
}


sampleType SMEM_soundReadSample32F_L(SoundFileInMemory& x) {
   float sample;
   x.readLittleEndian(sample);
   return (sampleType)sample;
}



//////////////////////////////
//
// SMEM_soundReadSample64F --
//


sampleType SMEM_soundReadSample64F_B(SoundFileInMemory& x) {
   double sample;
   x.readBigEndian(sample);
   return (sampleType)sample;
}


sampleType SMEM_soundReadSample64F_L(SoundFileInMemory& x) {
   double sample;
   x.readLittleEndian(sample);
   return (sampleType)sample;
}



///////////////////////////////////////////////////////////////////////////
//
// FileIO functions used to read character arrays instead
//

//////////////////////////////
//
// SoundFileInMemory::readBigEndian --
//	Read numbers from a file as big endian
//

void SoundFileInMemory::readBigEndian(char& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}

void SoundFileInMemory::readBigEndian(uchar& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}

void SoundFileInMemory::readBigEndian(short& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}

void SoundFileInMemory::readBigEndian(ushort& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}

void SoundFileInMemory::readBigEndian(long& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}

void SoundFileInMemory::readBigEndian(ulong& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}

void SoundFileInMemory::readBigEndian(int& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}

void SoundFileInMemory::readBigEndian(uint& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}

void SoundFileInMemory::readBigEndian(float& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}

void SoundFileInMemory::readBigEndian(double& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}



//////////////////////////////
//
// SoundFileInMemory::readLittleEndian --
//	Read numbers from a file as little endian
//

void SoundFileInMemory::readLittleEndian(char& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}

void SoundFileInMemory::readLittleEndian(uchar& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}

void SoundFileInMemory::readLittleEndian(short& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}

void SoundFileInMemory::readLittleEndian(ushort& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}

void SoundFileInMemory::readLittleEndian(long& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}

void SoundFileInMemory::readLittleEndian(ulong& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}

void SoundFileInMemory::readLittleEndian(int& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}

void SoundFileInMemory::readLittleEndian(uint& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}

void SoundFileInMemory::readLittleEndian(float& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}

void SoundFileInMemory::readLittleEndian(double& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}



//////////////////////////////
//
// SoundFileInMemory::readMachineEndian --
//	Read numbers from a file in the same endian as the computer.
//

void SoundFileInMemory::readMachineEndian(char& aNumber) {
   storageRead(aNumber);
}

void SoundFileInMemory::readMachineEndian(uchar& aNumber) {
   storageRead(aNumber);
}

void SoundFileInMemory::readMachineEndian(short& aNumber) {
   storageRead(aNumber);
}

void SoundFileInMemory::readMachineEndian(ushort& aNumber) {
   storageRead(aNumber);
}

void SoundFileInMemory::readMachineEndian(long& aNumber) {
   storageRead(aNumber);
}

void SoundFileInMemory::readMachineEndian(ulong& aNumber) {
   storageRead(aNumber);
}

void SoundFileInMemory::readMachineEndian(int& aNumber) {
   storageRead(aNumber);
}

void SoundFileInMemory::readMachineEndian(uint& aNumber) {
   storageRead(aNumber);
}

void SoundFileInMemory::readMachineEndian(float& aNumber) {
   storageRead(aNumber);
}

void SoundFileInMemory::readMachineEndian(double& aNumber) {
   storageRead(aNumber);
}



//////////////////////////////
//
// SoundFileInMemory::readNotMachineEndian --
//	Read numbers from a file with different endian from the computer.
//

void SoundFileInMemory::readNotMachineEndian(char& aNumber) {
   storageRead(aNumber);
   aNumber = flipBytes(aNumber);
}

void SoundFileInMemory::readNotMachineEndian(uchar& aNumber) {
   storageRead(aNumber);
   aNumber = flipBytes(aNumber);
}

void SoundFileInMemory::readNotMachineEndian(short& aNumber) {
   storageRead(aNumber);
   aNumber = flipBytes(aNumber);
}

void SoundFileInMemory::readNotMachineEndian(ushort& aNumber) {
   storageRead(aNumber);
   aNumber = flipBytes(aNumber);
}

void SoundFileInMemory::readNotMachineEndian(long& aNumber) {
   storageRead(aNumber);
   aNumber = flipBytes(aNumber);
}

void SoundFileInMemory::readNotMachineEndian(ulong& aNumber) {
   storageRead(aNumber);
   aNumber = flipBytes(aNumber);
}

void SoundFileInMemory::readNotMachineEndian(int& aNumber) {
   storageRead(aNumber);
   aNumber = flipBytes(aNumber);
}

void SoundFileInMemory::readNotMachineEndian(uint& aNumber) {
   storageRead(aNumber);
   aNumber = flipBytes(aNumber);
}

void SoundFileInMemory::readNotMachineEndian(float& aNumber) {
   storageRead(aNumber);
   aNumber = flipBytes(aNumber);
}

void SoundFileInMemory::readNotMachineEndian(double& aNumber) {
   storageRead(aNumber);
   aNumber = flipBytes(aNumber);
}



//////////////////////////////
//
// SoundFileInMemory::storageRead --
//

void SoundFileInMemory::storageRead(char& aNumber) {
   if (dataPointer >= soundDataSize) {
      aNumber = '\0';
   }
   
   aNumber = soundData[dataPointer];
   dataPointer += 1;
}

void SoundFileInMemory::storageRead(uchar& aNumber) {
   if (dataPointer >= soundDataSize) {
      aNumber = '\0';
   }
   
   aNumber = (uchar)soundData[dataPointer];
   dataPointer += 1;
}

void SoundFileInMemory::storageRead(short& aNumber) {
   if (dataPointer >= soundDataSize - 1) {
      aNumber = 0;
   }

   aNumber = (uchar)soundData[dataPointer];
   dataPointer += 1;
   aNumber = aNumber << 8;
   aNumber |= (uchar)soundData[dataPointer];
   dataPointer += 1;
}

void SoundFileInMemory::storageRead(ushort& aNumber) {
   if (dataPointer >= soundDataSize - 1) {
      aNumber = 0;
   }

   aNumber = (uchar)soundData[dataPointer];
   dataPointer += 1;
   aNumber = aNumber << 8;
   aNumber |= (uchar)soundData[dataPointer];
   dataPointer += 1;
}

void SoundFileInMemory::storageRead(long& aNumber) {
   if (dataPointer >= soundDataSize - 3) {
      aNumber = 0;
   }

   aNumber = (uchar)soundData[dataPointer];
   dataPointer += 1;
   aNumber = aNumber << 8;
   aNumber |= (uchar)soundData[dataPointer];
   dataPointer += 1;
   aNumber = aNumber << 8;
   aNumber |= (uchar)soundData[dataPointer];
   dataPointer += 1;
   aNumber = aNumber << 8;
   aNumber |= (uchar)soundData[dataPointer];
   dataPointer += 1;
}

void SoundFileInMemory::storageRead(ulong& aNumber) {
   if (dataPointer >= soundDataSize - 3) {
      aNumber = 0;
   }

   aNumber = (uchar)soundData[dataPointer];
   dataPointer += 1;
   aNumber = aNumber << 8;
   aNumber |= (uchar)soundData[dataPointer];
   dataPointer += 1;
   aNumber = aNumber << 8;
   aNumber |= (uchar)soundData[dataPointer];
   dataPointer += 1;
   aNumber = aNumber << 8;
   aNumber |= (uchar)soundData[dataPointer];
   dataPointer += 1;
}

void SoundFileInMemory::storageRead(int& aNumber) {
   if (dataPointer >= soundDataSize - 3) {
      aNumber = 0;
   }

   aNumber = (uchar)soundData[dataPointer];
   dataPointer += 1;
   aNumber = aNumber << 8;
   aNumber |= (uchar)soundData[dataPointer];
   dataPointer += 1;
   aNumber = aNumber << 8;
   aNumber |= (uchar)soundData[dataPointer];
   dataPointer += 1;
   aNumber = aNumber << 8;
   aNumber |= (uchar)soundData[dataPointer];
   dataPointer += 1;
}

void SoundFileInMemory::storageRead(uint& aNumber) {
   if (dataPointer >= soundDataSize - 3) {
      aNumber = 0;
   }

   aNumber = (uchar)soundData[dataPointer];
   dataPointer += 1;
   aNumber = aNumber << 8;
   aNumber |= (uchar)soundData[dataPointer];
   dataPointer += 1;
   aNumber = aNumber << 8;
   aNumber |= (uchar)soundData[dataPointer];
   dataPointer += 1;
   aNumber = aNumber << 8;
   aNumber |= (uchar)soundData[dataPointer];
   dataPointer += 1;
}


void SoundFileInMemory::storageRead(float& aNumber) {
   if (dataPointer >= soundDataSize - 7) {
      aNumber = 0;
   }
   char* fpointer = (char*)&aNumber;

   fpointer[0] = (uchar)soundData[dataPointer];
   dataPointer += 1;
   fpointer[1] = (uchar)soundData[dataPointer];
   dataPointer += 1;
   fpointer[2] = (uchar)soundData[dataPointer];
   dataPointer += 1;
   fpointer[3] = (uchar)soundData[dataPointer];
   dataPointer += 1;
}

void SoundFileInMemory::storageRead(double& aNumber) {
   if (dataPointer >= soundDataSize - 7) {
      aNumber = 0;
   }
   char* fpointer = (char*)&aNumber;

   fpointer[0] = (uchar)soundData[dataPointer];
   dataPointer += 1;
   fpointer[1] = (uchar)soundData[dataPointer];
   dataPointer += 1;
   fpointer[2] = (uchar)soundData[dataPointer];
   dataPointer += 1;
   fpointer[3] = (uchar)soundData[dataPointer];
   dataPointer += 1;
   fpointer[4] = (uchar)soundData[dataPointer];
   dataPointer += 1;
   fpointer[5] = (uchar)soundData[dataPointer];
   dataPointer += 1;
   fpointer[6] = (uchar)soundData[dataPointer];
   dataPointer += 1;
   fpointer[7] = (uchar)soundData[dataPointer];
   dataPointer += 1;
}



///////////////////////////////////////////////////////////////////////////
// 
// private functions
//


//////////////////////////////
//
// SoundFileInMemory::flipBytes -- flip the bytes in a number
//

char SoundFileInMemory::flipBytes(char aNumber) {
   return aNumber;
}


uchar SoundFileInMemory::flipBytes(uchar aNumber) {
   return aNumber;
}


short SoundFileInMemory::flipBytes(short aNumber) {
   static uchar output[2];
   static uchar* input;
   input = (uchar*)(&aNumber);

   output[0] = input[1];
   output[1] = input[0];

   return *((short*)(&output));
}


ushort SoundFileInMemory::flipBytes(ushort aNumber) {
   static uchar output[2];
   static uchar* input;
   input = (uchar*)(&aNumber);

   output[0] = input[1];
   output[1] = input[0];
 
   return *((ushort*)(&output));
}


long SoundFileInMemory::flipBytes(long aNumber) {
   static uchar output[4];
   static uchar* input;
   input = (uchar*)(&aNumber);

   output[0] = input[3];
   output[1] = input[2];
   output[2] = input[1];
   output[3] = input[0];

   return *((long*)(&output));
}


ulong SoundFileInMemory::flipBytes(ulong aNumber) {
   static uchar output[4];
   static uchar* input;
   input = (uchar*)(&aNumber);

   output[0] = input[3];
   output[1] = input[2];
   output[2] = input[1];
   output[3] = input[0];

   return *((ulong*)(&output));
}


int SoundFileInMemory::flipBytes(int aNumber) {
   static uchar output[sizeof(uint)];
   static uchar* input;
   input = (uchar*)(&aNumber);

   for(uint i=0; i<sizeof(int); i++) {
      output[i] = input[sizeof(int)-1-i];
   }

   return *((int*)(&output));
}


uint SoundFileInMemory::flipBytes(uint aNumber) {
   static uchar output[sizeof(uint)];
   static uchar* input;
   input = (uchar*)(&aNumber);

   for(uint i=0; i<sizeof(uint); i++) {
      output[i] = input[sizeof(uint)-1-i];
   }

   return *((uint*)(&output));
}

 
   
float SoundFileInMemory::flipBytes(float aNumber) {
   static uchar output[4];
   static uchar* input;
   input = (uchar*)(&aNumber);

   output[0] = input[3];
   output[1] = input[2];
   output[2] = input[1];
   output[3] = input[0];

   return *((float*)(&output));
}


double SoundFileInMemory::flipBytes(double aNumber) {
   static uchar output[8];
   static uchar* input;
   input = (uchar*)(&aNumber);

   output[0] = input[7];
   output[1] = input[6];
   output[2] = input[5];
   output[3] = input[4];
   output[4] = input[3];
   output[5] = input[2];
   output[6] = input[1];
   output[7] = input[0];

   return *((double*)(&output));
}


// md5sum:	ce3800565b79469cbdc3b2f145803afc  SoundFileInMemory.cpp
