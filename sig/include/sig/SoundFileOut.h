//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Nov 12 15:17:54 PST 1996
// Last Modified: Tue Apr 29 21:38:49 GMT-0800 1997
// Filename:      ...sig/code/Filter/SoundFileOut/SoundFileOut.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _SOUNDFILEOUT_H_INCLUDED
#define _SOUNDFILEOUT_H_INCLUDED


#include "Filter.h"
#include "SoundHeader.h"
#include <fstream.h>


class SoundFileOut : public Filter {
   public:
                     SoundFileOut      (char* aFileName, SoundHeader& header);
                    ~SoundFileOut      ();
      void           action            (void);
      void           close             (void);
      const char*    getFilename       (void) const;
      sampleType     output            (int channel);

      void           insertSample16Bit (short sample);
 
   protected:
      char          *filename;
      int            endianType;
      SoundHeader    sndHeader;
      FileIO         outFile;
      long           sampleCount;
      ushort         outSample;


      void           determineSampleWritingFunction   (void);
      
      void           (*writeSample)       (FileIO& outFile, sampleType aSample);


};



#endif  // _SOUNDFILEOUT_H_INCLUDED



// md5sum:	e0b1282e71d8e2eba9d9985ebc234460  SoundFileOut.h
