//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed May 14 13:37:00 GMT-0800 1997
// Last Modified: Wed May 14 22:50:33 GMT-0800 1997
// Filename:      ...sig/code/Filter/Resonator/Resonator.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cc -I../../../include && rm -f %b.o
//

#include "Resonator.h"
#include <stdlib.h>
#include <math.h>


#define FREQUENCY_FORM (0)     /* inputs[1] is a resonance in Hz */
#define NORM_ANGULAR_FORM (1)  /* inputs[1] is a norm ang freq of pole */

#define RADIUS_FORM (0)        /* inputs[2] is a z-plane radius */
#define BANDWIDTH_FORM (1)     /* inputs[2] is a bandwidth in Hz */


//////////////////////////////
//
// Resonator::Resonator
//	default values: aFrequency = 0.0, aBandwidth = 100.0
//

Resonator::Resonator(sampleType aFrequency, sampleType aBandwidth) {
   out.setSize(4);
   setHzForm();
   setBandwidthForm();
   setFrequency(aFrequency);  // this line has to come before setBandwidth
   setBandwidth(aBandwidth);
   setName("Resonator");
   brandname = FILTER;
}



//////////////////////////////
//
// Resonator::~Resonator
//

Resonator::~Resonator() { }



//////////////////////////////
//
// Resonator::action
//

void Resonator::action(void) {
  static sampleType R;
  static sampleType ang;

  R = makeR();
  ang = makeAng(R);

   out.write((1-R*R)*sin(ang)*inputs[0] + 2*R*cos(ang)*out[0] - R*R*out[-1]);
}




//////////////////////////////
//
// Resonator::getFrequency
//

sampleType Resonator::getFrequency(void) {
   return frequency;
}



//////////////////////////////
//
// Resonator::getBandwidth
//

sampleType Resonator::getBandwidth(void) {
   return (1 - radius) * getSrate() / PI;
}



//////////////////////////////
//
// Resonator::getOmega -- normalized angular frequency
//

sampleType Resonator::getOmega(void) {
   return angularFrequency / PI;
}



//////////////////////////////
//
// Resonator::getPoleRadius
//

sampleType Resonator::getPoleRadius(void) {
   return radius;
}



//////////////////////////////
//
// Resonator::output
//

sampleType Resonator::output(int channel) {
   return out[0];
}



//////////////////////////////
//
// Resonator::setBandwidth
//

void Resonator::setBandwidth(sampleType aBandwidth) {
   radius = 1 - TWOPI * aBandwidth / getSrate();
}



//////////////////////////////
//
// Resonator::setBandwidthForm
//

void Resonator::setBandwidthForm(void) {
   radiusForm = BANDWIDTH_FORM;
}


//////////////////////////////
//
// Resonator::setFrequency
//

void Resonator::setFrequency(sampleType aFrequency) {
   frequency = aFrequency;
   angularFrequency = acos( (2*radius)/(1+radius*radius) * 
      cos( TWOPI * aFrequency / getSrate()) );
}



//////////////////////////////
//
// Resonator::setHzForm
//

void Resonator::setHzForm(void) {
   frequencyForm = FREQUENCY_FORM;
}



//////////////////////////////
//
// Resonator::setOmega
//

void Resonator::setOmega(sampleType anOmega) {
   angularFrequency = PI * anOmega;

cerr << "angularFrequency is: " << angularFrequency << endl;

}



//////////////////////////////
//
// Resonator::setOmegaForm
//

void Resonator::setOmegaForm(void) {
   frequencyForm = NORM_ANGULAR_FORM;
}



//////////////////////////////
//
// Resonator::setPoleRadius
//

void Resonator::setPoleRadius(sampleType aPoleRadius) {
   radius = aPoleRadius;
   updateFrequency();
}



//////////////////////////////
//
// Resonator::setRadiusForm
//

void Resonator::setRadiusForm(void) {
   radiusForm = RADIUS_FORM;
   updateFrequency();
}


////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// Resonator::makeAng
//

sampleType Resonator::makeAng(sampleType aRadius) {
   if (inputs.connectionQ(1)) {
      if (frequencyForm == NORM_ANGULAR_FORM) {
         return PI * inputs[1];
      } else {  // FREQUENCY_FORM
         return acos(2 * aRadius / (1 + aRadius * aRadius) * 
            cos(TWOPI * inputs[1] / getSrate()));
      }
   } else {
      return angularFrequency;
   }
}


//////////////////////////////
//
// Resonator::makeR
//

sampleType Resonator::makeR(void) {
   if (inputs.connectionQ(2)) {
      if (radiusForm == RADIUS_FORM) {
         return inputs[2];
      } else {  // BANDWIDTH_FORM
         return 1 - PI * inputs[2] / getSrate(); 
      }
   } else {
      return radius;
   }
}



//////////////////////////////
//
// Resonator::updateFrequency

void Resonator::updateFrequency(void) {
//   angularFrequency = acos( (2*radius)/(1+radius*radius) * 
//      cos( TWOPI * frequency / getSrate()) );
}
   
// md5sum:	f131204b16bb471764c16eab054988ba  Resonator.cpp
