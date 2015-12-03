//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue May  6 20:09:36 GMT-0800 1997
// Last Modified: Tue May  6 20:09:39 GMT-0800 1997
// Filename:      ...sig/code/Filter/Selector/Selector.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "Selector.h"
#include <stdlib.h>


//////////////////////////////
//
// Selector::Selector
//	default value: aChannel = 0;
//

Selector::Selector(double aChannel) {
   setName("Selector");
   setChannel(aChannel);
   brandname = FILTER;
}



//////////////////////////////
//
// Selector::action
//

void Selector::action(void) {
   // do nothing   
}



//////////////////////////////
//
// Selector::getChannel
//

double Selector::getChannel(void) {
   return streamChannel;
}



//////////////////////////////
//
// Selector::output
//

sampleType Selector::output(int channel) {
   return inputs.signal(0)->output(streamChannel);
}


//////////////////////////////
//
// Selector::setChannel
//

void Selector::setChannel(double aChannel) {
   streamChannel = aChannel;
}



// md5sum:	97f165a1655ca4f4929c79464b2921b3  Selector.cpp
