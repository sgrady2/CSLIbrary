// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri May  2 15:55:32 GMT-0800 1997
// Last Modified: Fri Nov 28 05:46:58 GMT-0800 1997
// Filename:      .../sig/code/Generator/Envelope/Envelope.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#include "Envelope.h"
#include <iostream.h>
#include <ctype.h>
#include <math.h>



//////////////////////////////
//
// Envelope::Envelope
//	default value: aString = "(0 1; 1 1)", durationInSamples = 0
//
 
Envelope::Envelope(const char* aString, int durationInSamples) :
      EnvelopeString(aString) {
   if (durationInSamples < 0) {
      cerr << "Error: cannot have a negative duration: " 
           << durationInSamples << " in envelope." << endl;
      exit(1);
   }
   currPoint = -1;
   currSample = 0;
   active = 0;
   setDurationSamples(durationInSamples);
   setName("Envelope");
   outputValue = points[1][0];
   brandname = GENERATOR;
   trigger(0);
}



//////////////////////////////
//
// Envelope::~Envelope
//

Envelope::~Envelope() {
   // do nothing
}



//////////////////////////////
//
// Envelope::action
//

void Envelope::action(void) {
   if (activeQ()) {
      calculateNextSample();
   }
}



//////////////////////////////
//
// Envelope::activeQ
//

int Envelope::activeQ(void) {
   return active;
}



//////////////////////////////
//
// Envelope::getDurationSamples
//

long Envelope::getDurationSamples(void) {
   return sampleTimes[getNumPoints()-1];
}



//////////////////////////////
//
// Envelope::getDurationSeconds
//

double Envelope::getDurationSeconds(void) {
   return ((double)getDurationSamples() / getSrate());
}



//////////////////////////////
//
// Envelope::getEnvelopeSamples  -- returns the size of the
//    envelope as entered by the user in the envelope string.
//    Only important if there is a stick point in the string.
//

long Envelope::getEnvelopeSamples(void) {
   switch (tolower(absoluteType)) {
      case 'm':
         return (long)normalization;
         break;
      case 't':
         return (long)(normalization * getSrate() + 0.5);
         break;
      default:
         cerr << "Error: unknown timing type in envleope: " 
              << absoluteType << endl;
         exit(1);
   }

   // include the following line for stupid Visual C++ 5.0 compiler.
   // this line should never be reached.
   return 0;
}



//////////////////////////////
//
// Envelope::output
//

sampleType Envelope::output(int channel) {
   return outputValue;
}



//////////////////////////////
//
// Envelope::printState(void)
//

void Envelope::printState(void) {
   cerr << "Envelope: " << getEnvelope() << endl;
   cerr << "Srate:    " << getSrate() << endl;
   cerr << "current:  " << currSample << " samples" << endl;
   cerr << "duration: " << sampleTimes[sampleTimes.getSize()-1] << endl;
   this->print();
}



//////////////////////////////
//
// Envelope::setDuration
//


void Envelope::setDuration(long newDuration) {
   setDurationSamples(newDuration);
}

void Envelope::setDuration(int newDuration) {
   setDurationSamples(newDuration);
}

void Envelope::setDuration(double newDuration) {
   setDurationSeconds(newDuration);
}



//////////////////////////////
//
// Envelope::setDurationSamples
//

void Envelope::setDurationSamples(long newDuration) {
   int i;
   sampleTimes.setSize(getNumPoints());

   if (getStickIndex() >= 0 && newDuration > getEnvelopeSamples()) {
      // stickpoint: stretch stick region
      for (i=0; i<getNumPoints()-1; i++) {
         if (points[0][i] >= 0) {
            sampleTimes[i] = (long)(getEnvelopeSamples() * points[0][i] + 0.5);
         } else {
            sampleTimes[i] = (long)(newDuration + 0.5 + getEnvelopeSamples() * 
               points[0][i]);
         }
      }
      sampleTimes[getNumPoints()-1] = newDuration;
   } else {
      // no stickpoint: stretch whole envelope
      for (i=0; i<getNumPoints()-1; i++) {
         if (points[0][i] >= 0) {
            sampleTimes[i] = (long)(newDuration * points[0][i] + 0.5);
         } else {
            sampleTimes[i] = (long)(newDuration * (1.0 + points[0][i]) + 0.5);
         }
      sampleTimes[getNumPoints()-1] = newDuration;
      }
   }

   trigger(0);

   if (newDuration == 0) {
      active = 0;
   }
}



//////////////////////////////
//
// Envelope::setDurationSeconds
//

void Envelope::setDurationSeconds(double newDuration) {
   setDurationSamples((long)(newDuration * getSrate() + 0.5));
}



//////////////////////////////
//
// Envelope::trigger
//	default value: startPoint = 0
//

void Envelope::trigger(int startPoint) {
   if (startPoint < 0  || startPoint >= getNumPoints()) {
      cerr << "Invalid access point " << startPoint << " for envelope: " 
           << getEnvelope() << endl;
      exit(1);
   }
   active = 1;
   currPoint = startPoint;
   currSample = sampleTimes[startPoint];
   outputValue = points[1][startPoint];

   interpType = tolower(pointInterp[currPoint]);
   if (interpType == '\0') interpType = tolower(defaultInterpolation);

   switch (interpType) {
      case 'l':
         // determine linear increment
         if (currPoint < getNumPoints()-1) {
            linearIncrement = (points[1][currPoint+1] - points[1][currPoint])/
               (sampleTimes[currPoint+1] - sampleTimes[currPoint]);
         }
         break;
     case 'g': 
         // determine geometric increment
         if (currPoint < getNumPoints()-1) {
            if (points[1][currPoint+1] == 0.0 || points[1][currPoint] == 0.0) {
               cerr << "Cannot have a zero value in geometric interpolation." 
                    << endl;
               exit(1);
            } else if (points[1][currPoint+1]*points[1][currPoint] < 0.0) {
               cerr << "Cannot cross zero in geometric interpolation." 
                    << endl;
               exit(1);
            } else {
               geometricIncrement = 
                  pow((points[1][currPoint+1]/points[1][currPoint]),
                     1.0/(sampleTimes[currPoint+1] - sampleTimes[currPoint]));
            }
         }
         break;
      case 'c': 
         // don't bother with interpolation
         break;
      default:
         cerr << "An unknown interpolation type: \"" 
              << pointInterp[currPoint] << "\"" << endl;
         exit(1);
   }

}



///////////////////////////////////////////////////////////////////////////
//
// Private functions
//


//////////////////////////////
//
// Envelope::calculateNextSample

void Envelope::calculateNextSample(void) {

   currSample++;
   if (currSample >= getDurationSamples()) {
      outputValue = points[1][getNumPoints()-1];
      active = 0;
      return;
   } 

   if (currSample == sampleTimes[currPoint+1]) {
      // choosing new interval
      while (currSample >= sampleTimes[currPoint+1] && 
            currPoint < getNumPoints()-1) {
         currPoint++;
      }
      outputValue =  points[1][currPoint];
   
      // calculate interp increment
      interpType = tolower(pointInterp[currPoint]);
      if (interpType == '\0') interpType = tolower(defaultInterpolation);
      switch (interpType) {
         case 'l':
            // determine linear increment
            if (currPoint < getNumPoints()-1) {
               linearIncrement = (points[1][currPoint+1] -
                   points[1][currPoint])/ (sampleTimes[currPoint+1] - 
                   sampleTimes[currPoint]);
            }
            break;
         case 'g': 
            // determine geometric increment
            if (currPoint < getNumPoints()-1) {
               if (points[1][currPoint+1] == 0.0 || 
                     points[1][currPoint] == 0.0) {
                  cerr << "Cannot have a zero value in "
                       << "geometric interpolation." << endl;
                  exit(1);
               } else if (points[1][currPoint+1]*points[1][currPoint] < 0.0) {
                  cerr << "Cannot cross zero in geometric interpolation." 
                       << endl;
                  exit(1);
               } else {
                  geometricIncrement = 
                     pow((points[1][currPoint+1]/points[1][currPoint]),
                        1.0/(sampleTimes[currPoint+1] - 
                           sampleTimes[currPoint]));
               }
            }
            break;
         default:
            cerr << "Unknown interpolation type given: \"" 
                 << pointInterp[currPoint] << "\"" << endl;
            exit(1);
         }
   } else {
      switch (interpType) {
         case 'c':
            break;
         case 'l':
            outputValue += linearIncrement;
            break;
         case 'x':
            cerr << "Exponential interpolation not implemented yet" << endl;
            exit(1);
            break;
         case 'g':
            outputValue *= geometricIncrement;
            break;
         default:
            cerr << "Unknown interpolation type indicated: \"" 
                 << pointInterp[currPoint] << "\"" << endl;
            exit(1);
      }
   }

}


// md5sum:	a90ef7ad3e46cc254114db7367c16e7b  Envelope.cpp
