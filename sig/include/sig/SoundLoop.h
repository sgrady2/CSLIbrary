//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Mar  9 14:48:43 PST 1999
// Last Modified: Tue Mar  9 14:48:49 PST 1999
// Filename:      ...sig/code/Generator/SoundLoop/SoundLoop.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//
//

#ifndef _SOUNDLOOP_H_INCLUDED
#define _SOUNDLOOP_H_INCLUDED

#include "SoundFileInMemory.h"


#define LOOPMODE_STRAIGHT   0
#define LOOPMODE_PALINDROME 1

class SoundLoop : public SoundFileInMemory {
   public:
                        SoundLoop      (char* aFileName = "",
                                             float start=NONE,
                                             float dur = NONE);
                       ~SoundLoop      ();

      void              action         (void);
      void              borrow         (const SoundFileInMemory& aFile);
      int               changeBegin    (int delta);
      int               changeEnd      (int delta);
      int               changeStart    (int delta);
      int               getBegin       (void) const;
      int               getCurrent     (void) const;
      int               getEnd         (void) const;
      int               getFill        (void) const;
      int               getStart       (void) const;
      void              goForward      (void);
      void              goBackward     (void);
      void              goTo           (int sampleNum);
      void              gotoBegin      (void);
      void              gotoEnd        (void);
      void              gotoStart      (void);
      void              loopStraight   (void);
      void              loopPalindrome (void);
      void              printState     (void);
      void              reset          (int sampleIndex = 0);
      void              setBegin       (int sampleNum);
      void              setEnd         (int sampleNum);
      void              setFile        (char* aFilename);
      void              setFill        (int numSamples);
      void              setStart       (int sampleNum);

   protected:
      int               fillAmount;    // blanks samples between loops
      int               currentFill;   // current cound of blanks
      int               direction;     // forward=1 backward=-1
      int               boundaryQ;     // 1=at loop boundary, 0 if not
      int               startPointer;   // initial sample position
      int               beginPointer;   // begin sample of loop
      int               endPointer;     // end sample of loop
      int               loopMode;      // method of looping 0=normal, 1=palindrm
};
        

#endif  _SOUNDLOOP_H_INCLUDED


// md5sum:	b1f46544df0e99d6f7726f34c57a2857  SoundLoop.h
