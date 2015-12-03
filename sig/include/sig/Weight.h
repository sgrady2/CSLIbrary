//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Feb 26 04:29:45 PST 1999
// Last Modified: Fri Feb 26 04:29:51 PST 1999
// Filename:      ...sigNet/Weight/Weight.h
// Syntax:        C++ 
//

#ifndef _WEIGHT_H_INCLUDED
#define _WEIGHT_H_INCLUDED

#include "NetBase.h"

class Weight : public NetBase {
   public:
                  Weight         (void);
                  Weight         (int rows, int columns);
                 ~Weight         ();

   protected:

};



#endif // _WEIGHT_H_INCLUDED


// md5sum:	ab2bc372ace32ea1eeb5f02703765bdc  Weight.h
