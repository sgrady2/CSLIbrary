//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Jun 23 13:19:12 PDT 1998
// Last Modified: Tue Jun 23 13:19:17 PDT 1998
// Filename:      ...humdrum++/include/function_information.h
// Syntax:        C++ 
//
// Description:   Useful for debugging quickly
//


#ifndef _FUNCTION_INFORMATION_H_INCLUDED
#define _FUNCTION_INFORMATION_H_INCLUDED

// #define function_information(string)    cout << (string) << endl;

#ifdef DEBUG
   #ifndef function_information
      #define function_information(string)    cout << string << endl;
   #endif
#else
   #ifndef function_information
      #define function_information(string)    
   #endif
#endif



#endif // _FUNCTION_INFORMATION_H_INCLUDED



// md5sum:	87ece071f67691326b07e5f524d04f16  function_information.h
