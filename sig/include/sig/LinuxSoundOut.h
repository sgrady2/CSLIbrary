//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu May  8 00:16:50 GMT-0800 1997
// Last Modified: Thu May  8 00:16:53 GMT-0800 1997
// Filename:      .../sig/code/Filter/LinuxSoundOut/LinuxSoundOut.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#ifndef _LINUXSOUNDOUT_H_INCLUDED
#define _LINUXSOUNDOUT_H_INCLUDED


#include "Filter.h"
#include "Array.h"

#include "RTSoundIO.h"

class LinuxSoundOut : public Filter {
   public:
                         LinuxSoundOut  (MY_FLOAT srate, int channels, int port = 0);
                        ~LinuxSoundOut  ();

      void               action         (void);
      sampleType         output         (int index);
      void               flushBuffer    (void);
 
   protected:
      RTSoundIO         *truesound;           // STK sound interface

      short*             buffer;              // storage area for sound
      long               bufferSize;          // size of storage in samples
      int                bufferIndex;         // current pointer to buffer

      // private functions
      void               sendBuffer     (void);
      void               zeroSend       (void);  // send leftover frame
      
};



#endif  // _LINUXSOUNDOUT_H_INCLUDED



// md5sum:	8a9bc74674fc66afbfd432e06e5e827a  LinuxSoundOut.h
