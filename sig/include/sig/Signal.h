//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Nov  1 23:35:02 PST 1996
// Last Modified: Mon Apr 28 19:26:28 GMT-0800 1997
// Filename:      ...sig/code/Signal/Signal.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../include && rm -f %b.o
//

#ifndef _SIGNAL_H_INCLUDED
#define _SIGNAL_H_INCLUDED


#include "sigTypes.h"
#include "sigDefines.h"


class Signal {
   public:
                           Signal           (void);
      virtual             ~Signal           ();

      virtual void         action           (void) = 0;
      void                 clear            (void) {}
      char const*          getName          (void) const;
      float                getSrate         (void) const;
      int                  getType          (void) const;
      virtual sampleType   output           (int index = 0) = 0;
      virtual sampleType   output           (double index);
      virtual void         printConnections (int flag) = 0;
      virtual void         printState       (void) = 0;
      void                 setName          (const char* newname);
      virtual void         setSrate         (float aSrate);
      virtual void         tick             (int newTickNumber) = 0;   

   protected:
      int                  tickNumber;
      char                *signalName;
      float                srate;
      int                  brandname;       // 0 = signal, 1 = generator, 
                                            // 2 = filter
};



#endif  // _SIGNAL_H_INCLUDED



// md5sum:	53678841ce3ee4de05a3cff0029e4de7  Signal.h
