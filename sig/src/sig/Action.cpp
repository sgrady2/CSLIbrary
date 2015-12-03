//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Nov  1 23:35:02 PST 1996
// Last Modified: Fri Nov  1 23:35:02 PST 1996
// Filename:      ...sig/code/Action/Action.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#include "Action.h"
#include "Filter.h"
#include <iostream.h>


//////////////////////////////
//
// Action::Action
//

Action::Action(void) {
   ticker = 0;
}



//////////////////////////////
//
// Action::~Action
//

Action::~Action() { }
  


//////////////////////////////
//
// Action::freezeState
//

void Action::freezeState(Signal& sig) {
   signalState.setSize(0);
   recurseSetState(&sig);
}



//////////////////////////////
//
// Action::recursiveTick
//	default value: numTicks = 1
//

void Action::recursiveTick(Signal& sig, long numTicks) {
   for (int i=0; i<numTicks; i++) {
      sig.tick(ticker++);
   }
}



//////////////////////////////
//
// Action::tick
//

void Action::tick(Signal& sig) {
   sig.tick(ticker++);
}


void Action::tick(Signal& sig, long numTicks) {
   ticker += numTicks;
   freezeState(sig);
   tickFrozenState(numTicks);
}
   


//////////////////////////////
//
// Action::tickFrozenState
//

void Action::tickFrozenState(long numTicks) {
   int i, j;
   for (i=0; i<numTicks; i++) {
      // signalState has action order in reverse
      for (j=signalState.getSize()-1; j>=0; j--) {  
         signalState[j]->action();
      }
   }
}


//////////////////////////////////////////////////////////////////////////////
//
// Private functions
//

//////////////////////////////
//
// Action::recurseSetState -- recurses through a signal stream storing 
//	generator and filter pointers in an array in the correct order
//	(which is in reverse ticking order).
//

void Action::recurseSetState(Signal *sig) {
   int i;

   // check if signal has already been counted
   for (i=0; i<signalState.getSize(); i++) {
      if (signalState[i] == sig) return;    
   }


   // not in collection, so add it.  Then, add all inputs to collection
   signalState.append(sig);
   
   switch(sig->getType()) {
      case GENERATOR:
         return;
         break;
      case FILTER:
         for (i=0; i<((Filter*)sig)->inputs.getSize(); i++) {
            if (((Filter*)sig)->inputs.connectionQ(i)) {
               recurseSetState(((Filter*)sig)->inputs.signal(i));
            }
         }
         break;
      default:
         cerr << "Unknown signal type: " << sig->getType() 
              << " for signal " << sig->getName() << endl;
         exit(1);
   }

}

// md5sum:	88796b3e5c1aea71a7f6bd0e9dcca362  Action.cpp
