//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Jun 12 21:45:10 GMT-0800 1997
// Last Modified: Thu Jun 12 21:45:13 GMT-0800 1997
// Filename:      ...sig/doc/examples/sig/sigfile/ab-test/ab-test.cpp
// Syntax:        C++; sig
//
// Description:   Plays sound 1 as the reference, and then you choose between 
//                two randomized selections using the two input soundfiles.
//
// Note: currently only works with NeXT computers.
//


#include <stdlib.h>
#include <iostream.h>
#include <time.h>
#include <string.h>

void exitUsage(char* commandName);

int main(int argc, char* argv[]) {
   if (argc != 3) exitUsage(argv[0]); 

   srand(time(NULL));   
   int reference = 1;
   int select1 = rand() % 2 + 1;         // 1 or 2
   int select2 = 3 - select1;            // 2 or 1

   char commandR[1000] = {};
   char command1[1000] = {};
   char command2[1000] = {};

   strcat(commandR, "sndplay "); strcat(commandR, argv[reference]);
   strcat(command1, "sndplay "); strcat(command1, argv[select1]);
   strcat(command2, "sndplay "); strcat(command2, argv[select2]);

   int choice = 0;
   cout << "You must choose which of samples 1 and 2 are identical"
        << " to the reference." << endl;
   while (choice < 3 && choice >= 0) {
      cout << "Which sound to play:" << endl;
      cout << "   0 = reference, 1 = selection 1, "
           << "2 = selection 2, 3 = make choice: "
           << endl;
      cin  >> choice; 
      switch (choice) {
         case 0:
            system(commandR);
            break;
         case 1:
            system(command1);
            break;
         case 2:
            system(command2);
            break;
      }
   }
   if (choice == 3) {
      cout << "Which sample is identical to the reference (1 or 2): ";
      cin  >> choice;
      if (choice == 1) {
         if (reference == select1) {
            cout << "You are correct!" << endl;
         } else {
            cout << "You are not correct." << endl;
         }
      } else if (choice == 2) {
         if (reference == select2) {
            cout << "You are correct!" << endl;
         } else {
            cout << "You are not correct." << endl;
         }
      }
   }

   return 0;
}



void exitUsage(char* commandName) {
   cout << endl;
   cout
   << "Program for testing perception of differences between two soundfiles.\n"
   << "The first sound file is the reference sound, and then sample 1 and\n"
   << "sample 2 are both the reference sound and the second soundfile in a\n"
   << "random order. Your goal is to match the reference soundfile with one\n"
   << "of the two selections.\n";
   cout << endl;
   cout << "Usage: " << commandName << " reference alternate" << endl;
   cout << endl;

   exit(1);
}



