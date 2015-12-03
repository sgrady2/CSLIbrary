//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Mar  9 08:03:17 PST 1999
// Last Modified: Tue Mar  9 08:03:22 PST 1999
// Filename:      ...sig/code/Generator/SoundFileInMemory/SoundFileInMemory.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//
//

#ifndef _SOUNDFILEINMEMORY_H_INCLUDED
#define _SOUNDFILEINMEMORY_H_INCLUDED


#include "Generator.h"
#include "SoundHeader.h"
#include "helperFunctions.h"
#include "Array.h"
#include "FileIO.h"

#include <fstream.h>
#include <iostream.h>
#include <assert.h>


class SoundFileInMemory : public Generator {
   public:
                        SoundFileInMemory     (char* aFileName = "",
                                                 float start=NONE,
                                                 float dur = NONE);
                       ~SoundFileInMemory     ();

      void              action                (void);
      void              borrow                (const SoundFileInMemory& aFile);
      short             extractSample16Bit    (void);
      int               getBitsPerSample      (void);
      int               getChannels           (void);
      const char*       getFilename           (void) const;
      static int        getMemoryUsage        (void);
      long              getSamples            (void);
      SoundHeader&      getHeader             (void);
      sampleType        output                (int channel = 0);
      int               ownership             (void) const;
      void              printState            (void);
      void              reset                 (int sampleIndex = 0);
      void              setFile               (char* aFileName, float start = 
                                                 NONE, float dur = NONE);
      static short      mulaw8ToLinear16      (uchar index);

   protected:
      FileIO            sndfile;              // soundfile to read in
      int               endianType;           // how soundfile data is stored
      char             *filename;             // name of the soundfile
      SoundHeader       sndHeader;            // header from soundfile
      long              curSampleIndex;       // the current sample number
      long              begSampleIndex;       // the first sample number
      long              endSampleIndex;       // the last sample number
      long              maxSampleIndex;       // the highest sample index value 
      int               loopingQ;
      int               runningQ;
      Array<sampleType> Output;               // output samples, by channel
     
      sampleType        (*readSample)         (SoundFileInMemory& x);
      void              determineSampleReadingFunction   (void);
      void              fillOutputArray                  (void);


      static int        memoryUsage;          // keeping track of total memory
      char*             soundData;            // storage for soundfile
      int               borrowedQ;            // true if sound is being borrowed
      int               soundDataSize;        // size of storage
      int               dataPointer;          // current data pointer
        

   // friends
      // big endian samples
      friend sampleType SMEM_soundReadSample8M_B  (SoundFileInMemory& x);
      friend sampleType SMEM_soundReadSample8L_B  (SoundFileInMemory& x);
      friend sampleType SMEM_soundReadSample16L_B (SoundFileInMemory& x);
      friend sampleType SMEM_soundReadSample24L_B (SoundFileInMemory& x);
      friend sampleType SMEM_soundReadSample32L_B (SoundFileInMemory& x);
      friend sampleType SMEM_soundReadSample32F_B (SoundFileInMemory& x);
      friend sampleType SMEM_soundReadSample64F_B (SoundFileInMemory& x);

      // little endian samples
      friend sampleType SMEM_soundReadSample8M_L  (SoundFileInMemory& x);
      friend sampleType SMEM_soundReadSample8L_L  (SoundFileInMemory& x);
      friend sampleType SMEM_soundReadSample16L_L (SoundFileInMemory& x);
      friend sampleType SMEM_soundReadSample24L_L (SoundFileInMemory& x);
      friend sampleType SMEM_soundReadSample32L_L (SoundFileInMemory& x);
      friend sampleType SMEM_soundReadSample32F_L (SoundFileInMemory& x);
      friend sampleType SMEM_soundReadSample64F_L (SoundFileInMemory& x);


   private:

      void readBigEndian(char& aNumber);
      void readBigEndian(uchar& aNumber);
      void readBigEndian(short& aNumber);
      void readBigEndian(ushort& aNumber);
      void readBigEndian(long& aNumber);
      void readBigEndian(ulong& aNumber);
      void readBigEndian(int& aNumber);
      void readBigEndian(uint& aNumber);
      void readBigEndian(float& aNumber);
      void readBigEndian(double& aNumber);

      void readLittleEndian(char& aNumber);
      void readLittleEndian(uchar& aNumber);
      void readLittleEndian(short& aNumber);
      void readLittleEndian(ushort& aNumber);
      void readLittleEndian(long& aNumber);
      void readLittleEndian(ulong& aNumber);
      void readLittleEndian(int& aNumber);
      void readLittleEndian(uint& aNumber);
      void readLittleEndian(float& aNumber);
      void readLittleEndian(double& aNumber);

      void readMachineEndian(char& aNumber);
      void readMachineEndian(uchar& aNumber);
      void readMachineEndian(short& aNumber);
      void readMachineEndian(ushort& aNumber);
      void readMachineEndian(long& aNumber);
      void readMachineEndian(ulong& aNumber);
      void readMachineEndian(int& aNumber);
      void readMachineEndian(uint& aNumber);
      void readMachineEndian(float& aNumber);
      void readMachineEndian(double& aNumber);

      void readNotMachineEndian(char& aNumber);
      void readNotMachineEndian(uchar& aNumber);
      void readNotMachineEndian(short& aNumber);
      void readNotMachineEndian(ushort& aNumber);
      void readNotMachineEndian(long& aNumber);
      void readNotMachineEndian(ulong& aNumber);
      void readNotMachineEndian(int& aNumber);
      void readNotMachineEndian(uint& aNumber);
      void readNotMachineEndian(float& aNumber);
      void readNotMachineEndian(double& aNumber);

      void storageRead(char& aNumber);
      void storageRead(uchar& aNumber);
      void storageRead(short& aNumber);
      void storageRead(ushort& aNumber);
      void storageRead(long& aNumber);
      void storageRead(ulong& aNumber);
      void storageRead(int& aNumber);
      void storageRead(uint& aNumber);
      void storageRead(float& aNumber);
      void storageRead(double& aNumber);

      char   flipBytes(char aNumber);
      uchar  flipBytes(uchar aNumber);
      short  flipBytes(short aNumber);
      ushort flipBytes(ushort aNumber);
      long   flipBytes(long aNumber);
      ulong  flipBytes(ulong aNumber);
      int    flipBytes(int aNumber);
      uint   flipBytes(uint aNumber);
      float  flipBytes(float aNumber);
      double flipBytes(double aNumber);

};
        

#endif  _SOUNDFILEINMEMORY_H_INCLUDED


// md5sum:	3fe67359fce0d1b54f4ed16c379294e2  SoundFileInMemory.h
