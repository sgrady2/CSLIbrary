//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Nov 26 14:12:01 PST 1999
// Last Modified: Fri Dec  2 13:26:44 PST 1999
// Filename:      ...sig/maint/code/info/MidiFile/MidiFile.h
// Web Address:   http://www-ccrma.stanford.edu/~craig/improv/include/MidiFile.h
// Syntax:        C++ 
//
// Description:   A class which can read/write Standard MIDI files.
//                MIDI data is stored by track in an array.  This
//                class is used for example in the MidiPerform class.
//

#ifndef _MIDIfILE_H_INCLUDED
#define _MIDIfILE_H_INCLUDED

#include "FileIO.h"
#include "Array.h"
#include "Collection.h"

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned long  ulong;

#define TIME_STATE_DELTA       0
#define TIME_STATE_ABSOLUTE    1

#define TRACK_STATE_SPLIT      0
#define TRACK_STATE_JOINED     1


class _MFEvent {
   public:
      _MFEvent (void);
      _MFEvent (int command);
      _MFEvent (int command, int param1);
      _MFEvent (int command, int param1, int param2);
      _MFEvent (int track, int command, int param1, int param2);
      _MFEvent (int aTime, int aTrack, int command, int param1, int param2);
     ~_MFEvent ();
      int time;
      int track;
      Array<uchar> data;
};



class MidiFile {
   public:
                MidiFile                 (void);
                MidiFile                 (char* aFile);
               ~MidiFile                 ();

      void      absoluteTime              (void);
      int       addTrack                  (void);
      void      deltaTime                 (void);
      void      deleteTrack               (int aTrack);
      void      erase                     (void);
      _MFEvent& getEvent                  (int aTrack, int anIndex);
      int       getTimeState              (void);
      int       getTrackState             (void);
      int       getTicksPerQuarterNote    (void);
      int       getTrackCount             (void);
      int       getNumTracks              (void);
      int       getNumEvents              (int aTrack);
      void      joinTracks                (void);
      void      mergeTracks               (int aTrack1, int aTrack2);
      int       read                      (char* aFile);
      void      sortTrack                 (Collection<_MFEvent>& trackData);
      void      sortTracks                (void);
      void      splitTracks               (void);
      int       write                     (char* aFile);

   protected:
      Collection<Collection<_MFEvent>*> events;  // midi file events
      int                   ticksPerQuarterNote; // time base of file
      int                   trackCount;          // # of tracks in file
      int                   theTrackState;       // joined or split
      int                   theTimeState;        // absolute or delta
      char*                 readFileName;        // read file name

   private:
      void      extractMidiData  (FileIO& inputfile, Array<uchar>& array, 
                                       uchar& runningCommand);
      ulong     extractVlvTime   (FileIO& inputfile);
      ulong     unpackVLV        (uchar a, uchar b, uchar c, uchar d, uchar e);
      void      writeVLValue     (long aValue, Array<uchar>& data);
};


int eventcompare(const void* a, const void* b);
ostream& operator<<(ostream& out, MidiFile& aMidiFile);

#endif // _MIDIfILE_H_INCLUDED



// md5sum:        21f879cae24831b973b0476c400ee299  MidiFile.h
