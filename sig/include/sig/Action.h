//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Nov  2 23:37:12 PST 1996
// Last Modified: Thu May 22 23:31:00 GMT-0800 1997
// Filename:      .../sig/code/base/Action/Action.h
// Web Address:   http://sig.sapp.org/include/sig/Action.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _ACTION_H_INCLUDED
#define _ACTION_H_INCLUDED


#include "Signal.h"
#include "Collection.h"
#include "sigTypes.h"
#include <iostream.h>


class Action {

   public:
                           Action            (void);
                          ~Action            ();

      void                 freezeState       (Signal& sig);
      void                 recursiveTick     (Signal& sig, long numTicks = 1);
      void                 tick              (Signal& sig);
      void                 tick              (Signal& sig, long numTicks);
      void                 tickFrozenState   (long numTicks = 1);

   protected:
      long                 ticker;           // for recursive tick Generation
      Collection<Signal*>  signalState;      // signals arranged in action order

      void                 recurseSetState   (Signal *sig);

};



#endif  // _ACTION_H_INCLUDED



// md5sum:        551b057c8be870eb11c75fe946fe500a Action.h
