//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu> 
// Creation Date: Wed Feb 11 23:57:05 GMT-0800 1998
// Last Modified: Wed Feb 11 23:57:17 GMT-0800 1998
// Filename:      ...sig/code/control/improv/improv.h
// Web Address:   http://www-ccrma.stanford.edu/~craig/improv/include/improv.h
// Syntax:        C++
//
// Description:   Basic helper functions for use in all improv environments.
//

#ifndef _IMPROV_H_INCLUDED
#define _IMPROV_H_INCLUDED

#include "mididefines.h"

// include kitchen sink for users:

#ifdef VISUAL
   #include <windows.h>        /* for windowizims */
   #include <mmsystem.h>       /* multimedia functions */
#endif

#include <stdio.h>             /* for printf function (C syntax) */
#include <iostream.h>          /* for cout function (C++ syntax) */
#include <stdlib.h>            /* for the standard library functions */
#include <iomanip.h>           /* for stream formatting commands */
#include <math.h>              /* for some math functions */
#include <ctype.h>             /* for some character functions */

#include "sigControl.h"        /* for definitions of sig control classes */
#include "Options.h"           /* for command-line options */

// functions for the console version of the interface:
void displayInputPorts(void);
void displayOutputPorts(void);
void printboxtop(void);
     void printlinetop(void);
void printboxbottom(void);
     void printlinebottom(void);
void printintermediateline(void);
void printstringline(char* aString);
     void psl(char* aString);
void printcenterline(char* aString);
     void pcl(char* aString);


// useful functions common to all improv environments:
int         midiscale(int value, int min, int max);
const char* GeneralMidiName(int index);


#endif  // _IMPROV_H_INCLUDED


// md5sum:        96521303a5dd6dc23b6f60e7acde37a2  improv.h
