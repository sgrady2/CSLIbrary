//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Nov  1 23:35:02 PST 1996
// Last Modified: Sat Apr  4 22:47:36 GMT-0800 1998
// Last Modified: Wed Mar 17 16:52:53 PST 1999
// Filename:      ...sig/code/Generator/SoundFileIn/SoundFileIn.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#include "SoundFileIn.h"
#include <iostream.h>
#include <stdlib.h>
#include <string.h>


//////////////////////////////
//
// SoundFileIn::SoundFileIn --
//    defaults: aFileName = "", start = NONE, dur = NONE
//

SoundFileIn::SoundFileIn(char* aFileName, float start, float dur) {
   setName("SoundFileIn");

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
// SoundFileIn::~SoundFileIn --
//

SoundFileIn::~SoundFileIn() { 
   delete [] filename;
}



//////////////////////////////
//
// SoundFileIn::action --
//

void SoundFileIn::action(void) {
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
// SoundFileIn::extractSample16Bit -- reads a sample from the
//	soundfile.  The soundfile must be in 16 bit format or else
//	unreported errors will occur.  Also, you cannot use the
//	action function after calling this function since this
//	function does not keep track of the number of samples read
//	from the file.  This function is used only to extract the
//	integer sample values without converting them to floating
//	point samples.
//

short SoundFileIn::extractSample16Bit(void) {
   short sample;
   switch (endianType) {
      case SIG_BIG_ENDIAN:
         sndfile.readBigEndian(sample);
         break;
      case SIG_LITTLE_ENDIAN:
         sndfile.readLittleEndian(sample);
         break;
   }

   return sample;
}



//////////////////////////////
//
// SoundFileIn::getBitsPerSample --
//

int SoundFileIn::getBitsPerSample(void) {
   return sndHeader.getBitsPerSample();
}



//////////////////////////////
//
// SoundFileIn::getChannels --
//

int SoundFileIn::getChannels(void) {
   return sndHeader.getChannels();
}



//////////////////////////////
//
// SoundFileIn::getFilename --
//

const char* SoundFileIn::getFilename(void) const {
   if (filename == NULL) {
      return "";
   } else {
      return filename;
   }
}



//////////////////////////////
//
// SoundFileIn::getHeader --
//

SoundHeader& SoundFileIn::getHeader(void) {
   return sndHeader;
}



//////////////////////////////
//
// SoundFileIn::getSamples -- returns the number of samples
//	in the soundfile.  If a soundfile has more than one
//	channel, then all of the samples occuring at the
//	same time are considered to be ONE sample according
//	to the counting convention of this function.
//	In other words, if you know the sample rate for
//	the sound file, then dividing the count returned
//	by this function by the sampling rate will return the
//	duration of the soundfile.
//

long SoundFileIn::getSamples(void) {
   return maxSampleIndex + 1;
}



//////////////////////////////
//
// SoundFileIn::output --
//     default: channel = 0
//

sampleType SoundFileIn::output(int channel) {
   return Output[channel];
}



//////////////////////////////
//
// SoundFileIn::printState --
//

void SoundFileIn::printState(void) {
   int oldFlag = sndHeader.setOutputType(TYPE_TEXT);
   cerr << "Filename: " << filename << endl;
   cerr << sndHeader;
   sndHeader.setOutputType(oldFlag);
}



//////////////////////////////
//
// SoundFileIn::reset --
//     default: sampleIndex = 0
//

void SoundFileIn::reset(int sampleIndex) {
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
// SoundFileIn::setFile --
//

void SoundFileIn::setFile(char* aFileName, float start, float dur) {
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

   determineSampleReadingFunction();
}





///////////////////////////////////////////////////////////////////////////
//
// private functions
//



// The following functions are used by the readSample function.
// These functions have to be outside of the SoundFileIn class
// because Microsoft's Visual C++ 5.0 compiler cannot handle pointers
// to class member functions.

// big endian samples
sampleType         soundReadSample8M_B         (FileIO& soundFile);
sampleType         soundReadSample8L_B         (FileIO& soundFile);
sampleType         soundReadSample16L_B        (FileIO& soundFile);
sampleType         soundReadSample24L_B        (FileIO& soundFile);
sampleType         soundReadSample32L_B        (FileIO& soundFile);
sampleType         soundReadSample32F_B        (FileIO& soundFile);
sampleType         soundReadSample64F_B        (FileIO& soundFile);

// little endian samples
sampleType        soundReadSample8M_L          (FileIO& soundFile);
sampleType        soundReadSample8L_L          (FileIO& soundFile);
sampleType        soundReadSample16L_L         (FileIO& soundFile);
sampleType        soundReadSample24L_L         (FileIO& soundFile);
sampleType        soundReadSample32L_L         (FileIO& soundFile);
sampleType        soundReadSample32F_L         (FileIO& soundFile);
sampleType        soundReadSample64F_L         (FileIO& soundFile);




//////////////////////////////
//
// SoundFileIn::determineSampleReadingFunction --
//

void SoundFileIn::determineSampleReadingFunction(void) {
   switch (sndHeader.getInputType()) {
      case TYPE_SND:                        // big endian samples
         endianType = SIG_BIG_ENDIAN;
         switch (sndHeader.getNextFormat()) {
            case SND_FORMAT_MULAW_8:
               readSample = soundReadSample8M_B;
               break;
            case SND_FORMAT_LINEAR_8:
               readSample = soundReadSample8L_B;
               break;
            case SND_FORMAT_LINEAR_16:
               readSample = soundReadSample16L_B;
               break;
            case SND_FORMAT_LINEAR_24:
               readSample = soundReadSample24L_B;
               break;
            case SND_FORMAT_LINEAR_32:
               readSample = soundReadSample32L_B;
               break;
            case SND_FORMAT_FLOAT:
               readSample = soundReadSample32F_B;
               break;
            case SND_FORMAT_DOUBLE:
               readSample = soundReadSample64F_B;
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
               readSample = soundReadSample8M_L;
               break;
            case SND_FORMAT_LINEAR_8:
               readSample = soundReadSample8L_L;
               break;
            case SND_FORMAT_LINEAR_16:
               readSample = soundReadSample16L_L;
               break;
            case SND_FORMAT_LINEAR_24:
               readSample = soundReadSample24L_L;
               break;
            case SND_FORMAT_LINEAR_32:
               readSample = soundReadSample32L_L;
               break;
            case SND_FORMAT_FLOAT:
               readSample = soundReadSample32F_L;
               break;
            case SND_FORMAT_DOUBLE:
               readSample = soundReadSample64F_L;
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
// SoundFileIn::fillOutputArray --
//

void SoundFileIn::fillOutputArray(void) {
   for (int i=0; i<sndHeader.getChannels(); i++) {
      Output[i] = readSample(sndfile);
   }
}



///////////////////////////////////////////////////////////////////////////
//
// Helper functions which are intended to be private
// These functions have to be outside of the SoundFileIn class
// because Microsoft's Visual C++ 5.0 compiler cannot handle pointers
// to class member functions.
//



//////////////////////////////
//
// mulaw8ToLinear16 -- converts 8-bit mulaw to 
//	16-bit linear quantization
//

short mulaw8ToLinear16(uchar index) {

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
// soundReadSample8M -- read channel samples as
//	8-bit Mulaw values.  Both of the following
//	functions do exactly the same thing.
//

sampleType soundReadSample8M_B(FileIO& soundfile) {
   uchar sample;
   soundfile.readBigEndian(sample);
   return (sampleType)mulaw8ToLinear16(sample) / (long)0x8000;
}


sampleType soundReadSample8M_L(FileIO& soundfile) {
   uchar sample;
   soundfile.readLittleEndian(sample);
   return (sampleType)mulaw8ToLinear16(sample) / (long)0x8000;
}



//////////////////////////////
//
// soundReadSample8L -- both functions do exactly the same thing
//

sampleType soundReadSample8L_B(FileIO& soundfile) {
   char sample;
   soundfile.readBigEndian(sample);
   return (sampleType)sample / (long)0x80;
}

sampleType soundReadSample8L_L(FileIO& soundfile) {
   char sample;
   soundfile.readLittleEndian(sample);
   return (sampleType)sample / (long)0x80;
}



//////////////////////////////
//
// soundReadSample16L --
//
 
sampleType soundReadSample16L_B(FileIO& soundfile) {
   short sample;
   soundfile.readBigEndian(sample);
   return (sampleType)sample / (long)0x8000;
}

 
sampleType soundReadSample16L_L(FileIO& soundfile) {
   short sample;
   soundfile.readLittleEndian(sample);
   return (sampleType)sample / (long)0x8000;
}



//////////////////////////////
//
// soundReadSample24L --
//

sampleType soundReadSample24L_B(FileIO& soundfile) {
   char temp[3];
   long sample;
   soundfile.readLittleEndian(temp[0]);
   soundfile.readLittleEndian(temp[1]);
   soundfile.readLittleEndian(temp[2]);
   sample = temp[0];
   sample = (sample << 8) | temp[1];
   sample = (sample << 8) | temp[2];
   return (sampleType)sample / (long)0x800000;
}


sampleType soundReadSample24L_L(FileIO& soundfile) {
   char temp[3];
   long sample;
   soundfile.readLittleEndian(temp[2]);
   soundfile.readLittleEndian(temp[1]);
   soundfile.readLittleEndian(temp[0]);
   sample = temp[0];
   sample = (sample << 8) | temp[1];
   sample = (sample << 8) | temp[2];
   return (sampleType)sample / (long)0x800000;
}



//////////////////////////////
//
// soundReadSample32L --
//

sampleType soundReadSample32L_B(FileIO& soundfile) {
   long sample;
   soundfile.readBigEndian(sample);
   return (sampleType)sample / (ulong)0x80000000;
}


sampleType soundReadSample32L_L(FileIO& soundfile) {
   long sample;
   soundfile.readLittleEndian(sample);
   return (sampleType)sample / (ulong)0x80000000;
}



//////////////////////////////
//
// soundReadSample32F --
//

sampleType soundReadSample32F_B(FileIO& soundfile) {
   float sample;
   soundfile.readBigEndian(sample);
   return (sampleType)sample;
}


sampleType soundReadSample32F_L(FileIO& soundfile) {
   float sample;
   soundfile.readLittleEndian(sample);
   return (sampleType)sample;
}



//////////////////////////////
//
// soundReadSample64F --
//


sampleType soundReadSample64F_B(FileIO& soundfile) {
   double sample;
   soundfile.readBigEndian(sample);
   return (sampleType)sample;
}


sampleType soundReadSample64F_L(FileIO& soundfile) {
   double sample;
   soundfile.readLittleEndian(sample);
   return (sampleType)sample;
}





// md5sum:	aeae7d20d00b7632bbbb7f8795bccdfc  SoundFileIn.cpp
