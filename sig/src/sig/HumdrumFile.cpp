//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon May 18 13:43:47 PDT 1998
// Last Modified: Wed Jun 10 22:49:12 PDT 1998
// Last Modified: Thu Jul  1 16:19:35 PDT 1999
// Last Modified: Thu Apr 13 18:43:34 PDT 2000 (added generalized ex interps)
// Last Modified: Sat May  6 14:52:19 PDT 2000 (added appendCompositeDuration)
// Filename:      ...humdrum++/src/HumdrumFile.cpp
// Syntax:        C++ 
// $Smake:        cc -Wall -I../include -g -c %f && rm %b.o
//

#include "HumdrumFile.h"
#include "Convert.h"
#include <fstream.h>
#include <iostream.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//////////////////////////////
//
// HumdrumFile::HumdrumFile
//

HumdrumFile::HumdrumFile(void) {
   records.setSize(1000);          // initial storage size 1000 lines
   records.setSize(0);
   records.allowGrowth();          
   records.setAllocSize(1000);     // grow in increments of 1000 lines
}


HumdrumFile::HumdrumFile(const HumdrumFile& aHumdrumFile) {
   records.setSize(1000);          // initial storage size 1000 lines
   records.setSize(0);
   records.allowGrowth();          
   records.setAllocSize(1000);     // grow in increments of 1000 lines

   *this = aHumdrumFile;
}


HumdrumFile::HumdrumFile(const char* filename) {
   records.setSize(1000);          // initial storage size 1000 lines
   records.setSize(0);
   records.allowGrowth();          
   records.setAllocSize(1000);     // grow in increments of 1000 lines

   ifstream infile(filename, ios::in | ios::nocreate);
   if (!infile) {
      cerr << "Error: cannot open file: " << filename << endl;
      exit(1);
   }

   char templine[4096];
   while (!infile.eof()) {
      infile.getline(templine, 4096);
      appendLine(templine);
   }
   assignExInt();
}



//////////////////////////////
//
// HumdrumFile::~HumdrumFile
//

HumdrumFile::~HumdrumFile() {
   clear();
}



//////////////////////////////
//
// HumdrumFile::appendCompositeDuration  -- add an extra
//   spine at the right-hand side which give the duration
//   of the current line in terms of 1.0 = a quarter note, 
//   2.0 = a half note, etc.
//

void HumdrumFile::appendCompositeDuration(void) {
   processRecordsForDuration(*this);
}





//////////////////////////////
//
// HumdrumFile::appendLine  -- adds a line to a humdrum file
//

void HumdrumFile::appendLine(const char* aLine) {
   HumdrumRecord* aRecord;
   aRecord = new HumdrumRecord;
   aRecord->setLine(aLine);
   records[records.getSize()] = aRecord;
}


void HumdrumFile::appendLine(HumdrumRecord& aRecord) {
   HumdrumRecord *tempRecord;
   tempRecord = new HumdrumRecord;
   *tempRecord = aRecord;
   records[records.getSize()] = tempRecord;
}
   

void HumdrumFile::appendLine(HumdrumRecord* aRecord) {
   appendLine(*aRecord);
}



//////////////////////////////
//
// Humdrum::assignExInt -- traces through spines and
//	marks the exclusive interpretations for each data field.
//	run this function when you want the exint markers
//	for the line fields to be accurate.
//
// Note: currently does not allow splitting or joining of
// 	spines.  Will be done later
//

void HumdrumFile::assignExInt(void) {
   static int exintmaker = 30000;
   Collection<int> interps;
   int i, k;
   int location = -1;
   int endlocation;
   int lastSize = 0;
   int setupQ = 0;
   for (i=0; i<records.getSize(); i++) {
      if (!setupQ && records[i]->getType() == E_humrec_data_interpretation) {
         interps.setSize(records[i]->getFieldCount());
         for (k=0; k<records[i]->getFieldCount(); k++) {
            interps[k] = Convert::exint.getValue((*records[i])[k]);
            if (interps[k] == E_unknown) {
               Convert::exint.add(exintmaker++, (*records[i])[k]);
            }
            interps[k] = Convert::exint.getValue((*records[i])[k]);
         }
         setupQ = 1;
         lastSize = interps.getSize();
      }

      // handle assignment of excluisve interpretations
      if (setupQ && records[i]->getType() >= E_humrec_data) {
         // handle splitting or joining of spines
         if ((*records[i])[0][0] == '*') {
            for (int p=0; p<records[i]->getFieldCount(); p++) {
               // spine expansion: *^
               if (strcmp((*records[i])[p], "*^") == 0) {
                  location = -1;
                  for (int m=0; m<records[i]->getFieldCount(); m++) {
                     if (strcmp((*records[i])[m], "*^") == 0) {
                        location = m;
                        interps.setSize(interps.getSize() + 1);
                        for (int n=interps.getSize() - 1; n>location; n--) {
                           interps[n] = interps[n-1];
                        }
                        break;
                     }
                  }
                  break;
               // spine contractions: *v
               } else if (strcmp((*records[i])[p], "*v") == 0) {
                  location = -1;
                  for (int m=0; m<records[i]->getFieldCount(); m++) {
                     if (strcmp((*records[i])[m], "*v") == 0) {
                        location = m;
                        endlocation = m;
                        while (endlocation < records[i]->getFieldCount() &&
                              strcmp((*records[i])[endlocation],"*v") == 0) {
                           endlocation++;
                        }
                        endlocation--;
                        for (int n=location; 
                              n<interps.getSize()-1-endlocation+location;
                              n++) {
                           interps[n] = interps[n+endlocation-location];
                        }
                        interps.setSize(interps.getSize() - 
                              1 - location + endlocation);
                        break;
                     }
                  }
                  break;
               }

            }
         }
 
         for (k=0; k<records[i]->getFieldCount(); k++) {
            records[i]->setExInt(k, interps[k]);
         }
      }
   }
}      
      


//////////////////////////////
//
// HumdrumFile::clear -- removes all lines from the humdrum file
//

void HumdrumFile::clear(void) {
   for (int i=0; i<getNumLines(); i++) {
      delete records[i];
      records[i] = NULL;
   }
   records.setSize(0);
}



//////////////////////////////
//
// HumdrumFile::extract
//

HumdrumFile HumdrumFile::extract(int aField) {
   int fieldNumber = aField;
   HumdrumRecord aRecord;
   HumdrumFile output;
   for (int i=0; i<getNumLines(); i++) {
      switch(records[i]->getType()) {
         case E_humrec_data:
         case E_humrec_data_comment:
         case E_humrec_data_kern_measure:
         case E_humrec_data_tandem:
         case E_humrec_data_interpretation:
            if (fieldNumber < 0) {
               fieldNumber = records[i]->getFieldCount() - 1;
            }
            aRecord.setLine((*records[i])[fieldNumber]);
            aRecord.setExInt(0, records[i]->getExInt(fieldNumber));
            output.appendLine(aRecord);
            break;
         case E_humrec_empty:
            output.appendLine(records[i]);
            break;
         case E_humrec_none:
         case E_humrec_comment:
         case E_humrec_global_comment:
         default:
            output.appendLine(records[i]);
      }
   }

   return output;
}



//////////////////////////////
//
// HumdrumFile::getLine -- returns the character string of the
//	specified line
//

const char* HumdrumFile::getLine(int index) {
   if (index >= getNumLines()) {
      cerr << "Error: maximum line index is: " << getNumLines() 
           << ", but you asked for line index: " << index << endl;
      exit(1);
   }
  
   return records[index]->getLine();
}



//////////////////////////////
//
// HumdrumFile::getNumLines  -- returns the number of lines
//	in the HumdrumFile
//

int HumdrumFile::getNumLines(void) {
   return records.getSize();
}



//////////////////////////////
//
// HumdrumFile::getRecord  -- returns the record of the specified line
//

HumdrumRecord& HumdrumFile::getRecord(int index) {
   if (index >= getNumLines()) {
      cerr << "Error: maximum line index is: " << getNumLines() 
           << ", but you asked for line index: " << index << endl;
      exit(1);
   }

   return *records[index];
}



//////////////////////////////
//
// HumdrumFile::getTiedDuration -- returns the total duration of
//   a tied note if the first note is the beginning of a tie.
//   Returns the duration of the note if not a tied note, or
//   zero if the specified field is not a note.
//     default value: token = 0;
//

double HumdrumFile::getTiedDuration(int linenum, int field, int token) {
   HumdrumFile& file = *this;
   int length = file.getNumLines();
   char buffer[128] = {0};
   double duration = 0.0;   // total duration of tied notes.
   int done = 0;            // true when end of tied note is found
   int startpitch = 0;      // starting pitch of the tie
   int matchpitch = 0;      // current matching pitch of the tie
   
   file[linenum].getToken(buffer, field, token);
   if (strchr(buffer, '[')) {
      duration = Convert::kernToDuration(buffer);
      // allow for enharmonic ties:
      startpitch = Convert::kernToMidiNoteNumber(buffer);
   } else {
      return Convert::kernToDuration(buffer);
   }

   int currentLine = linenum + 1;
   while (!done && currentLine < length) {
      if (file[currentLine].getType() != E_humrec_data) {
         currentLine++;
         continue;
      }

      if (strchr(file[currentLine][field], '_')) {
         matchpitch = Convert::kernToMidiNoteNumber(file[currentLine][field]);
         if (startpitch == matchpitch) {
            duration += Convert::kernToDuration(file[currentLine][field]);
         } else {
            done = 1;
         }
      } else if (strchr(file[currentLine][field], ']')) {
         matchpitch = Convert::kernToMidiNoteNumber(file[currentLine][field]);
         if (startpitch == matchpitch) {
            duration += Convert::kernToDuration(file[currentLine][field]);
            done = 1;
         } else {
            done = 1;
         }
      }
      currentLine++;
   }

   return duration;
}




//////////////////////////////
//
// HumdrumFile::operator=
//

HumdrumFile& HumdrumFile::operator=(const HumdrumFile& aFile) {
   // don't copy onto self
   if (&aFile == this) {
      return *this;
   }

   // delete any current contents
   int i;
   for (i=0; i<records.getSize(); i++) {
      delete records[i];
      records[i] = NULL;
   }

   records.setSize(aFile.records.getSize());
   for (i=0; i<aFile.records.getSize(); i++) {
      records[i] = new HumdrumRecord;
      *(records[i]) = *(aFile.records[i]);
   }

   return *this;
}



//////////////////////////////
//
// HumdrumFile::read -- read in a humdrum file.
//

void HumdrumFile::read(const char* filename) {
   int i;
   for (i=0; i<records.getSize(); i++) {
      delete records[i];
      records[i] = NULL;
   }
   records.setSize(0);

   ifstream infile(filename, ios::in | ios::nocreate);
   if (!infile) {
      cerr << "Error: could not open file: " << filename << endl;
      exit(1);
   }

   char templine[4096];
   while (!infile.eof()) {
      infile.getline(templine, 4096, '\n');
      appendLine(templine);
   }
   assignExInt();
}


void HumdrumFile::read(istream& inStream) {
   char* templine;
   templine = new char[4096];
   while (!inStream.eof()) {
      inStream.getline(templine, 4096);
      appendLine(templine);
   }
   assignExInt();
   delete [] templine;
}



//////////////////////////////
//
// HumdrumFile::removeNullRecords
//

HumdrumFile HumdrumFile::removeNullRecords(void) {
   HumdrumFile output;
   int j;
   int nulltest = 1;
   for (int i=0; i<getNumLines(); i++) {
      nulltest = 1;
      switch(records[i]->getType()) {
         case E_humrec_data:
            for (j=0; j<records[i]->getFieldCount(); j++) {
               if (strcmp((*records[i])[j], ".") != 0) {
                  nulltest = 0;
                  break;
               }
            }
            if (nulltest) {
               // do nothing
            } else {
               output.appendLine(records[i]);
            }
            break;
         default:
            output.appendLine(records[i]);
      }
   }

   return output;
}





















//////////////////////////////
//
// HumdrumFile::operator[]
//

HumdrumRecord& HumdrumFile::operator[](int index) {
   if (index >= getNumLines()) {
      cerr << "Error: maximum line index is: " << getNumLines() 
           << ", but you asked for line index: " << index << endl;
      exit(1);
   }

   return *records[index];
}



//////////////////////////////
//
// HumdrumFile::write -- write out a humdrum file.
//

void HumdrumFile::write(const char* filename) {
   ofstream outfile(filename, ios::out | ios::noreplace);
   if (!outfile) {
      cerr << "Error: could not open file for writing: " << filename << endl;
      cerr << "Perhaps it already exists?" << endl;
      exit(1);
   }

   for (int i=0; i<records.getSize(); i++) {
      outfile << records[i]->getLine() << endl;
   }
}


void HumdrumFile::write(ostream& outStream) {
   for (int i=0; i<records.getSize(); i++) {
      outStream << records[i]->getLine() << endl;
   }
}



///////////////////////////////////////////////////////////////////////////



//////////////////////////////
//
// operator<<
//

ostream& operator<<(ostream& out, HumdrumFile& aHumdrumFile) {
   for (int i=0; i<aHumdrumFile.getNumLines(); i++) {
      out << aHumdrumFile[i].getLine() << '\n';
   }
   return out;
}




///////////////////////////////////////////////////////////////////////////
// 
// Private functions for use with appendCompositeDuration().
// 


//////////////////////////////
//
// HumdrumFile::processRecordsForDuration -- go through the file
//   and append the duration information.
//

void HumdrumFile::processRecordsForDuration(HumdrumFile& infile) {
   char aString[256] = {0};
   double duration;
   int durint = 1;
   int lastDataRecord = 0;

   for (int i=0; i<infile.getNumLines(); i++) {
      switch (infile[i].getType()) {
         case E_humrec_none:
         case E_humrec_empty:
         case E_humrec_comment:
         case E_humrec_global_comment:
            break;
         case E_humrec_data_comment:
            infile[i].appendField("!");
            break;
         case E_humrec_data_tandem:
            if (infile[i].equalFieldsQ("**kern")) {
               infile[i].appendField(infile[i][0]);
            } else {
               infile[i].appendField("*");
            }
            break;
         case E_humrec_data_kern_measure:
            if (infile[i].equalFieldsQ("**kern")) {
               infile[i].appendField(infile[i][0]);
            } else {
               infile[i].appendField("=");
            }
            break;
         case E_humrec_data_interpretation:
            infile[i].appendField("**duration");
            determineDuration(infile[i], durint);
            break;
         case E_humrec_data:
            // handle null fields
            if (infile[i].equalFieldsQ("**kern", ".")) {
               infile[i].appendField(".");
               break;
            }

            duration = determineDuration(infile[i], durint);
            sprintf(aString, "%f", duration);
            infile[i].appendField(aString);
          
            lastDataRecord = i;
            break;
         default:
            cerr << "Error on line " << (i+1) << " of input" << endl;
            exit(1);
      }
   }
}



//////////////////////////////
//
// HumdrumFile::determineDuration -- get the duration of the current
//   HumdrumFile record.
//

double HumdrumFile::determineDuration(const HumdrumRecord& aRecord, 
      int& durinit) {
   static Collection<double> lastdurations;  // for keeping track of durations
   static Collection<double> runningstatus;  // for keeping track current dur

   int i;
   // initialization:
   if (durinit) {
      durinit = 0;
      lastdurations.setSize(aRecord.getFieldCount());
      runningstatus.setSize(aRecord.getFieldCount());
      for (i=0; i<aRecord.getFieldCount(); i++) {
         lastdurations[i] = 0;
         runningstatus[i] = 0;
      }
      // remove non-kern spines from note list
      for (i=0; i<aRecord.getFieldCount(); i++) {
         if (aRecord.getExInt(i) != E_KERN_EXINT) {
            lastdurations.setSize(lastdurations.getSize() - 1);
            runningstatus.setSize(runningstatus.getSize() - 1);
         }
      }

      return 0.0;
   }


   // Step (1): if lastdurations == runningstatus, then zero running status.
   for (i=0; i<runningstatus.getSize(); i++) {
      if (runningstatus[i] == lastdurations[i]) {
         runningstatus[i] = 0;
      }
   }

 
   // Step (2): input new durations into the lastdurations array
   int count = 0;
   for (i=0; i<aRecord.getFieldCount(); i++) {
      if (aRecord.getExInt(i) == E_KERN_EXINT) {
         if (strcmp(aRecord[i], ".") == 0) {
            ; // don't do anything
         } else {
            lastdurations[i] = Convert::kernToDuration(aRecord[i]);
         }
         count++;
      }
   }
   if (count != runningstatus.getSize()) {
      cerr << "Error: spine count has changed" << endl;
   }


   // Step (3): find minimum duration by subtracting last from running
   double min = 99999999;
   double test;
   for (i=0; i<lastdurations.getSize(); i++) {
      test = lastdurations[i] - runningstatus[i];
      if (test < min) {
         min = test;
      }
   }
   

   // Step (4): add the duration to the running values and to meter position
   for (i=0; i<runningstatus.getSize(); i++) {
      runningstatus[i] += min;
   }

   return min;
}


// md5sum:	44f7949908369218246595c8699d2ef8  HumdrumFile.cpp
