//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Programmer:    Dan Gang <dang@ccrma.stanford.edu>
// Creation Date: Wed May  5 21:42:45 PDT 1999
// Last Modified: Sun May  9 14:49:50 PDT 1999
// Last Modified: Thu Jul  1 16:17:54 PDT 1999
// Filename:      ...sig/maint/code/net/ChordNet.cpp
// Syntax:        C++; synthImprov 2.1; sigNet 1.0
//  
// Description:   For auto acompaniment of a simple melody.
//                Initial conversion of rule-based automatic accompaniment
//                to integrated version that includes automatic accompaniment
//                using a Jordan Neural Network
//

#include "ChordNet.h"
#include <math.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////
//
// neural network weightings
//

//
// Programmer:   Dan Gang <dang@ccrma.stanford.edu>
// Creation Date: August 1998
// Filename:      NaNa.h
// Syntax:        C++; 
//  
// Description: NaNa.h contains weights for NN and functions 
// Syntax:        C++
//
// Description: Basic functions for use in NaNa environments
//


#ifndef _NaNa_H_INCLUDED
#define _NaNa_H_INCLUDED

#define pc0 "a00000000000"
#define pc1 "0a0000000000"
#define pc2 "00a000000000"
#define pc3 "000a00000000"
#define pc4 "0000a0000000"
#define pc5 "00000a000000"
#define pc6 "000000a00000"
#define pc7 "0000000a0000"
#define pc8 "00000000a000"
#define pc9 "000000000a00"
#define pc10 "0000000000a0"
#define pc11 "00000000000a"

//command set weight 0.5 max 1 min 0 eta 0.2 alpha 0.9
//command network  Nplan=5 Nout=15 Nhid=0 Nhiddis=12 Tseq=32 Nts=6 n.jor -D -I -s112119137
//command pattern /CS/phd/dang/NaNa/input/corpus/learncorpusex11a.nn -D

// step=20 Error=0.015537
//# Connection PlanHidden with Bias of Hidden  (-1 x 12) (-1==symmetric)
double PlanHidden[12][5] = { 
	{-0.110869, 0.227794, 0.0450906, -0.081177, 0.253797},  
	{0.163479, -0.287842, 0.139535, 0.0840595, 0.839308 },
	{-0.019756, -0.316708, -0.41276, 1.17794, -0.0840777},  
	{-0.142948, -0.495775, 0.207978, -0.034976, -0.618306}, 
	{0.160378, -0.16194, 0.349515, -0.715392, 0.268005, },
	{-0.521222, 0.0602619, 0.1577, 0.0548997, 0.693842  }, 
	{0.429725, -0.461931, 0.873766, -0.126842, -0.0502923},  
	{-0.870206, -0.125856, 0.0192284, 0.0919152, 0.522328 }, 
	{-0.134457, -0.250674, 0.0850061, -1.24671, -0.108505 },
	{0.490738, 0.0999509, 0.0548701, 0.476875, -0.578738 },
	{1.20837, 0.0509074, -0.243084, 0.313088, 0.401644 },
	{0.243544, 0.736368, 0.811133, -0.00132401, -0.417897}
};

double HiddenBias[12] = {
	5.07745, 0.430052, -2.14818, 0.500213, 0.790463, -4.19291, 0.537896, 5.48546, -0.0728183, -0.209303, 0.597441, -5.54316 
};

//# Connection StateHidden with Bias of Hidden  (15 x 12) (-1==symmetric)
double StateHidden[12][15] = { 
	{-6.25916, 3.53436, 0.225686, 6.00141, 0.663964, -0.279341, 0.630276, 0.811073, 0.236478, -0.450726, 0.0728002, 0.91743, 0.869966, 0.126213, -0.50082 }, 
	{-0.523304, -0.413452, 0.636293, -1.60306, 0.426708, -0.78121, 0.0736788, 0.657092, -0.322298, -0.481418, -0.532722, 0.470316, -0.0880986, -0.0726384, 0.205179 }, 
	{-7.38226, -2.65139, -0.296368, 1.18521, -1.04151, -1.17005, 0.216592, 0.614409, -0.141119, -0.155214, 0.215494, 2.51292, 0.302868, -1.56932, 0.0978145 }, 
	{0.96748, 0.210994, -0.794036, 0.867346, 0.195659, 0.811862, 0.413285, -0.471887, -0.165674, 0.220222, 0.738613, 0.532882, 0.18453, 0.0614014, 0.336979 }, 
	{-1.84712, 2.8846, 0.387987, -7.84582, -2.74795, 0.324158, 0.487652, -0.0158103, -1.29422, 0.530356, 0.95928, -1.23252, 0.657988, 1.11435, 0.00110851 }, 
	{-1.27297, 0.463047, -0.750701, -6.52179, -0.881521, 0.534078, -1.39542, -0.504337, -0.0960159, -1.28044, -0.478955, -1.43168, -1.36878, -1.10022, 0.0163745 }, 
	{0.197877, -0.305174, 0.697357, -0.116508, 0.5722, 0.309311, -0.574932, 0.194083, 0.0964487, 0.218096, 0.263561, -0.38176, -0.232303, -0.164412, 0.866864 },
	{-8.83271, 0.0726604, 0.185702, -0.339338, -1.18612, 0.511163, 0.203193, 1.31644, 1.02667, 0.2895, -0.126404, 5.83559, -0.189131, -0.426514, -0.00707513 }, 
	{0.00431213, -0.107275, -0.925541, -0.228402, -0.101798, -0.330313, -0.54684, -0.137107, 0.0448645, -0.456442, 0.354835, 0.14794, 0.3982, 0.522421, 0.224962 }, 
	{1.02149, -0.951009, 0.307559, -2.24209, 0.201602, 0.340349, 0.287066, 0.222049, 0.0222152, 0.220743, 1.40141, 2.79661, 0.0258969, 1.16287, 1.12383 },
	{0.776326, -0.418589, -0.289152, 0.553857, -0.365746, -0.667568, -0.427018, 0.696059, 0.193844, 0.2489, -0.188651, 0.240775, 0.0432631, -0.187957, 0.409431 }, 
	{3.8202, -1.09977, 0.432631, -4.55456, -1.936, -1.14194, 0.197818, -0.926726, 0.355415, -0.607688, 0.999192, 5.102, -0.832237, 0.773233, 0.152933 }
}; 

// Connection HiddenOutput with Bias of Output  (12 x 15) (-1==symmetric)
double HiddenOutput[15][12] = { 
	{4.86446, 0, 0, 0, 4.24623, 0, 0, 3.0403, 0, 0, 0, 0},
	{0, 0, 5.05959, 0, 0, 2.20201, 0, 0, 0, -1.58471, 0, 0 },
	{0, 0, 0, 0, -1.93705, 0, 0, -1.43268, 0, 0, 0, -1.72331 },
	{-4.60679, 0, 0, 0, 0, 2.95954, 0, 0, 0, 2.30961, 0, 0 },
	{0, 0, 0.365634, 0, 0, 0, 0, -2.4853, 0, 0, 0, -3.96126 }, 
	{-0.3599, 0, 0, 0, -0.917768, 0, 0, 0, 0, 1.26599, 0, 0 }, 
	{0, 0, -1.04573, 0, 0, -0.791553, 0, 0, 0, 0, 0, -3.38039 }, 
	{0.408348, 0, 0, 0, -0.0983262, 0, 0, -0.834229, 0, 0, -1.81853, 0 }, 
	{-2.17207, 0, -0.279525, 0, 0, 0, -2.12954, 0, 0, -2.23185, 0, 0 },
	{0, 0, -2.04806, 0, -2.4918, 0, 0, 0, -2.38638, 0, 0, -1.17062 },
	{-3.55088, 0, 0, -3.71494, 0, -0.816178, 0, 0, 0, -3.58611, 0, 0 }, 
	{0, 0, -3.53841, 0, 0, 2.50814, 0, -4.35304, 0, 0, 0, -5.20856 }, 
	{0, -1.561, 0, 0, -1.97327, 0, 0, -1.03698, 0, -2.45561, 0, 0 },
	{0, 0, -0.835238, 0, 0, -0.548672, 0, 0, 0, -3.89033, 0, -3.51307 }, 
	{-3.34737, 0, 0, 0, 0, 0.305974, 0, 0, -1.78247, 0, 0, 0 }
};


double OutputBias[15] = {
	-6.50719, -2.01908, -5.79082, -2.60568, -2.42228, -3.84337, -7.16777, -2.4693, -2.41108, 
         -3.24289, -6.20499, -1.24445, -4.35009, -6.13612, -4.70746};

// Connection KnHidden with Bias of Hidden  (12 x 12) (-1==symmetric)

double MelodyHidden[12][12] = { 
	{4, 0.637972, -8.78425, 0.68693, 7.48149, -8.1842, -3.58028, 6.97046, 0.253935, -7.83076, -0.409652, -5.09737 },
	{-0.201642, 4, 0.348902, 0.671262, 0.301975, -0.777951, -0.491686, 0.165233, 1.00441, -0.241964, -0.640973, 0.192884 }, 
	{-3.52856, -0.324552, 4, -0.359535, -3.5817, -1.23641, -0.199796, 0.858581, 0.782494, 2.09227, 0.31877, 4.04956 },
	{0.543101, -0.00831894, 0.372073, 4, -0.0887593, 0.0664709, -0.664752, -0.648517, -0.415968, 0.468224, -0.0569394, -0.135662 }, 
	{17.6129, -0.0906164, -13.4781, 0.29164, 4, -3.41517, -1.05172, 2.30007, -0.264236, -3.27885, -1.01118, -4.28732 }, 
	{-1.34445, -0.167944, -1.69774, -1.00029, -3.77015, 4, -0.157005, -4.64475, 0.0441919, 6.27466, 0.17468, 2.33015 },
	{0.151232, 0.192797, -0.292717, 0.197381, 0.402297, 1.04536, 4, 0.0996696, 0.330434, -0.124837, -0.650434, 0.58225 }, 
	{2.48499, -0.562962, -8.56214, 0.198914, 7.5061, -8.01611, -1.16601, 4, -0.221735, 0.703366, -0.202944, -2.90192 }, 
	{0.292713, -0.0973846, -0.209418, -0.399686, -1.50163, 0.36459, 0.0110837, 0.309329, 4, 0.906916, -0.813961, -0.44201 }, 
	{4.72773, -0.113395, -8.15461, -0.254152, 1.96847, -0.0955737, 0.272752, 0.514554, -0.044006, 4, -0.144999, 0.349055 },
	{0.512744, 0.0553214, -0.132779, -1.12926, 0.0686145, -0.447304, 0.585091, -0.0926546, -0.0438791, 0.199281, 4, -0.398611 },
	{16.4605, 0.312709, -20.9976, 0.588206, 7.6178, -1.44414, 0.932811, 0.784706, 0.263818, 4.89194, -0.0197849, 4 }
};

//# Connection MeterHidden with Bias of Hidden  (6 x 12) (-1==symmetric)
double MeterHidden[12][6] = { 
	{1.87236, 0.207953, 4.0451, -2.58976, 1.80061, 4.16415}, 
	{-0.158514, 0.112169, 0.101961, 0.410933, 1.50648, 0.54636},
	{2.65661, -1.85419, -0.295774, -3.34658, -1.02516, -1.58411}, 
	{0.163087, 1.09452, -0.404038, -0.344055, 0.378398, 0.499057}, 
	{-1.53113, 0.349319, -4.70533, 1.35529, -1.01764, 3.67793}, 
	{2.96676, -1.52083, -3.38555, 1.63497, -1.97514, -1.03633},
	{0.560451, -0.0804922, -0.00839831, 0.977821, 1.23318, 0.158979}, 
	{-6.12408, 0.720502, -1.9644, -3.96331, 5.86143, -0.190665}, 
	{0.324034, 1.02972, 0.75472, -0.853834, 0.677922, 0.412272}, 
	{0.711322, -0.706233, -1.9898, -2.14145, -0.27628, 0.554657}, 
	{0.220567, 1.11932, 0.22355, 0.620373, 1.18222, 0.369802}, 
	{-1.28114, -1.39432, -6.85839, 3.40999, -4.47003, -1.3176}
}; 


#endif // _NaNa_H_INCLUDED

//////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// ChordNet::ChordNet --
//

ChordNet::ChordNet(void) {
      // set the size of the layers:
      Output.setSize(1, 15);          // 15
      Hidden.setSize(1, 12);          // 12
      State.setSize(1, 15);           // 15
      Melody.setSize(1, 12);          // 12
      MelodyX.setSize(1, 12);         // 12
      Meter.setSize(1, 6);            // 6
      Plan.setSize(1, 5);             // 5

      HiddenBias.setSize(1, 12);      // 12
      OutputBias.setSize(1, 15);      // 15

      StateDecay = 0.7;
      MelodyDecay = 0.5;

      // zero the current contents of the layers
      Output.zero(); 
      Hidden.zero(); 
      State.zero();  
      Melody.zero(); 
      MelodyX.zero(); 
      Meter.zero();  
      Plan.zero();   

      // set the size of the connections:
      HiddenOutput.setSize(15, 12);    // 12 x 15
      StateHidden.setSize(12, 15);     // 15 x 12
      MelodyHidden.setSize(12, 12);    // 12 x 12
      MeterHidden.setSize(12, 6);      //  6 x 12
      PlanHidden.setSize(12, 5);       //  5 x 12

      // fill the connection weight matrices:
      int i, j;

      for (i=0; i<12; i++) {
         HiddenBias[i] = ::HiddenBias[i];
      }

      for (i=0; i<15; i++) {
         OutputBias[i] = ::OutputBias[i];
      }
 
      for (i=0; i<15; i++) {
         for (j=0; j<12; j++) {
            HiddenOutput.cell(i, j) = ::HiddenOutput[i][j];
         }
      }

      for (i=0; i<12; i++) {
         for (j=0; j<15; j++) {
            StateHidden.cell(i, j) = ::StateHidden[i][j];
         }
      }


      for (i=0; i<12; i++) {
         for (j=0; j<12; j++) {
            MelodyHidden.cell(i, j) = ::MelodyHidden[i][j];
         }
      }

      for (i=0; i<12; i++) {
         for (j=0; j<6; j++) {
            MeterHidden.cell(i, j) = ::MeterHidden[i][j];
         }
      }

      for (i=0; i<12; i++) {
         for (j=0; j<5; j++) {
            PlanHidden.cell(i, j) = ::PlanHidden[i][j];
         }
      }

}


//////////////////////////////
//
// ChordNet::~ChordNet --
//

ChordNet::~ChordNet() { 
   // do nothing
}



//////////////////////////////
//
// ChordNet::activate -- produce the next Output layer values
//

void ChordNet::activate(void) { 
   State.multiply(StateDecay);
   State.add(Output);
   State.transpose();

   Hidden.zero();
   Hidden.transpose();
   
   Matrix<double>::multiply(Hidden, StateHidden, State);
   State.transpose();
   Hidden.transpose();

   Matrix<double> HiddenTemp(1, 12);

   MelodyX.multiply(MelodyDecay);
   MelodyX.add(Melody);

   HiddenTemp.transpose();
   MelodyX.transpose();
   Matrix<double>::multiply(HiddenTemp, MelodyHidden, MelodyX);
   MelodyX.transpose();
   HiddenTemp.transpose();
   Hidden.add(HiddenTemp);

   HiddenTemp.transpose();
   Meter.transpose();
   Matrix<double>::multiply(HiddenTemp, MeterHidden, Meter);
   Meter.transpose();
   HiddenTemp.transpose();
   Hidden.add(HiddenTemp);

   HiddenTemp.transpose();
   Plan.transpose();
   Matrix<double>::multiply(HiddenTemp, PlanHidden, Plan);
   Plan.transpose();
   HiddenTemp.transpose();
   Hidden.add(HiddenTemp);

   Hidden.add(HiddenBias);
   applyLogistic(Hidden);


   limit(Hidden, 0.003);

   Output.transpose();
   Hidden.transpose();
   Matrix<double>::multiply(Output, HiddenOutput, Hidden);
   Hidden.transpose();
   Output.transpose();

   Output.add(OutputBias);

   applyLogistic(Output);

   limit(Output, 0.003);
}



//////////////////////////////
//
// ChordNet::initialize --
//

void ChordNet::initialize(istream& initstream) { 
  // do nothing;
}


///////////////////////////////////////////////////////////////////////////
//
// private functions
//

//////////////////////////////
//
// ChordNet::applyLogistic --
//

void ChordNet::applyLogistic(Matrix<double>& aLayer) {
   for (int i=0; i<aLayer.getSize(); i++) {
      aLayer[i] = 1.0/(1.0 + exp(-aLayer[i]));
   }
}



//////////////////////////////
//
// ChordNet::limit -- limit
//

void ChordNet::limit(Matrix<double>& aLayer, double value) {
   for (int i=0; i<aLayer.getSize(); i++) {
      if (aLayer[i] < value) {
         aLayer[i] = value;
      } else if (aLayer[i] > 1.0 - value) {
         aLayer[i] = 1.0 - value;
      }
   }
}



// md5sum:	a6a526bf3858c4755b53989717f117af  ChordNet.cpp
