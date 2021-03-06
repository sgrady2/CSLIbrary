//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Jan 12 09:03:35 PST 1999
// Last Modified: Tue Jan 12 09:03:35 PST 1999
// Filename:      ...sig/maint/code/control/KeyboardInput/visual/KeyboardInput_visual.h
// Web Address:   http://www-ccrma.stanford.edu/~craig/improv/include/KeyboardInput_visual.h
// Syntax:        C++ 
//
// Description:   Controls the behaviour of the computer keyboard so
//                that individual keys from the keyboard can be read
//                immediately after they are pressed rather than when
//                the enter key is pressed.  Specific to DOS computers.
//


#ifndef _KEYBOARDINPUT_VISUAL_H_INCLUDED
#define _KEYBOARDINPUT_VISUAL_H_INCLUDED

#ifdef VISUAL


class KeyboardInput_visual {
   public:
                KeyboardInput_visual    (void);
               ~KeyboardInput_visual    ();

      int       hit                     (void);
      int       getch                   (void);
      void      newset                  (void);
      void      oldset                  (void);
      void      reset                   (void);

   protected:
      static int             classCount;
      static struct termio   current_io;
      static struct termio   original_io;

   private:
      void      initialize               (void);
      void      deinitialize             (void);
 

};

#endif  // VISUAL

#endif  // _KEYBOARDINPUT_VISUAL_H_INCLUDED



// md5sum:        00f6e297dd7b791905f014190f2b04ac  KeyboardInput_visual.h
