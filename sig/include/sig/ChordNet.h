//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Programmer:    Dan Gang <dang@ccrma.stanford.edu>
// Creation Date: Wed May  5 21:42:45 PDT 1999
// Last Modified: Sun May  9 14:49:50 PDT 1999
// Filename:      ...sig/maint/code/net/ChordNet.cpp
// Syntax:        C++; synthImprov 2.1; sigNet 1.0
//  
// Description:   For auto acompaniment of a simple melody.
//                Initial conversion of rule-based automatic accompaniment
//                to integrated version that includes automatic accompaniment
//                using a Jordan Neural Network
//

#ifndef _CHORDNET_H_INCLUDED
#define _CHORDNET_H_INCLUDED

#include "Matrix.h"

class ChordNet {
   public:  
                       ChordNet        (void);
                      ~ChordNet        ();

      void             initialize      (istream& initstream);
      void             activate        (void);

      // layers:
      Matrix<double> Output;
      Matrix<double> Hidden;
      Matrix<double> State;
      Matrix<double> Melody;
      Matrix<double> Meter;
      Matrix<double> Plan;

      Matrix<double> HiddenBias;
      Matrix<double> OutputBias;

      double StateDecay;
      double MelodyDecay;

      // connections:
      Matrix<double> HiddenOutput;
      Matrix<double> StateHidden;
      Matrix<double> MelodyHidden;
      Matrix<double> MeterHidden;
      Matrix<double> PlanHidden;

   private:
      Matrix<double> MelodyX;
      
      void           applyLogistic       (Matrix<double>& aLayer);
      void           limit               (Matrix<double>& aLayer, double value);

};


#endif // _CHORDNET_H_INCLUDED

// md5sum:	cd7a333584a079c654264a8384c1da36  ChordNet.h
