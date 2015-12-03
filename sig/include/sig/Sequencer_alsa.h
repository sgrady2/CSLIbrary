//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu May 11 21:06:21 PDT 2000
// Last Modified: Thu May 11 21:06:25 PDT 2000
// Filename:      ...sig/maint/code/control/MidiOutPort/Sequencer_alsa.h
// Web Address:   http://sig.sapp.org/include/sig/Sequencer_alsa.h
// Syntax:        C++ 
//
// Description:   Basic MIDI input/output functionality for the 
//                Linux ALSA midi device /dev/snd/midiXX.  This class
//                is inherited by the classes MidiInPort_alsa and
//                MidiOutPort_alsa.
//

#ifndef _SEQUENCER_ALSA_H_INCLUDED
#define _SEQUENCER_ALSA_H_INCLUDED

#ifdef ALSA

#include <iostream.h>
#include <sys/asoundlib.h>
#include "Collection.h"

#define MIDI_EXTERNAL  (1)
#define MIDI_INTERNAL  (2)

typedef unsigned char uchar;


class Sequencer_alsa {
   public:
                    Sequencer_alsa       (int autoOpen = 1);
                   ~Sequencer_alsa       ();

      void          close                (void);
      void          displayInputs        (ostream& out = cout, 
                                            char* initial = "\t");
      void          displayOutputs       (ostream& out = cout, 
                                            char* initial = "\t");
      static const char*   getInputName  (int aDevice);
      static const char*   getOutputName (int aDevice);
      static int    getNumInputs         (void);
      static int    getNumOutputs        (void);
      int           is_open              (int mode, int index);
      int           is_open_in           (int index);
      int           is_open_out          (int index);
      int           open                 (void);
      void          read                 (int dev, uchar* buf, int count);
      void          rebuildInfoDatabase  (void);
      int           write                (int aDevice, int aByte);
      int           write                (int aDevice, uchar* bytes, int count);
      int           write                (int aDevice, char* bytes, int count);
      int           write                (int aDevice, int* bytes, int count);
      
      int           getInCardValue       (int aDevice) const;
      int           getOutCardValue      (int aDevice) const;
   protected:
      static int    class_count;            // number of existing classes using
      static int    indevcount;             // number of MIDI input devices
      static int    outdevcount;            // number of MIDI output devices
      static int    initialized;            // for starting buileinfodatabase

      static Collection<snd_rawmidi_t*> Sequencer_alsa::rawmidi_in;
      static Collection<snd_rawmidi_t*> Sequencer_alsa::rawmidi_out;
      static Collection<int> Sequencer_alsa::midiincard;
      static Collection<int> Sequencer_alsa::midioutcard;
      static Collection<int> Sequencer_alsa::midiindevice;
      static Collection<int> Sequencer_alsa::midioutdevice;
      static Collection<char*> Sequencer_alsa::midiinname;
      static Collection<char*> Sequencer_alsa::midioutname;

   private:
      static void   buildInfoDatabase     (void);
      static int    getFd                 (void);   
      int           getInDeviceValue      (int aDevice) const;
      int           getInputType          (int aDevice) const;
      int           getOutDeviceValue     (int aDevice) const;
      int           getOutputType         (int aDevice) const;
      void          removeInfoDatabase    (void);


   friend void *interpretMidiInputStreamPrivateALSA(void * x);

};

#else  /* ALSA is not defined */

typedef unsigned char uchar;

class Sequencer_alsa {
   public:
                    Sequencer_alsa       (int autoOpen = 1) { }
                   ~Sequencer_alsa       () { }

      void          close                (void) { };
      void          displayInputs        (ostream& out = cout, 
                                            char* initial = "\t") 
                                         { out << initial << "NONE\n"; }
      void          displayOutputs       (ostream& out = cout, 
                                            char* initial = "\t") 
                                         { out << initial << "NONE\n"; }
      static const char*   getInputName  (int aDevice) { return ""; }
      static const char*   getOutputName (int aDevice) { return ""; }
      static int    getNumInputs         (void) { return 0; }
      static int    getNumOutputs        (void) { return 0; }
      int           is_open              (int mode, int index) { return 0; }
      int           is_open_in           (int index) { return 0; }
      int           is_open_out          (int index) { return 0; }
      int           open                 (void) { return 0; }
      void          read                 (int dev, uchar* buf, int count) { }
      void          rebuildInfoDatabase  (void) { }
      int           write                (int aDevice, int aByte) { return 0; }
      int           write                (int aDevice, uchar* bytes, int count) { return 0; }
      int           write                (int aDevice, char* bytes, int count) { return 0; }
      int           write                (int aDevice, int* bytes, int count) { return 0; }
      int           getInCardValue       (int aDevice) const { return 0; }
      int           getOutCardValue      (int aDevice) const { return 0; }

};


#endif ALSA




#endif  // _SEQUENCER_ALSA_H_INCLUDED


// md5sum:	a46bcd4109ccbcace03730d1696e18ac  Sequencer_alsa.h
