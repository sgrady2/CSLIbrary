//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Nov  1 23:35:02 PST 1996
// Last Modified: Mon Apr 28 21:52:13 GMT-0800 1997
// Filename:      ...sig/code/Generator/SoundFileIn/SoundFileIn.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//
//

#ifndef _SOUNDFILEIN_H_INCLUDED
#define _SOUNDFILEIN_H_INCLUDED


#include "Generator.h"
#include "SoundHeader.h"
#include "helperFunctions.h"
#include "Array.h"
#include "FileIO.h"

#include <fstream.h>
#include <iostream.h>
#include <assert.h>


class SoundFileIn : public Generator {
   public:
                        SoundFileIn           (char* aFileName = "",
                                                 float start=NONE,
                                                 float dur = NONE);
                       ~SoundFileIn();

      void              action                (void);
      short             extractSample16Bit    (void);
      int               getBitsPerSample      (void);
      int               getChannels           (void);
      const char*       getFilename           (void) const;
      long              getSamples            (void);
      SoundHeader&      getHeader             (void);
      sampleType        output                (int channel = 0);
      void              printState            (void);
      void              reset                 (int sampleIndex = 0);
      void              setFile               (char* aFileName, float start = 
                                                 NONE, float dur = NONE);

   protected:
      FileIO            sndfile;              // soundfile input stream
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
     
      sampleType        (*readSample)                    (FileIO&);
      void              determineSampleReadingFunction   (void);
      void              fillOutputArray                  (void);
        
};
        


#endif  _SOUNDFILEIN_H_INCLUDED



// md5sum:	d6f1ad602eba235cbcd87fa2f5c7b026  SoundFileIn.h
