//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Mar 11 22:01:39 GMT-0800 1997
// Last Modified: Sat Jul  5 21:20:49 PDT 1997
// Filename:      ...sig/code/base/Signal/Input/Input.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#include "Input.h"


//////////////////////////////
//
// Input::Input
//

Input::Input(void) { 
   inputs.allowGrowth();
   channels.allowGrowth();
   defaults.allowGrowth();
   defaults.zero();
}

Input::Input(Signal& aSignal) {
   inputs.allowGrowth();
   channels.allowGrowth();
   insert(aSignal);
   defaults.allowGrowth();
   defaults.zero();
}

Input::Input(Signal* aSignal) {
   inputs.allowGrowth();
   channels.allowGrowth();
   insert(aSignal);
   defaults.allowGrowth();
   defaults.zero();
}

Input::Input(Signal& aSignal, Signal& bSignal) {
   inputs.allowGrowth();
   channels.allowGrowth();
   insert(aSignal);
   insert(bSignal);
   defaults.allowGrowth();
   defaults.zero();
}

Input::Input(Signal* aSignal, Signal* bSignal) {
   inputs.allowGrowth();
   channels.allowGrowth();
   insert(aSignal);
   insert(bSignal);
   defaults.allowGrowth();
   defaults.zero();
}

Input::Input(Signal& aSignal, Signal& bSignal, Signal& cSignal) {
   inputs.allowGrowth();
   channels.allowGrowth();
   insert(aSignal);
   insert(bSignal);
   insert(cSignal);
   defaults.allowGrowth();
   defaults.zero();
}

Input::Input(Signal* aSignal, Signal* bSignal, Signal* cSignal) {
   inputs.allowGrowth();
   channels.allowGrowth();
   insert(aSignal);
   insert(bSignal);
   insert(cSignal);
   defaults.allowGrowth();
   defaults.zero();
}



//////////////////////////////
//
// Input::~Input
//

Input::~Input() { }



//////////////////////////////
//
// Input::connectionQ
//

int Input::connectionQ(int index) {
   if (index < 0 || index >= inputs.getSize()) {
      return 0;
   } else if (inputs[index] == NULL) {
      return 0;
   } else {
      return 1;
   }
}



//////////////////////////////
//
// Input::getSize
//

int Input::getSize(void) {
   return inputs.getSize();
}


//////////////////////////////
//
// Input::insert
//	Default value: index = -1, channel = 0
//

Signal* Input::insert(Signal& aSignal, int index, int channel) {
   int i;
   if (index >= defaults.getSize()) {
      setDefault(index, 0.0);
   } 
   if (index < 0) {
      // check for a blank spot in input array and insert signal pointer
      for (i=0; i<inputs.getSize(); i++) {
         if (inputs[i] == NULL) {
            inputs[i] = &aSignal;
            channels[i] = channel;
            return NULL;
         }
      }
      // no blank spot, add at end
      inputs[inputs.getSize()] = &aSignal;
      channels[i] = channel;
      return NULL;
   } else if (index <= inputs.getSize()) {
      Signal* temp = inputs[index];
      inputs[index] = &aSignal;
      channels[index] = channel;
      return temp;
   } else {             // need to grow by more than one element in input array
      int oldSize = inputs.getSize();
      inputs.grow(index - oldSize + 1);
      channels.grow(index - oldSize + 1);
      for (i=oldSize; i<index-1; i++) {
         inputs[i] = NULL;
         channels[i] = 0;
      }
      inputs[index] = &aSignal;
      channels[index] = channel;
      return NULL;
   }
}

Signal* Input::insert(Signal* aSignal, int index, int channel) {
   return insert(*aSignal, index, channel);
}


//////////////////////////////
//
// Input::operator[]
//

sampleType Input::operator[](int index) {
   if (connectionQ(index)) {
      return inputs[index]->output(channels[index]);
   } else {
      if (index >= 0 && index < defaults.getSize()) {
         return defaults[index];
      } else {
         return 0;
      }
   }
}



//////////////////////////////
//
// Input::printChannels
//

void Input::printChannels(void) {
   cerr << "Channel size is : " << channels.getSize() << endl;
   for (int i=0; i<channels.getSize(); i++) {
      cerr << " +++ index " << i << " channel is " << channels[i] << endl;
   }
}



//////////////////////////////
//
// Input::remove
//

Signal* Input::remove(int index) {
   if (index < 0 || index >= inputs.getSize()) {
      for (int i=0; i<inputs.getSize(); i++) {
         inputs[i] = NULL;
         channels[i] = 0;
      }
      return NULL;
   } else {
      Signal* temp = inputs[index];
      inputs[index] = NULL;
      channels[index] = 0;
      return temp;
   }
}


int Input::remove(Signal& aSignal) {
   for (int i=0; i<inputs.getSize(); i++) {
      if (inputs[i] == &aSignal) {
         inputs[i] = NULL;
         return 1;
      }
   }
   return 0;
}


int Input::remove(Signal* aSignal) {
   return remove(*aSignal);
}
   


//////////////////////////////
//
// Input::setDefault
//

sampleType Input::setDefault(int index, sampleType aNewDefault) {
   if (index > defaults.getSize()) {
      int previousSize = defaults.getSize();
      defaults.grow(index - previousSize + 1);
      for (int i=previousSize; i<index; i++) {
         defaults[i] = 0.0;
      }
   }
   sampleType oldDefault = defaults[index];
   defaults[index] = aNewDefault;
   return oldDefault;
}


//////////////////////////////
//
// Input::signal
//

Signal* Input::signal(int index) {
   if (connectionQ(index)) {
      return inputs[index];
   } else {
      return NULL;
   }
}



//////////////////////////////
//
// Input::tick
//

void Input::tick(long newTickNumber) {
   for (int i=0; i<inputs.getSize(); i++) {
      if (connectionQ(i)) {
         inputs[i]->tick(newTickNumber);
      }
   }
}



// md5sum:	6f8ee7269ed35d6cc439e10ebeb9284a  Input.cpp
