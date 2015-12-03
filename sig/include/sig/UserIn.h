//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Apr 28 21:46:28 GMT-0800 1997
// Last Modified: Tue Apr 29 22:04:44 GMT-0800 1997
// Filename:      .../sig/code/Generator/UserIn/UserIn.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _USERIN_H_INCLUDED 
#define _USERIN_H_INCLUDED 


#include "Generator.h"


class UserIn : public Generator {
   public:
                      UserIn            (void);
                     ~UserIn            ();
                      
      void            action            (void);
      sampleType      output            (int channel);

   protected:
      sampleType      outputValue;
};



#endif  // _USERIN_H_INCLUDED 



// md5sum:	eeb102c4dcc062613cf8046b80a9d180  UserIn.h
