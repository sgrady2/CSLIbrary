//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Nov  2 10:36:12 PST 1996
// Last Modified: Sun Mar 15 22:30:33 GMT-0800 1998
// Filename:      ...sig/code/misc/sigDefines.h
// Web Address:   http://www-ccrma.stanford.edu/~craig/improv/include/sigDefines.h
// Syntax:        C++ 
//
// Description:   Basic defines for all sig-related classes and code.
//

#ifndef _SIGDEFINES_H_INCLUDED
#define _SIGDEFINES_H_INCLUDED


#define SIG_VERSION "sig version: Sun Apr 19 21:35:05 PDT 1998."


#define GENERATOR            (1)
#define FILTER               (2)


#define NONE                 (-1)

#ifndef PI
   #define PI                (3.1415926535897932384)
#endif
#define TWOPI                (6.2831853)
#define HALFPI               (1.5707963)
#define SND_FORMAT_LINEAR_16 (3)


#endif  // _SIGDEFINES_H_INCLUDED



// md5sum:        f5dee005048365e35bb4de5fc0caf835  sigDefines.h
