//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Mar  3 03:31:22 PST 1999
// Last Modified: Wed Mar  3 11:50:51 PST 1999
// Filename:      .../Layer/Layer.h
// Syntax:        C++ 
//

#ifndef _LAYER_H_INCLUDED
#define _LAYER_H_INCLUDED

#include "NetBase.h"
#include "LayerInput.h"

typedef double (*ActFunc)(double x);


class Layer : public NetBase {
   public:
                    Layer          (const char* aName = NULL);
                    Layer          (int size, const char* aName = NULL);
                    Layer          (double* contents, int aSize, 
                                                const char* aName = NULL);
                    Layer          (char* contents, int aSize, 
                                                const char* aName = NULL);
                   ~Layer          ();

      void          activate       (void);
      void          apply          (ActFunc aFunction);

      int           connect        (const Layer& aLayer,Weight* weights = NULL);
      int           connect        (const Layer& aLayer, int index, 
                                                       Weight* weights = NULL);
      int           connect        (Layer* aLayer, Weight* weights = NULL);
      int           connect        (Layer* aLayer, int index, 
                                                       Weight* weights = NULL);
      int           connectDirect  (const Layer& aLayer, int aScale = 1);
      int           connectDirect  (const Layer& aLayer, int index, 
                                                         int aScale = 1);
      int           connectDirect  (Layer* aLayer, int aScale = 1);
      int           connectDirect  (Layer* aLayer, int index, int aScale = 1);

      int           disconnect     (const Layer& aLayer);
      int           disconnect     (Layer* aLayer);
      int           disconnect     (int index);

      void          freeze         (void);
      int           getEpoch       (void) const;
      int           getSize        (void) const;
      void          setContents    (const double* contents, int aSize);
      void          setContents    (const char* contents, int aSize);
      void          setEpoch       (int anEpoch);
      void          setActivation  (ActFunc aFunction);
      void          unfreeze       (void);

      static double logistic       (double x);
      static double identity       (double x);

   protected:
      LayerInput inputs;        // handling of Layer inputs
      int        epoch;         // for keeping track of activation count
      int        frozenQ;       // freezing or unfreezing of layer output
      ActFunc    actfunc;       // pointer to activation function

      int        actNumber;     // for preventing activation loops
      static int nextActNumber; // preventing activation loops between networks

   private:
      void     activatePrivate         (int epoch);
      void     applyActivationFunction (Matrix<double>& preAct);

};



#endif  // _LAYER_H_INCLUDED



// md5sum:	f52100fd01fca34d39d907fd06a5e7e2  Layer.h
