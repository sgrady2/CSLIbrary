//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Mar  9 08:05:31 PST 1999
// Last Modified: Tue Mar  9 08:05:34 PST 1999
// Filename:      ...sig/code/Generator/SoundLoop/SoundLoop.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#include "SoundLoop.h"
#include <iostream.h>
#include <stdlib.h>
#include <string.h>



//////////////////////////////
//
// SoundLoop::SoundLoop --
//    defaults: aFileName = "", start = NONE, dur = NONE
//

SoundLoop::SoundLoop(char* aFileName, float start, float dur) :
      SoundFileInMemory(aFileName, start, dur) {
   fillAmount = 0;               // blanks samples between loops
   currentFill = 0;              // current cound of blanks
   direction = 1;                // forward=1 backward=-1
   boundaryQ = 0;                // 1=at loop boundary, 0 if not
   startPointer = 0;              // initial sample position
   beginPointer = 0;              // begin sample of loop
   endPointer = soundDataSize - sndHeader.getBlockAlign();  
   loopMode = LOOPMODE_STRAIGHT;
  
   if (aFileName[0] != '\0') {
      setFile(aFileName);
   }
}



//////////////////////////////
//
// SoundLoop::~SoundLoop --
//

SoundLoop::~SoundLoop() {
   // do nothing;
}



//////////////////////////////
//
// SoundLoop::action -- calculate the next sound sample and adjust
//    sound data pointer accordingly.
//

void SoundLoop::action(void) { 
   if (boundaryQ) {
      if (currentFill < fillAmount) {
         currentFill++;
         Output.zero();
         return;
      } else {
         currentFill = 0;
         boundaryQ = 0;
      }
   } 

   fillOutputArray();

   if (direction == 1) {
      if (dataPointer > endPointer) {
         switch (loopMode) {
            case LOOPMODE_PALINDROME:
               direction = -1;
               boundaryQ = 1;
               break;
            case LOOPMODE_STRAIGHT:
            default:
               dataPointer = beginPointer;
               boundaryQ = 1;
               break;

         }
      }
   } else {
      if (direction == -1 && loopMode == LOOPMODE_PALINDROME &&
           dataPointer == beginPointer + sndHeader.getBlockAlign()) {
         direction = 1;
         dataPointer = beginPointer + sndHeader.getBlockAlign();
      } else if (dataPointer <= beginPointer) {
         switch (loopMode) {
            case LOOPMODE_PALINDROME:
               direction = 1;
               dataPointer = beginPointer + sndHeader.getBlockAlign();
               break;
            case LOOPMODE_STRAIGHT:
            default:
               dataPointer = endPointer;
               boundaryQ = 1;
               break;

         }
 
      }
   }

   if (direction == -1) {
      dataPointer -= sndHeader.getBlockAlign() * 2;
      if (dataPointer < 0) {
         dataPointer = 0;
      }
  
   }

}



//////////////////////////////
//
// SoundLoop::borrow --
//

void SoundLoop::borrow(const SoundFileInMemory& aFile) {
   SoundFileInMemory::borrow(aFile);
   startPointer = 0;
   endPointer = soundDataSize;
   beginPointer = 0;
   dataPointer = startPointer;
}



//////////////////////////////
//
// SoundLoop::changeBegin -- adjust the beginning of the loop
//     returns the new begin point
//

int SoundLoop::changeBegin(int delta) {
   beginPointer += delta * sndHeader.getBlockAlign();

   if (beginPointer < 0) {
      beginPointer = 0;
   }
   if (beginPointer > endPointer && soundData != NULL) {
      beginPointer = endPointer;
   }

   return getBegin();
}



//////////////////////////////
//
// SoundLoop::changeEnd -- adjust the beginning of the loop
//     returns the new begin point
//

int SoundLoop::changeEnd(int delta) {
cout << "Changing end" << endl;
   endPointer += delta * sndHeader.getBlockAlign();

   if (endPointer > soundDataSize) {
      endPointer = soundDataSize;
   }
   if (endPointer < beginPointer && soundData != NULL) {
      endPointer = beginPointer;
   }

   return getEnd();
}



//////////////////////////////
//
// SoundLoop::changeStart -- adjust the beginning of the loop
//     returns the new begin point
//

int SoundLoop::changeStart(int delta) {
   startPointer += delta * sndHeader.getBlockAlign();
   
   if (startPointer > soundDataSize) {
      startPointer = soundDataSize;
   }
   if (startPointer < 0) {
      startPointer = 0;
   }

   return getStart();
}



//////////////////////////////
//
// SoundLoop::getBegin -- return the beginning loop sample
//
int SoundLoop::getBegin(void) const {
   return beginPointer / sndHeader.getBlockAlign();
}



//////////////////////////////
//
// SoundLoop::getCurrent -- return the current sample number
//

int SoundLoop::getCurrent(void) const {
   return dataPointer / sndHeader.getBlockAlign();
}



//////////////////////////////
//
// SoundLoop::getEnd -- return the ending sample of the loop
//

int SoundLoop::getEnd(void) const {
   return endPointer / sndHeader.getBlockAlign();
}



//////////////////////////////
//
// SoundLoop::getFill -- return the fill sample amount
//

int SoundLoop::getFill(void) const {
   return fillAmount;
}



//////////////////////////////
//
// SoundLoop::getStart -- return to the start sample point of the
//      sound.
//

int SoundLoop::getStart(void) const {
   return startPointer / sndHeader.getBlockAlign();
}



//////////////////////////////
//
// SoundLoop::goForward -- set the direction to go forward
//

void SoundLoop::goForward(void) { 
   direction = 1;
}



//////////////////////////////
//
// SoundLoop::goBackward -- set the direction to go backward
//

void SoundLoop::goBackward(void) { 
   direction = -1;
}



//////////////////////////////
//
// SoundLoop::goTo -- go to the specified sample position
//

void SoundLoop::goTo(int sampleNum) { 
   int temp = sampleNum * getChannels() * (getBitsPerSample()/8);
   if (temp >= soundDataSize) {
      cout << "Error: Cannot go to sample " << sampleNum << endl;
      cout << "Ignoring request" << endl;
      return;
   } 

   dataPointer = temp;
}



//////////////////////////////
//
// SoundLoop::gotoBegin -- go to the beginning loop sample
//

void SoundLoop::gotoBegin(void) { 
   dataPointer = beginPointer;
}



//////////////////////////////
//
// SoundLoop::gotoEnd -- go the the loop end sample
//

void SoundLoop::gotoEnd(void) { 
   dataPointer = endPointer;
}



//////////////////////////////
//
// SoundLoop::gotoStart -- go the the loop end sample
//

void SoundLoop::gotoStart(void) { 
   dataPointer = startPointer;
}



//////////////////////////////
//
// SoundLoop::loopStraight --  loop in a normal fashion
//

void SoundLoop::loopStraight(void) {
   loopMode = LOOPMODE_STRAIGHT;
}


//////////////////////////////
//
// SoundLoop::gotoEnd -- go the the loop end sample
//

void SoundLoop::loopPalindrome(void) {
   loopMode = LOOPMODE_PALINDROME;
}


//////////////////////////////
//
// SoundLoop::printState -- print the current state of the sound loop
//

void SoundLoop::printState(void) { }



//////////////////////////////
//
// SoundLoop::reset -- similar goto(0) command
//     default value: sampleIndex = 0;
//

void SoundLoop::reset(int sampleIndex) {
   dataPointer = startPointer;
}



//////////////////////////////
//
// SoundLoop::setBegin -- set the beginning sample of the loop.
//

void SoundLoop::setBegin(int sampleNum) { 
   int temp = sampleNum * sndHeader.getBlockAlign();
   if (temp >= soundDataSize || sampleNum < -1) {
      cout << "Cannot set begin sample to " << sampleNum << endl;
      return;
   }

   beginPointer = temp;
}



//////////////////////////////
//
// SoundLoop::setEnd -- set the ending sample of the loop
//

void SoundLoop::setEnd(int sampleNum) { 
   int temp = sampleNum * sndHeader.getBlockAlign();
   if (temp >= soundDataSize || sampleNum < -1) {
      cout << "Cannot set end sample to " << sampleNum << endl;
      return;
   }

   endPointer = temp;
}



//////////////////////////////
//
// SoundLoop::setFile -- 
//

void SoundLoop::setFile(char* aFilename) {
   SoundFileInMemory::setFile(aFilename);
   startPointer = 0;
   endPointer = soundDataSize;
   beginPointer = 0;
   dataPointer = startPointer;
}



//////////////////////////////
//
// SoundLoop::setFill -- set the number of blank samples between
//    loop boundary.
//

void SoundLoop::setFill(int numSamples) { 
   if (numSamples < 0) {
      cout << "Error: cannot have a negative fill at loop boundary" << endl;
      return;
   }

   fillAmount = numSamples;
}



//////////////////////////////
//
// SoundLoop::setStart -- set the initial sample of the loop before it
//     loops
//

void SoundLoop::setStart(int sampleNum) { 
   int temp = sampleNum * sndHeader.getBlockAlign();
   if (temp >= soundDataSize || sampleNum < -1) {
      cout << "Cannot set start sample to " << sampleNum << endl;
      return;
   }

   startPointer = temp;
}




// md5sum:	058922d1fabf6a1efcfdac73a902ae5e  SoundLoop.cpp
