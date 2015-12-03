//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Feb 26 04:29:45 PST 1999
// Last Modified: Wed Mar  3 12:53:57 PST 1999
// Filename:      ...sigNet/LayerInput/LayerInput.h
// Syntax:        C++ 
//

#ifndef _LAYERINPUT_H_INCLUDED
#define _LAYERINPUT_H_INCLUDED

#include "Collection.h"
#include "Weight.h"

class Layer;


#define WEIGHT_OWNER_SELF              1
#define WEIGHT_OWNER_EXTERNAL          0

#define TYPE_LAYER_CONNECTION_NONE     0
#define TYPE_LAYER_CONNECTION_STANDARD 1
#define TYPE_LAYER_CONNECTION_DIRECT   2

class _NetConnectionData {
   public:
                    _NetConnectionData(void);
                   ~_NetConnectionData();

      Weight*        weights;       // pointer to weights
      int            ownerQ;        // if this is an external Matrix
      int            type;          // 0=not connections, 1=layer, 2=weight
      Layer*         layer;         // pointer to layer
      double         layerScale;    // constant layer gain if type 0
      Matrix<double> netresult;     // holder for matrix multiplication inputs
      int            updateValue;   // number of epochs between epochs
      int            currentUpdate; // current number of epoch update
};



class LayerInput : public NetBase {
   public:
                  LayerInput         (void);
                 ~LayerInput         ();
       
      void        activate           (Layer& output, int aNumber);
      int         getConnectionCount (void);
      Layer*      getConnection      (int index);
      int         getType            (int index);

      int         connect            (Layer* aLayer, Weight* weights = NULL);
      int         connect            (Layer* aLayer, int index, 
                                                     Weight* weights = NULL);
      int         connect            (const Layer& aLayer, 
                                                     Weight* weights = NULL);
      int         connect            (const Layer& aLayer, int index, 
                                                     Weight* weights = NULL);

      int         disconnect         (const Layer& aLayer);
      int         disconnect         (Layer* aLayer);
      int         disconnect         (int index);

      int         connectedQ         (int index);
      int         connectedQ         (Layer* aLayer);
      int         connectedQ         (const Layer& aLayer);
      double&     bias               (int index);
      Weight*     getWeight          (int index);
      double&     getWeight          (int index, int row, int column);

   protected:
      Collection<_NetConnectionData*> connections;
      Matrix<double> internalBias;
      Matrix<double> netresult;
};


#endif // _LAYERINPUT_H_INCLUDED


// md5sum:	ba5c1ab4e5f1afd31b81f362e8ed6a58  LayerInput.h
