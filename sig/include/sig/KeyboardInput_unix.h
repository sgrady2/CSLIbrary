//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Jan 12 09:03:35 PST 1999
// Last Modified: Tue Jan 12 09:03:35 PST 1999
// Filename:      ...sig/maint/code/control/KeyboardInput/unix/KeyboardInput_unix.h
// Web Address:   http://www-ccrma.stanford.edu/~craig/improv/include/KeyboardInput_unix.h
// Syntax:        C++ 
// Reference:     http://cowf.cc.utexas.edu/~apoc/programs/c/kbhit.c
//
// Description:   Controls the behaviour of the computer keyboard so
//                that individual keys from the keyboard can be read
//                immediately after they are pressed rather than when
//                the enter key is pressed.  Specific to UNIX computers.
//


#ifndef _KEYBOARDINPUT_UNIX_H_INCLUDED
#define _KEYBOARDINPUT_UNIX_H_INCLUDED

#ifndef VISUAL

#include <termio.h>

class KeyboardInput_unix {
   public:
                KeyboardInput_unix      (void);
               ~KeyboardInput_unix      ();

      int       hit                     (void);
      int       getch                   (void);
      void      newset                  (void);
      void      oldset                  (void);
      void      reset                   (void);

   protected:
      int                    termioset;
      static int             classCount;
      static struct termio   current_io;
      static struct termio   original_io;

   private:
      void      initialize               (void);
      void      deinitialize             (void);
 

};

#endif  // VISUAL

#endif  // _KEYBOARDINPUT_UNIX_H_INCLUDED



// md5sum:        710f31f7e32789cc2313472cbef471fa  KeyboardInput_unix.h
