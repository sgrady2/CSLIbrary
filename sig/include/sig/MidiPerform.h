//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Nov 27 14:00:11 PST 1999
// Last Modified: Wed Dec  1 11:35:57 PST 1999
// Filename:      ...sig/maint/code/control/MidiPerform/MidiPerform.h
// Web Address:   http://www-ccrma.stanford.edu/~craig/improv/include/MidiPerform.h
// Syntax:        C++ 
//
// Description:   A class which performs a MIDI file with various
//                types of tempo and volume controls built in.
//                Works similar to a sequencer program, but not designed
//                for editing of the data.
//

#ifndef _MIDIPERFORM_H_INCLUDED
#define _MIDIPERFORM_H_INCLUDED

#include "MidiFile.h"
#include "CircularBuffer.h"
#include "SigTimer.h"
#include "MidiOutput.h"

#define TEMPO_METHOD_AUTOMATIC 0
#define TEMPO_METHOD_CONSTANT  1
#define TEMPO_METHOD_ONEBACK   2
#define TEMPO_METHOD_TWOBACK   3
#define TEMPO_METHOD_THREEBACK 4
#define TEMPO_METHOD_FOURBACK  5


class MidiPerform : public MidiOutput {
   public:
                MidiPerform           (void);
                MidiPerform           (char* aFile);
               ~MidiPerform           ();

      void      beat                  (void);
      double    getAmp                (void);
      int       getMaxAmp             (void);
      int       getTempoMethod        (void);
      double    getTempo              (void);
      int       channelCollapse       (int aSetting = -1);
      void      check                 (void);
      double    getBeatFraction       (void);
      int       getBeatLocation       (void);
      void      pause                 (void);
      void      play                  (void);
      void      read                  (char* aFile);
      void      rewind                (void);
      void      setAmp                (double anAmp);
      void      setMaxAmp             (int aMax);
      void      setBeatLocation       (double aLocation);
      void      setTempoMethod        (int aMethod);
      void      setTempo              (double aTempo);
      void      stop                  (void);

   protected:
      MidiFile                        midifile;
      double                          tempo;
      SigTimer                        performanceTimer;
      SigTimer                        beatTimer;
      SigTimer                        millisecTimer;
      double                          pauseLocation;
      int                             playingQ;   
      CircularBuffer<double>          beatTimes;
      int                            *readIndex;
      int                             tempoMethod;     
      double                          amp;
      int                             maxamp;
      int                             channelcollapseQ;

   private:
      double       getAverageTempo    (int count);
      void         beat_tracktempo    (int watchhistory);
      void         beat_automatic     (void);
      void         beat_constant      (void);
};


#endif // _MIDIPERFORM_H_INCLUDED



// md5sum:        d5d5cc1e7e0da2b209592c4796986c64  MidiPerform.h
