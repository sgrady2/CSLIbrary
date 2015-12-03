//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Nov  3 20:04:10 PST 1997
// Last Modified: Mon Nov  3 20:04:13 PST 1997
// Filename:      ...sig/code/Filter/ResoundOut/ResoundOut.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _RESOUNDOUT_H_INCLUDED
#define _RESOUNDOUT_H_INCLUDED


#include "Filter.h"
#include "SoundHeader.h"
#include <fstream.h>


class ResoundOut : public Filter {
   public:
                     ResoundOut        (short *soundData, SoundHeader& header);
                    ~ResoundOut        ();
      void           action            (void);
      void           clear             (void);
      sampleType     output            (int channel);
 
   protected:
      short         *data;
      SoundHeader    sndheader;
      long           sampleCount;
};



#endif  // _RESOUNDOUT_H_INCLUDED



// md5sum:	02a3005ba771c9be9b9f17027b2ac05b  ResoundOut.h
