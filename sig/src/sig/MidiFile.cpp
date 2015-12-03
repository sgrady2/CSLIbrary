//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Nov 26 14:12:01 PST 1999
// Last Modified: Fri Dec  2 13:26:29 PST 1999
// Filename:      ...sig/maint/code/info/MidiFile/MidiFile.cpp
// Web Address:   http://sig.sapp.org/src/sig/MidiFile.cpp
// Syntax:        C++ 
//
// Description:   A class which can read/write Standard MIDI files.
//                MIDI data is stored by track in an array.  This
//                class is used for example in the MidiPerform class.
//

#include "MidiFile.h"
#include <iomanip.h>

//////////////////////////////
//
// _MFEvent::_MFEvent --
//

_MFEvent::_MFEvent(void) { 
   time = 0;
   track = 0;
   data.allowGrowth();
   data.setSize(0);
}

_MFEvent::_MFEvent(int command) { 
   time = 0;
   track = 0;
   data.allowGrowth();
   data.setSize(1);
   data[0] = (uchar)command;
}

_MFEvent::_MFEvent(int command, int param1) { 
   time = 0;
   track = 0;
   data.allowGrowth();
   data.setSize(2);
   data[0] = (uchar)command;
   data[1] = (uchar)param1;
}

_MFEvent::_MFEvent(int command, int param1, int param2) { 
   time = 0;
   track = 0;
   data.allowGrowth();
   data.setSize(3);
   data[0] = (uchar)command;
   data[1] = (uchar)param1;
   data[2] = (uchar)param2;
}

_MFEvent::_MFEvent(int aTrack, int command, int param1, int param2) { 
   time = 0;
   track = aTrack;
   data.allowGrowth();
   data.setSize(3);
   data[0] = (uchar)command;
   data[1] = (uchar)param1;
   data[2] = (uchar)param2;
}

_MFEvent::_MFEvent(int aTime, int aTrack, int command, int param1, int param2) {
   time = aTime;
   track = aTrack;
   data.allowGrowth();
   data.setSize(3);
   data[0] = (uchar)command;
   data[1] = (uchar)param1;
   data[2] = (uchar)param2;
}



//////////////////////////////
//
// _MFEvent::~MFEvent
//

_MFEvent::~_MFEvent() { 
   time = -1;
   track = -1;
   data.setSize(0);
}



//////////////////////////////
//
// MidiFile::MidiFile --
//

MidiFile::MidiFile(void) { 
   ticksPerQuarterNote = 48;             // time base of file
   trackCount = 1;                       // # of tracks in file
   theTrackState = TRACK_STATE_SPLIT;    // joined or split
   theTimeState = TIME_STATE_DELTA;      // absolute or delta
   events.setSize(1);
   events[0] = new Collection<_MFEvent>;
   events[0]->setSize(0);
   events[0]->allowGrowth(1);
   readFileName = new char[1];
   readFileName[0] = '\0';
}


MidiFile::MidiFile(char* aFile) { 
   ticksPerQuarterNote = 48;             // time base of file
   trackCount = 1;                       // # of tracks in file
   theTrackState = TRACK_STATE_SPLIT;    // joined or split
   theTimeState = TIME_STATE_DELTA;      // absolute or delta
   events.setSize(1);
   events[0] = new Collection<_MFEvent>;
   events[0]->setSize(0);
   events[0]->allowGrowth(1);
   readFileName = new char[1];
   readFileName[0] = '\0';
   read(aFile);
}



//////////////////////////////
//
// MidiFile::~MidiFile --
//

MidiFile::~MidiFile() { 
   if (readFileName != NULL) {
      delete [] readFileName;
      readFileName = NULL;
   }

   erase();

   if (events[0] != NULL) {  
      delete events[0];
      events[0] = NULL;
   }

}



//////////////////////////////
//
// MidiFile::absoluteTime -- convert the time data to
//    absolute time, which means that the time field
//    in the _MFEvent struct represents the exact tick
//    time to play the event rather than the time since
//    the last event to wait untill playing the current
//    event.
//

void MidiFile::absoluteTime(void) { 
   if (getTimeState() == TIME_STATE_ABSOLUTE) {
      return;
   }
   int i, j;
   int length = getNumTracks();
   int* timedata = new int[length];
   for (i=0; i<length; i++) {
      timedata[i] = 0;
      if (events[i]->getSize() > 0) {
         timedata[i] = (*events[i])[0].time;
      } else {
         continue;
      }
      for (j=1; j<events[i]->getSize(); j++) {
         timedata[i] += (*events[i])[j].time;
         (*events[i])[j].time = timedata[i];
      }
   }
   theTimeState = TIME_STATE_ABSOLUTE;
   delete [] timedata;
}



//////////////////////////////
//
// MidiFile::addTrack -- adds a blank track at end of the 
//    track list.  Returns the track number of the added
//    track.
//

int MidiFile::addTrack(void) { 
   int length = getNumTracks();
   events.setSize(length+1);
   events[length] = new Collection<_MFEvent>;
   events[length]->setSize(0);
   events[length]->allowGrowth(1);
   return length;
}



//////////////////////////////
//
// MidiFile::deleteTrack -- remove a track from the MidiFile.
//   Tracks are numbered starting at track 0.
//

void MidiFile::deleteTrack(int aTrack) { 
   int length = getNumTracks();
   if (aTrack < 0 || aTrack >= length) {
      return;
   }
   if (length == 1) {
      return;
   }
   delete events[aTrack];
   for (int i=aTrack; i<length-1; i++) {
      events[i] = events[i+1];
   }

   events[length] = NULL;
   events.setSize(length-1);
}



//////////////////////////////
//
// MidiFile::deltaTime -- convert the time data to
//     delta time, which means that the time field
//     in the _MFEvent struct represents the time
//     since the last event was played. When a MIDI file
//     is read from a file, this is the default setting.
//

void MidiFile::deltaTime(void) { 
   if (getTimeState() == TIME_STATE_DELTA) {
      return;
   }
   int i, j;
   int temp;
   int length = getNumTracks();
   int *timedata = new int[length];
   for (i=0; i<length; i++) {
      timedata[i] = 0;
      if (events[i]->getSize() > 0) {
         timedata[i] = (*events[i])[0].time;
      } else {
         continue;
      }
      for (j=1; j<events[i]->getSize(); j++) {
         temp = (*events[i])[j].time;
         (*events[i])[j].time = temp - timedata[i];
         timedata[i] = temp;
      }
   }
   theTimeState = TIME_STATE_DELTA;
   delete [] timedata;
}



//////////////////////////////
//
// MidiFile::erase -- make the MIDI file empty with one
//     track with no data in it.
//

void MidiFile::erase(void) { 
   int length = getNumTracks();
   for (int i=0; i<length; i++) {
      delete events[i];
      events[i] = NULL;
   }
   events.setSize(1);
   events[0] = new Collection<_MFEvent>;
   events[0]->setSize(0);
   events[0]->allowGrowth(1);
}



//////////////////////////////
//
// MidiFile::erase -- make the MIDI file empty with one
//     track with no data in it.
//

_MFEvent& MidiFile::getEvent(int aTrack, int anIndex) {
   return (*events[aTrack])[anIndex];
}



//////////////////////////////
//
// MidiFile::getTicksPerQuarterNote -- returns the number of
//   time units that are supposed to occur during a quarternote.
//

int MidiFile::getTicksPerQuarterNote(void) { 
   return ticksPerQuarterNote;
}



//////////////////////////////
//
// MidiFile::getTrackCount -- return the number of tracks in 
//   the Midi File.
//

int MidiFile::getTrackCount(void) { 
   return events.getSize();
}

int MidiFile::getNumTracks(void) { 
   return events.getSize();
}



//////////////////////////////
//
// MidiFile::getNumEvents -- returns the number of events
//   in a given track.
//

int MidiFile::getNumEvents(int aTrack) { 
   return events[aTrack]->getSize();
}



//////////////////////////////
//
// MidiFile::joinTracks -- merge the data from all tracks,
//   but keeping the identity of the tracks unique so that
//   the function splitTracks can be called to split the 
//   tracks into separate units again.  The style of the 
//   MidiFile when read from a file is with tracks split.
//

void MidiFile::joinTracks(void) { 
   if (getTrackState() == TRACK_STATE_JOINED) {
      return;
   }
   if (getNumTracks() == 1) {
      return;
   }

   Collection <_MFEvent>* joinedTrack;
   joinedTrack = new Collection<_MFEvent>;
   joinedTrack->setSize(0);
   int oldTimeState = getTimeState();
   if (oldTimeState == TIME_STATE_DELTA) {
      absoluteTime();
   }
   int i, j;
   int length = getNumTracks();
   for (i=0; i<length; i++) {
      for (j=0; j<events[i]->getSize(); j++) {
         joinedTrack->append((*events[i])[j]);
      }
   }

   erase(); 

   delete events[0];
   events[0] = joinedTrack;
   sortTracks();
   if (oldTimeState == TIME_STATE_DELTA) {
      deltaTime();
   }
}



//////////////////////////////
//
// MidiFile::mergeTracks -- combine the data from two
//   tracks into one.  Placing the data in the first
//   track location listed, and Moving the other tracks
//   in the file around to fill in the spot where Track2
//   used to be.  The results of this function call cannot
//   be reversed.
//

void MidiFile::mergeTracks(int aTrack1, int aTrack2) { 
   Collection <_MFEvent>* mergedTrack;
   mergedTrack = new Collection<_MFEvent>;
   mergedTrack->setSize(0);
   int oldTimeState = getTimeState();
   if (oldTimeState == TIME_STATE_DELTA) {
      absoluteTime();
   }
   int i, j;
   int length = getNumTracks();
   for (i=0; i<events[aTrack1]->getSize(); i++) {
      mergedTrack->append((*events[aTrack1])[i]);
   }
   for (j=0; j<events[aTrack2]->getSize(); i++) {
      (*events[aTrack2])[i].track = aTrack1;
      mergedTrack->append((*events[aTrack2])[i]);
   }

   sortTrack(*mergedTrack);

   delete events[aTrack1];

   events[aTrack1] = mergedTrack;

   for (i=aTrack2; i<length-1; i++) {
      events[i] = events[i+1];
   }

   events[length] = NULL;
   events.setSize(length-1);

   if (oldTimeState == TIME_STATE_DELTA) {
      deltaTime();
   }
}



//////////////////////////////
//
// MidiFile::read -- read a MIDI file and store its contents.
//

int MidiFile::read(char* aFile) { 

   #ifdef VISUAL
      FileIO inputfile(aFile, ios::in | ios::nocreate | ios::binary);
   #else
      FileIO inputfile(aFile, ios::in | ios::nocreate);
   #endif 


   if (!inputfile.is_open()) {
      return 0;
   }

   // Read the MIDI header (4 bytes of ID, 4 byte data size, 6 bytes
   // of data.

   ulong longdata;
   uchar chardata;
   ushort shortdata;
   
   inputfile.readBigEndian(chardata);
   if (chardata != 'M') {
      cout << "File: " << aFile << " is not a MIDI file" << endl;
      cout << "Chara data is" << chardata << endl;
      return 0;
   }

   inputfile.readBigEndian(chardata);
   if (chardata != 'T') {
      cout << "File: " << aFile << " is not a MIDI file" << endl;
      return 0;
   }

   inputfile.readBigEndian(chardata);
   if (chardata != 'h') {
      cout << "File: " << aFile << " is not a MIDI file" << endl;
      return 0;
   }

   inputfile.readBigEndian(chardata);
   if (chardata != 'd') {
      cout << "File: " << aFile << " is not a MIDI file" << endl;
      return 0;
   }

   // read header size
   inputfile.readBigEndian(longdata);
   if (longdata != 6) {
      cout << "File: " << aFile 
           << " is not a MIDI 1.0 Standard MIDI file." << endl;
      cout << "The header size is: " << longdata << endl;
      return 0;
   }

   // read file type
   int type;
   inputfile.readBigEndian(shortdata);
   switch (shortdata) {
      case 0:
         type = 0;
         break;
      case 1:
         type = 1;
         break;
      default:
         cout << "Error: cannot handle type " << shortdata 
              << " MIDI file" << endl;
         return 0;
   }
 
   // read number of tracks
   int tracks;
   inputfile.readBigEndian(shortdata);
   if (type == 0 && shortdata != 1) {
      cout << "Error: Type 0 MIDI file can only contain one track" << endl;
      return 0;
   } else {
      tracks = shortdata;
   }
   cout << "Track count is: " << tracks << endl;
   erase();
   if (events[0] != NULL) {
      delete events[0];
   }
   events.setSize(tracks);
   for (int z=0; z<tracks; z++) {
      events[z] = new Collection<_MFEvent>;
      events[z]->setAllocSize(10000);
      events[z]->setSize(0);
      events[z]->allowGrowth(1);
   }

   // read ticks per quarter note
   short signeddata;
   inputfile.readBigEndian(signeddata);
   if (signeddata <= 0) {
      cout << "Error: cannot handle SMTP tick values for quarter notes"
              " yet" << endl;   
      return 0;
   } 
   ticksPerQuarterNote = signeddata;

   //////////////////////////////////////////////////
   //
   // now read individual tracks:
   //

   uchar runningCommand = 0;
   _MFEvent event;
   int absticks;
   int barline;
   
   for (int i=0; i<tracks; i++) {
      cout << "\nReading Track: " << i + 1 << flush;
      // read track header...
     
      inputfile.readBigEndian(chardata);
      if (chardata != 'M') {
         cout << "File: " << aFile << " has bad track info" << endl;
         cout << "character 1 is: " << (int)chardata << endl;
         inputfile.readBigEndian(chardata);
         if (inputfile.eof()) {
            cout << "End of file reached" << endl;
         }
         return 0;
      }

      inputfile.readBigEndian(chardata);
      if (chardata != 'T') {
         cout << "File: " << aFile << " has bad track info" << endl;
         cout << "character 2 is: " << (int)chardata << endl;
         return 0;
      }

      inputfile.readBigEndian(chardata);
      if (chardata != 'r') {
         cout << "File: " << aFile << " has bad track info" << endl;
         cout << "character 3 is: " << (int)chardata << endl;
         return 0;
      }

      inputfile.readBigEndian(chardata);
      if (chardata != 'k') {
         cout << "File: " << aFile << " has bad track info" << endl;
         cout << "character 4 is: " << (int)chardata << endl;
         return 0;
      }

      // Now read track chunk size and throw it away because it is
      // not really necessary since the track MUST end with an
      // end of track meta event, and 50% of Midi files or so
      // do not correctly give the track size.
      inputfile.readBigEndian(longdata);

      // set the size of the track allocation so that it might
      // approximately fit the data.
      events[i]->setSize(longdata/2);
      events[i]->setSize(0);

      // process the track
      absticks = 0;
      barline = 1;
      while (!inputfile.eof()) {
         longdata = extractVlvTime(inputfile);
//cout << "ticks = " << longdata << endl;
         absticks += longdata;
         extractMidiData(inputfile, event.data, runningCommand);
//cout << "command = " << hex << (int)event.data[0] << dec << endl;
         if (event.data[0] == 0xff && (event.data[1] == 1 || 
             event.data[1] == 2 || event.data[1] == 3 || event.data[1] == 4)) {
           // mididata.append('\0');
           cout << '\t';
           for (int m=0; m<event.data[2]; m++) {
              cout << event.data[m+3];
           }
           cout.flush();
         } else if (event.data[0] == 0xff && event.data[1] == 0x2f) {
            break;
         }

         if (event.data[0] != 0xff && event.data[0] != 0xf0) {
            event.time = absticks;
            if ((event.data[0] & 0xf0) == 0x90) {
               if (event.data[1] < 12) {
                  event.data[0] = event.data[1];
                  switch (event.data[0]) {
                     case 2: event.data[2] = barline++;    // barline
                             break; 
                     case 0: break;                        // beat
                  }
               }
            }
            events[i]->append(event);
         }

      }

   }
   cout << endl;

   theTimeState = TIME_STATE_ABSOLUTE;
   return 1;
}


//////////////////////////////
//
// MidiFile::sortTrack -- 
//

void MidiFile::sortTrack(Collection<_MFEvent>& trackData) { 
   qsort(trackData.getBase(), trackData.getSize(), 
      sizeof(_MFEvent), eventcompare);
}



//////////////////////////////
//
// MidiFile::sortTracks -- sort all tracks in the MidiFile.
//

void MidiFile::sortTracks(void) { 
   for (int i=0; i<getTrackCount(); i++) {
      sortTrack(*events[i]);
   }
}



//////////////////////////////
//
// MidiFile::splitTracks -- take the joined tracks and split them
//   back into their separate track identities.
//

void MidiFile::splitTracks(void) { 
   if (getTrackState() == TRACK_STATE_SPLIT) {
      return;
   }

   int oldTimeState = getTimeState();
   if (oldTimeState == TIME_STATE_DELTA) {
      absoluteTime();
   }

   int maxTrack = 0;
   int i;
   int length = events[0]->getSize();
   for (i=0; i<length; i++) {
      if ((*events[0])[i].track > maxTrack) {
          maxTrack = (*events[0])[i].track;
      }
   }

   Collection<_MFEvent>* olddata = events[0];
   events[0] = NULL;
   events.setSize(maxTrack);
   for (i=0; i<maxTrack; i++) {
      events[i] = new Collection<_MFEvent>;
      events[i]->setSize(0);
      events[i]->allowGrowth();
   }

   int trackValue = 0;
   for (i=0; length; i++) {
      trackValue = (*olddata)[i].track;
      events[trackValue]->append((*olddata)[i]);
   }

   delete olddata;

   if (oldTimeState == TIME_STATE_DELTA) {
      deltaTime();
   }
}



//////////////////////////////
//
// MidiFile::timeState -- returns what type of time method is
//   being used: either TIME_STATE_ABSOLUTE or TIME_STATE_DELTA.
//

int MidiFile::getTimeState(void) { 
   return theTimeState;
}



//////////////////////////////
//
// MidiFile::getTrackState -- returns what type of track method
//     is being used: either TRACK_STATE_JOINED or TRACK_STATE_SPLIT.
//

int MidiFile::getTrackState(void) { 
   return theTrackState;
}



//////////////////////////////
//
// MidiFile::write -- write a standard MIDI file from data.
//

int MidiFile::write(char* aFile) { 
   int oldTimeState = getTimeState();
   if (oldTimeState == TIME_STATE_ABSOLUTE) {
      deltaTime();
   }

   #ifdef VISUAL
      FileIO outputfile(aFile, ios::out | ios::noreplace | ios::binary);
   #else
      FileIO outputfile(aFile, ios::out | ios::noreplace);
   #endif 

   if (!outputfile.is_open()) {
      cout << "Error: could not write: " << aFile << endl;
      exit(1);
   }

   // write the header of the Standard MIDI File
   
   char ch;
   // 1. The characters "MThd"
   ch = 'M';
   outputfile.writeBigEndian(ch);
   ch = 'T';
   outputfile.writeBigEndian(ch);
   ch = 'h';
   outputfile.writeBigEndian(ch);
   ch = 'd';
   outputfile.writeBigEndian(ch);

   // 2. write the size of the header (alwas a "6" stored in unsigned long
   ulong longdata = 6;
   outputfile.writeBigEndian(longdata);

   // 3. MIDI file format, type 0, 1, or 2
   ushort shortdata;
   if (getNumTracks() == 1) {
      shortdata = 0;
   } else {
      shortdata = 1;
   }
   outputfile.writeBigEndian(shortdata);
      
   // 4. write out the number of tracks.
   shortdata = getNumTracks();
   outputfile.writeBigEndian(shortdata);

   // 5. write out the number of ticks per quarternote. (avoiding SMTPE for now)
   shortdata = getTicksPerQuarterNote();
   outputfile.writeBigEndian(shortdata);


   // now write each track.
   Array<uchar> trackdata;
   uchar endoftrack[4] = {0, 0xff, 0x2f, 0x00};
   int i, j, k;
   for (i=0; i<getNumTracks(); i++) {
      trackdata.setSize(0);
      trackdata.allowGrowth();
      for (j=0; j<events[i]->getSize(); j++) {
         writeVLValue((*events[i])[j].time, trackdata);
         for (k=0; k<(*events[i])[j].data.getSize(); k++) {
            trackdata.append((*events[i])[j].data[k]);
         }
      }
      trackdata.append(endoftrack[0]);
      trackdata.append(endoftrack[1]);
      trackdata.append(endoftrack[2]);
      trackdata.append(endoftrack[3]);

      // now ready to write to MIDI file.
   
      // first write the track ID marker "MTrk":
      ch = 'M';
      outputfile.writeBigEndian(ch);
      ch = 'T';
      outputfile.writeBigEndian(ch);
      ch = 'r';
      outputfile.writeBigEndian(ch);
      ch = 'k';
      outputfile.writeBigEndian(ch);

      // A. write the size of the MIDI data to follow:
      longdata = trackdata.getSize();
      outputfile.writeBigEndian(longdata);

      // B. write the actual data
      outputfile.write(trackdata.getBase(), trackdata.getSize());
   }

   if (oldTimeState == TIME_STATE_ABSOLUTE) {
      absoluteTime();
   }
 
   outputfile.close();

   return 1;
}



///////////////////////////////////////////////////////////////////////////
//
// private functions
//
   

//////////////////////////////
//
// MidiF::extractMidiData --
//

void MidiFile::extractMidiData(FileIO& inputfile, Array<uchar>& array, 
   uchar& runningCommand) {

   uchar byte;
   array.setSize(0);
   array.allowGrowth();
   int runningQ;

   inputfile.readBigEndian(byte);

   if (byte < 0x80) {
      runningQ = 1;
      if (runningCommand == 0) {
         cout << "Error: running command with no previous command" << endl;
         exit(1);
      }
   } else {
      runningCommand = byte;
      runningQ = 0;
   }

   array.append(runningCommand);
   if (runningQ) {
      array.append(byte);
   }

   uchar metai;
   switch (runningCommand & 0xf0) {
      case 0x80:        // note off (2 more bytes)
      case 0x90:        // note on (2 more bytes)
      case 0xA0:        // aftertouch (2 more bytes)
      case 0xB0:        // cont. controller (2 more bytes)
      case 0xE0:        // pitch wheel (2 more bytes)
         inputfile.readBigEndian(byte);
         array.append(byte);
         if (!runningQ) {
            inputfile.readBigEndian(byte);
            array.append(byte);
         }
         break;
      case 0xC0:        // patch change (1 more byte)
      case 0xD0:        // channel pressure (1 more byte)
         if (!runningQ) {
            inputfile.readBigEndian(byte);
            array.append(byte);
         }
         break;
      case 0xF0:
         switch (runningCommand) {
            case 0xff:                 // meta event
               {
               if (!runningQ) {
                  inputfile.readBigEndian(byte);  // meta type
               array.append(byte);
               }
               inputfile.readBigEndian(metai);  // meta size
               array.append(metai);
               for (uchar j=0; j<metai; j++) {
                  inputfile.readBigEndian(byte);  // meta data
                  array.append(byte);
               }
               }
               break;
            case 0xf0:                // sysex
               // read until you find a 0xf7 character
               byte = 0;
               while (byte != 0xf7 && !inputfile.eof()) {
                  inputfile.readBigEndian(byte);  // meta data
               }
               break;
         }
         break; 
      default:
         cout << "Error reading midifile" << endl;
         cout << "Command byte was " << (int)runningCommand << endl;
         exit(1);
   }
}



//////////////////////////////
//
// MidiF::extractVlvTime -- 
//

ulong MidiFile::extractVlvTime(FileIO& inputfile) {
   uchar b[5] = {0};
 
   for (int i=0; i<5; i++) {
      inputfile.readBigEndian(b[i]);
      if (b[i] < 0x80) {
         break;
      }
   }
  
   return unpackVLV(b[0], b[1], b[2], b[3], b[4]);
}



//////////////////////////////
//
// MidiF::unpackVLV -- converts a VLV value to pure unsigned long value.
// default values: a = b = c = d = e = 0;
//

ulong MidiFile::unpackVLV(uchar a, uchar b, uchar c, uchar d, uchar e) {
   if (e > 0x7f) {
      cout << "Error: VLV value was too long" << endl;
      exit(1);
   }

   uchar bytes[5] = {a, b, c, d, e};
   int count = 0;
   while (bytes[count] > 0x7f && count < 5) {
      count++;
   }
   count++;

   ulong output = 0;
   for (int i=0; i<count; i++) {
      output = output << 7;
      output = output | (bytes[i] & 0x7f);
   }
   
   return output;
}
   

//////////////////////////////
//
// MidiFileWrite::writeVLValue -- write a number to the midifile
//    as a variable length value which segments a file into 7-bit
//    values.  Maximum size of aValue is 0x7fffffff
//

void MidiFile::writeVLValue(long aValue, Array<uchar>& outdata) {
   uchar bytes[5] = {0};
   bytes[0] = (uchar)(((ulong)aValue >> 28) & 0x7f);  // most significant 5 bits
   bytes[1] = (uchar)(((ulong)aValue >> 21) & 0x7f);  // next largest 7 bits
   bytes[2] = (uchar)(((ulong)aValue >> 14) & 0x7f);
   bytes[3] = (uchar)(((ulong)aValue >> 7)  & 0x7f);
   bytes[4] = (uchar)(((ulong)aValue)       & 0x7f);  // least significant 7 bits

   int start = 0;
   while (start<5 && bytes[start] == 0)  start++;

   for (int i=start; i<4; i++) {
      bytes[i] = bytes[i] | 0x80;    
      outdata.append(bytes[i]);
   }
   outdata.append(bytes[4]);
}

          
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// external functions
//


//////////////////////////////
//
// eventcompare -- for sorting the tracks
//

int eventcompare(const void* a, const void* b) {
   if (((_MFEvent*)a)->time > ((_MFEvent*)b)->time) {
      return 1;
   } else if (((_MFEvent*)a)->time < ((_MFEvent*)b)->time) {
      return -1;
   } else {
      return 0;
   }
}



//////////////////////////////
//
// operator<< -- for printing an ASCII version of the MIDI file
//

ostream& operator<<(ostream& out, MidiFile& aMidiFile) {
   int i, j, k;
   out << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
   out << "Number of Tracks: " << aMidiFile.getTrackCount() << "\n";
   out << "Time method: " << aMidiFile.getTimeState();
   if (aMidiFile.getTimeState() == TIME_STATE_DELTA) {
      out << " (Delta timing)";
   } else if (aMidiFile.getTimeState() == TIME_STATE_ABSOLUTE) {
      out << " (Absolute timing)";
   } else {
      out << " (unknown method)";
   } 
   out << "\n";
    
   out << "Divisions per Quarter Note: " << dec << aMidiFile.getTicksPerQuarterNote() << "\n";
   for (i=0; i<aMidiFile.getNumTracks(); i++) {
      out << "Track " << i 
          << "   +++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
      for (j=0; j<aMidiFile.getNumEvents(i); j++) {
         out << aMidiFile.getEvent(i, j).time << "\t"
             << "0x" << hex << (int)aMidiFile.getEvent(i, j).data[0] << " ";
         for (k=1; k<aMidiFile.getEvent(i, j).data.getSize(); k++) {
            out << dec << (int)aMidiFile.getEvent(i, j).data[k] << " ";
         }
         out << "\n";
      }
   }
   out << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
   return out;
}


// md5sum:        d8167ca3b12e09f4da95decb39ebb07d  MidiFile.cpp
