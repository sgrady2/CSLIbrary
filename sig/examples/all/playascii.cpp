//
// Programmer:    Leland Stanford, Jr. <leland@stanford.edu>
// Creation Date: 29 February 1999
// Last Modified: 29 February 1999
// Filename:      ...sig/doc/examples/improv/synthImprov/synthTemplate.cpp
// Syntax:        C++; synthImprov 2.0
//  
// Description: This is a template which you fill with algorithms 
//              in the following functions to create a synthImprov program.
//

#include "synthImprov.h"      // includes the default Win95 console interface
                              // for the synthImprov environment
#include <string.h>
#include <iostream.h>

/*----------------- beginning of improvization algorithms ---------------*/

// global variables:
fstream input;
int nextnotetime = 0;
char buffer[10000] = {0};
Array<uchar> data;

// functions:
int playline(void); 

/*--------------------- maintenance algorithms --------------------------*/

//////////////////////////////
//
// description -- this function is called by the improv interface
//     whenever a capital "D" is pressed on the computer keyboard.
//     Put a description of the program and how to use it here.
//

void description(void) {
   cout << "Plays an ASCII MIDI file" << endl;
} 



//////////////////////////////
//
// initialization -- this function is called by the improv
//     interface once at the start of the program.  Put items
//     here which need to be initialized at the beginning of
//     the program.
//

void initialization(void) { 
   nextnotetime = t_time;
   data.setSize(0);
   input.open("file", ios::in | ios::nocreate);
   if (!input.is_open()) {
      cout << "Error: cannot open file" << endl;
      exit(1); 
   }
   input.getline(buffer, 1024, '\n');
   while (!input.eof() && !isdigit(buffer[0])) {
      input.getline(buffer, 1024, '\n');
   }
}



//////////////////////////////
//
// finishup -- this function is called by the improv interface
//     whenever the program is exited.  Put items here which
//     need to be taken care of when the program is finished.
//

void finishup(void) { }


/*-------------------- main loop algorithms -----------------------------*/

//////////////////////////////
//
// mainloopalgorithms -- this function is called by the improv interface
//   continuously while the program is running.  The global variable t_time
//   which stores the current time is set just before this function is
//   called and remains constant while in this functions.
//

void mainloopalgorithms(void) { 
   while (nextnotetime <= t_time) {
      nextnotetime += playline(); 
   }   
}


/*-------------------- triggered algorithms -----------------------------*/

///////////////////////////////
//
// keyboardchar -- this function is called by the improv interface
//     whenever a key is pressed on the computer keyboard.
//     Put commands here which will be executed when a key is
//     pressed on the computer keyboard.
//

void keyboardchar(int key) { }


/*------------------ end improvization algorithms -----------------------*/


//////////////////////////////
//
// playline -- play the next midi message in the ascii midi file.
//

int playline(void) {
   data.setSize(0);
   int datum;
   uchar uchardatum;

cout << buffer << endl;

   char* dataz = strtok(buffer, "\t ");
   dataz = strtok(NULL, "\t ");
   while ((dataz != NULL && dataz[0] != ';') && (isdigit(dataz[0]))) {
      sscanf(dataz, "%d", &datum);
      uchardatum = (uchar)datum;
      data.append(uchardatum); 
      dataz = strtok(NULL, "\t ");
   }
   
   synth.rawsend(data.getBase(), data.getSize());

   input.getline(buffer, 1024, '\n');
   while (!input.eof() && !isdigit(buffer[0])) {
      input.getline(buffer, 1024, '\n');
   }

   if (input.eof()) {
      cout << "End of file" << endl;
      exit(0);
   }

   int deltatime;
   sscanf(buffer, "%d", &deltatime);

   return deltatime;
}


