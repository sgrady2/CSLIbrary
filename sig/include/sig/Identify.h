//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Jul  9 14:25:29 PDT 1998
// Last Modified: Thu Jul  9 14:25:29 PDT 1998
// Filename:      ...humdrum++/src/Identify.cpp
// Syntax:        C++ 
// $Smake:        g++ -O -o %b %f && strip %b
//


#ifndef _IDENFITY_H_INCLUDED
#define _IDENFITY_H_INCLUDED


class Identify {
   public:

   // identifications dealing with **kern
      static int humdrumNullField(const char* aField);
      static int kernTimeSignature(const char* kernString);



};




#endif // _IDENFITY_H_INCLUDED




// md5sum:	2c2ff25496ee4c7956b1aa9d794b4b34  Identify.h
