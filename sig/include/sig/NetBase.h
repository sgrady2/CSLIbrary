//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Feb 26 04:29:45 PST 1999
// Last Modified: Fri Feb 26 04:29:51 PST 1999
// Filename:      ...sigNet/NetBase/NetBase.h
// Syntax:        C++ 
//

#ifndef _NETBASE_H_INCLUDED
#define _NETBASE_H_INCLUDED

#include "Matrix.h"

class NetBase : public Matrix<double> {
   public:
                  NetBase       (const char* aName = NULL);
                  NetBase       (int size, const char* aName = NULL);
                  NetBase       (int rows, int colums,const char* aName = NULL);
                 ~NetBase       ();

      const char* getName       (void) const;
      void        setName       (const char* aString);
      void        setSize       (int aSize);
      void        seed          (double lowerlimit = 0.0, 
                                 double upperlimit = 1.0, int seed = 0);

   protected:
      char*       name;         // for storing name of element
      static int  namer;        // for naming unnamed elements
};



#endif // _NETBASE_H_INCLUDED



// md5sum:	76c61889f245de485867907f47736881  NetBase.h
