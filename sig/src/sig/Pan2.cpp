//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Jun  8 10:03:02 GMT-0800 1997
// Last Modified: Sun Jun  8 10:03:05 GMT-0800 1997
// Filename:      ...sig/code/Filter/Pan2/Pan2.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "Pan2.h"
#include <stdlib.h>
#include <math.h>


//////////////////////////////
//
// Pan2::Pan2
//

Pan2::Pan2(void) {
   setName("Pan2");
   outputValueLeft = 0;
   outputValueRight = 0;
   brandname = FILTER;
   setSpeedOfSound(330.0);
   setHeadSize(0.2);
   delayline.setSize(128);  // set back to 64, or 32 actually
}



//////////////////////////////
//
// Pan2::~Pan2
//

Pan2::~Pan2() { }



//////////////////////////////
//
// Pan2::action
//

void Pan2::action(void) {
   delayline.write(getSignal());

   distanceToRightEar = sqrt(getY()*getY()+
      (getX()-getHeadSize())*(getX()-getHeadSize()));
   distanceToLeftEar  = sqrt(getY()*getY()+
      (getX()+getHeadSize())*(getX()+getHeadSize()));
   sampleDelay = (distanceToRightEar - distanceToLeftEar)/getSpeedOfSound()*
      getSrate()/2.0;
   if (sampleDelay < 0) {
      leftDelayIndex  = -15 + sampleDelay;
      rightDelayIndex = -15 - sampleDelay;
   } else {
      leftDelayIndex  = -15 + sampleDelay;
      rightDelayIndex = -15 - sampleDelay;
   }

   // apply an amplitude envelope to the sound:
   if (distanceToLeftEar < 0.5) {
      outputValueLeft = delayline[leftDelayIndex] / 0.5;
   } else {
      outputValueLeft = delayline[leftDelayIndex] / distanceToLeftEar;
   }

   if (distanceToRightEar < 0.5) {
      outputValueRight = delayline[rightDelayIndex] / 0.5;
   } else {
      outputValueRight = delayline[rightDelayIndex] / distanceToRightEar;
   }

if (rightDelayIndex > 0 ) {
   cerr << "Error in right " << rightDelayIndex << endl;
}
if (rightDelayIndex > 0 ) {
   cerr << "Error in left " << leftDelayIndex << endl;
}

}



//////////////////////////////
//
// Pan2::getHeadSize
//

sampleType Pan2::getHeadSize(void) {
   return inputs[4];
}



//////////////////////////////
//
// Pan2::getSignal
//

sampleType Pan2::getSignal(void) {
   return inputs[0];
}



//////////////////////////////
//
// Pan2::getSpeedOfSound
//

sampleType Pan2::getSpeedOfSound(void) {
   return inputs[3];
}



//////////////////////////////
//
// Pan2::getX
//

sampleType Pan2::getX(void) {
   return inputs[1];
}



//////////////////////////////
//
// Pan2::getY
//

sampleType Pan2::getY(void) {
   return inputs[2];
}



//////////////////////////////
//
// Pan2::output
//

sampleType Pan2::output(int channel) {
   switch (channel) {
      case 0:
         return outputValueLeft;
         break;
      case 1:
         return outputValueRight;
         break;
      case 2:
         return distanceToLeftEar;
         break;
      case 3:
         return distanceToRightEar;
         break;
   }
   return 0.0;
}



//////////////////////////////
//
// Pan2::setHeadSize
//

void Pan2::setHeadSize(sampleType aHeadSize) {
   connect(aHeadSize/2.0, 4);
}



//////////////////////////////
//
// Pan2::setSpeedOfSound
//

void Pan2::setSpeedOfSound(sampleType aSpeed) {
   connect(aSpeed, 3);
}



//////////////////////////////
//
// Pan2::setX
//

void Pan2::setX(sampleType anX) {
   connect(anX, 1);
}



//////////////////////////////
//
// Pan2::setY
//

void Pan2::setY(sampleType aY) {
   connect(aY, 2);
}



// md5sum:	00ecde2bbeda7f5040206bae24bc3ffb  Pan2.cpp
