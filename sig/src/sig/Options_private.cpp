//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr  5 13:07:18 PDT 1998
// Last Modified: Sun Jan 10 05:43:24 PST 1999
// Filename:      ...sig/code/base/Options_private.cpp
// Web Address:   http://www-ccrma.stanford.edu/~craig/improv/include/Options_private.cpp
// Syntax:        C++ 
//
// Description:   A private function for use in the Options class.
//

#include "Options_private.h"
#include <iostream.h>
#include <stdlib.h>
#include <string.h>


//////////////////////////////
//
// option_register::option_register
//

option_register::option_register(void) {
   definition = NULL;
   defaultOption = NULL;
   modifiedOption = NULL;
   type = 's';
}


option_register::option_register(const char* aDefinition, char aType,
      const char* aDefaultOption, const char* aModifiedOption) {
   definition = NULL;
   defaultOption = NULL;
   modifiedOption = NULL;

   setType(aType);
   setDefinition(aDefinition);
   setDefault(aDefaultOption);
   setModified(aModifiedOption);
}



//////////////////////////////
//
// option_register::~option_register
//

option_register::~option_register() {
   if (definition != NULL) {
      delete [] definition;
   } 
   definition = NULL;

   if (defaultOption != NULL) {
      delete [] defaultOption;
   }
   defaultOption = NULL;
  
   if (modifiedOption != NULL) {
      delete [] modifiedOption;
   }
   modifiedOption = NULL;
}



//////////////////////////////
//
// option_register::clearModified -- sets the modified string to
//	NULL.
//

void option_register::clearModified(void) {
   if (modifiedOption != NULL) {
      delete [] modifiedOption;
   } 
   modifiedOption = NULL;
}   



//////////////////////////////
//
// option_register::getDefinition -- returns the initial definition
//	string used to define this entry.
//

const char* option_register::getDefinition(void) {
   return definition;
}



//////////////////////////////
//
// option_register::getDefault --  returns the default string
//	to be returned.  Will never return a NULL;
//

const char* option_register::getDefault(void) {
   return defaultOption;
}



//////////////////////////////
//
// option_register::getModified -- return the modified
//	option string
//

const char* option_register::getModified(void) {
   return modifiedOption;
}



//////////////////////////////
//
// option_register::getModifiedQ -- returns true if 
//	modified string is not null, false otherwise.
//

int option_register::getModifiedQ(void) {
   if (modifiedOption == NULL) {
      return 0;
   } else { 
      return 1;
   }
}



//////////////////////////////
//
// option_register::getType -- 
//

char option_register::getType(void) {
   return type;
}



//////////////////////////////
//
// option_register::getOption -- return the modified option
//  	or the default option if no modified option.
//

const char* option_register::getOption(void) {
   if (getModifiedQ()) {
      return getModified();
   } else {
      return getDefault();
   }
}



//////////////////////////////
//
// option_register::reset -- deallocate space for all
//	strings in object.  (but default string is set to "")
//

void option_register::reset(void) {
   if (definition != NULL) {
      delete [] definition;
   } 
   definition = NULL;

   if (defaultOption != NULL) {
      delete [] defaultOption;
   }
   defaultOption = NULL;
   defaultOption = new char[1];
   defaultOption[0] = '\0';
  
   if (modifiedOption != NULL) {
      delete [] modifiedOption;
   }
   modifiedOption = NULL;
}



//////////////////////////////
//
// option_register::setDefault --
//

void option_register::setDefault(const char* aString) {
   if (aString == NULL) {
      cout << "Error: default string cannot be null" << endl;
      exit(1);
   }

   if (defaultOption != NULL) {
      delete [] defaultOption;
   }
   if (aString == NULL) {
      defaultOption = NULL;
   } else {
      defaultOption = new char[strlen(aString) + 1];
      strcpy(defaultOption, aString);
   }
}



//////////////////////////////
//
// option_register::setDefinition --
//

void option_register::setDefinition(const char* aString) {

   if (definition != NULL) {
      delete [] definition;
   }
   if (aString == NULL) {
      definition = NULL;
   } else {
      definition = new char[strlen(aString) + 1];
      strcpy(definition, aString);
   }

}



//////////////////////////////
//
// option_register::setModified --
//

void option_register::setModified(const char* aString) {
   if (modifiedOption != NULL) {
      delete [] modifiedOption;
   }
   if (aString == NULL) {
      modifiedOption = NULL;
   } else {
      modifiedOption = new char[strlen(aString) + 1];
      strcpy(modifiedOption, aString);
   }
}



//////////////////////////////
//
// option_register::setType --
//

void option_register::setType(char aType) {
   type = aType;
}



/////////////////////////////////////////////////////////////////////////////
/// option_list class definitions ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// option_list::option_list --
//

option_list::option_list(void) {
   name = NULL;
   index = -1;
}



//////////////////////////////
//
// option_list::option_list --
//

option_list::option_list(const char* optionName, int anIndex) {
   name = NULL;
   setIndex(anIndex);
   setName(optionName);
}



//////////////////////////////
//
// option_list::~option_list --
//

option_list::~option_list() {
   if (name != NULL) {
      delete [] name;
   }
   name = NULL;
}



//////////////////////////////
//
// option_list::getIndex --
//

int option_list::getIndex(void) {
   return index;
}



//////////////////////////////
//
// option_list::getName --
//

const char* option_list::getName(void) {
   return name;
}



//////////////////////////////
//
// option_list::setName --
//

void option_list::setName(const char* aString) {
   if (name != NULL) {
      delete [] name;
   }
   name = new char[strlen(aString) + 1];
   strcpy(name, aString);
}



//////////////////////////////
//
// option_list::setIndex --
//

void option_list::setIndex(int anIndex) {
   index = anIndex;
}






// md5sum:        4b6d0e9392a2ec14dbaf43373a060f90  Options_private.cpp
