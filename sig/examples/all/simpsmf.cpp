//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Oct 24 10:39:47 PDT 1999
// Last Modified: Sun Oct 24 12:24:26 PDT 1999
// Filename:      /disk/linux2/tmp/midimaker/simpsmf.cpp
// Syntax:        C++ 
// $Smake:        g++ -O3 --static -Iinclude/sig/ -o %b %f %
//                -L lib/ -lsig && strip %b
//

#include "FileIO.h"
#include "Array.h"
#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

typedef unsigned char uchar;

// function declarations:
void finishOutput(FileIO& output, Array<uchar>& array);
int  processDataLine(const char* buffer, Array<uchar>& array);
int  processInput(fstream& input, Array<uchar>& array);
void startOutput(FileIO& output);
int  writeVLV(int duration, Array<uchar>& array);


///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   if (argc != 2) {
      cerr << "Usage: " << argv[0] << " humdrum-midi-file " << endl;
      exit(1);
   }

   fstream input(argv[1], ios::in | ios::nocreate);

   char outputname[1000] = {0};
   strncpy(outputname, argv[1], 1000);
   char* extensionStart = strchr(outputname, '.');
   if (extensionStart == NULL) {
      strcat(outputname, ".mid");
   } else {
      strcpy(extensionStart, ".mid");
   }

   FileIO output(outputname, ios::out);
   Array<uchar> miditrack;
   miditrack.setSize(10000);
   miditrack.setSize(0);
  
   if (!input.is_open()) {
      cerr << "Error: cannot open file: " << argv[1] << endl;
      exit(1);
   } 

   startOutput(output);
   int status = processInput(input, miditrack);
   finishOutput(output, miditrack);

   return status;
}


///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// finishOutput -- write the track size and its data to the output MIDI file.
//

void finishOutput(FileIO& output, Array<uchar>& array) {
   uchar arraydatum;

   // add the end-of-track to the track data:
   arraydatum = 0x00;
   array.append(arraydatum);
   arraydatum = 0xff;
   array.append(arraydatum);
   arraydatum = 0x2f;
   array.append(arraydatum);
   arraydatum = 0x00;
   array.append(arraydatum);

   // now write the size of the track data to the output file:
   unsigned long parameter = array.getSize();
   output.writeBigEndian(parameter);

   // and finally, write the track data itself:
   output.write(array.getBase(), array.getSize());
}



//////////////////////////////
//
// processDataLine -- convert the dataline into MIDI data.
//

int processDataLine(const char* buffer, Array<uchar>& array) {
   static char tokenbuffer[1024] = {0};
   strncpy(tokenbuffer, buffer, 1000);
   char* token = strtok(tokenbuffer, ". \t");
   int count = 0;
   int duration = 0;
   int midikey = 0;
   int midivel = 0;
   int status = 0;
   uchar arraydatum = 0;

   while (token != NULL) {
      if (strchr(token, '/') == NULL) {
         // this token has ill-formed MIDI data.
         token = strtok(NULL, ". \t");
         continue;
      }
      count++;
  
      status = sscanf(token, "%d/%d/%d", &duration, &midikey, &midivel);
      if (status != 3) {
         array.setSize(0);
         cerr << "Did not read a MIDI data entry properly" << endl;
         return 1;
      }
      
      // write the midi delta time to the MIDI track data:
      if (count == 1) {
         writeVLV(duration, array);
      } else {
         arraydatum = 0;
         array.append(arraydatum);
      }

      // write the MIDI command byte to the track data:
      if (midikey > 0) {
         arraydatum = 0x90;
         array.append(arraydatum);
         arraydatum = midikey;
         array.append(arraydatum);
         arraydatum = midivel;
         array.append(arraydatum);
      } else {
         arraydatum = 0x80;
         array.append(arraydatum);
         arraydatum = -midikey;
         array.append(arraydatum);
         arraydatum = midivel;
         array.append(arraydatum);
      }

      // get the next token ready for testing at the top of the loop
      token = strtok(NULL, ". \t");
   }

   return 0;
}



//////////////////////////////
//
// processInput -- read the input file and write to the midi output
//    file as required.
//

int processInput(fstream& input, Array<uchar>& array) {
   char buffer[1024] = {0};
   int exclusiveCount = 0;
   int tempo = 90;
   int status = 0;

   input.getline(buffer, 1000, '\n');
   while (!input.eof()) {
      if (buffer[0] == '\0' || buffer[0] == '!' || buffer[0] == '=') {
         input.getline(buffer, 1000, '\n');
         continue;
      }
  
      if (buffer[0] == '*') {
         if (buffer[1] == '*') {
            exclusiveCount++;
            if (exclusiveCount > 1) {
               array.setSize(0);
               cerr << "Error: too many exclusive interpretation lines" << endl;
               return 1;
            }
         } else if (buffer[1] == 'M' && buffer[2] == 'M' && 
               isdigit(buffer[3])) {
            sscanf(buffer, "*MM%d", &tempo);
         } else {
            input.getline(buffer, 1000, '\n');
            continue;
         }
      }

      // we must have data if this point is reached
      status = processDataLine(buffer, array);
      if (status != 0) {
         array.setSize(0);
         return 1;
      }

      input.getline(buffer, 1000, '\n');
   }

   return 0;
}



//////////////////////////////
//
// startOutput -- write the header for the output MIDI file
//

void startOutput(FileIO& output) {
   // write the basic MIDI header into output file.
   output << "MThd";
   unsigned long parameter;

   // write size of header chunk
   parameter = 6;
   output.writeBigEndian(parameter);
    
   // write MIDI format (type 0);
   unsigned short lparam;
   lparam = 0;
   output.writeBigEndian(lparam);

   // write the number of tracks in the file
   lparam = 1;
   output.writeBigEndian(lparam);
   
   // write the ticks per quarter-note
   lparam = 72;
   output.writeBigEndian(lparam);

   // write track chunk header
   output << "MTrk";

   // the next thing to write is the size of the data in bytes
   // and then the data.  This will be done by the finishOutput
   // function.
};



//////////////////////////////
//
// writeVLV -- write the header for the output MIDI file
//

int writeVLV(int number, Array<uchar>& array) {
   unsigned long value = (unsigned long)number;
   uchar arraydatum = 0;

   if (value >= (1 << 28)) {
      array.setSize(0);
      cerr << "Error: number too large to handle" << endl; 
      return 1;
   }
  
   unsigned long byte[4];
   byte[0] = (value >> 21) & 0x7f;
   byte[1] = (value >> 14) & 0x7f;
   byte[2] = (value >>  7) & 0x7f;
   byte[3] = (value >>  0) & 0x7f;

   int i;
   int flag = 0;
   for (i=0; i<3; i++) {
      if (byte[i] != 0) {
         flag = 1;
      }
      if (flag) {
         byte[i] |= 0x80;
      }
   }

   for (i=0; i<4; i++) {
      if (byte[i] >= 0x80 || i == 3) {
         arraydatum = (uchar)byte[i];
         array.append(arraydatum);
      }
   }

   return 0;
}




