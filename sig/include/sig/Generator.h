//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Nov  1 23:35:02 PST 1996
// Last Modified: Mon Apr 28 19:44:22 GMT-0800 1997
// Filename:      .../sig/code/Generator/Generator.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../include && rm -f %b.o
//

#ifndef _GENERATOR_H_INCLUDED
#define _GENERATOR_H_INCLUDED


#include "Signal.h"


class Generator : public Signal {
   public:
                         Generator          (void);
                        ~Generator          ();

      void               printConnections   (int flag); 
      void               printState         (void)  {}
      virtual void       tick               (int newTickNumber);
};



#endif  _GENERATOR_H_INCLUDED



// md5sum:	863e0a1dfbf55b4a2146c197468709d8  Generator.h
