//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Nov 23 18:46:35 GMT-0800 1997
// Last Modified: Sat Nov 29 10:29:39 GMT-0800 1997
// Filename:      ...sig/code/base/EnvelopeString/EnvelopeString.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef _ENVELOPESTRING_H_INCLUDED
#define _ENVELOPESTRING_H_INCLUDED


#include "Array.h"

#define UNKNOWN_ENV     (0)
#define CLM_ENV         (1)
#define LISP_ENV        (2)
#define MUSICKIT_ENV    (3)
   #define MK_ENV          (3)
#define MATHEMATICA_ENV (4)
   #define MMA_ENV         (4)
#define PLAIN_ENV       (5)
#define SIG_ENV         (6)


class EnvelopeString {
   public:
                        EnvelopeString    (void);
                        EnvelopeString    (const char* aString);
                       ~EnvelopeString    ();
      
      int               getDimension      (void) const;
      const char*       getEnvelope       (void);
      double            getLength         (void) const;
      int               getNumPoints      (void) const;
      int               getStickIndex     (void) const;
      void              print             (void);
      void              removeStickPoint  (void);
      void              setEnvelope       (const char* aString, 
                                             int aDimension = -1,
                                             int stringType = UNKNOWN_ENV);
      void              setInterpolation  (char aType);
      void              setLength         (double aLength);
      void              setStickIndex     (int anIndex);
      void              setStickSamples   (void);
      void              setStickSeconds   (void);
      void              setSyntax         (int syntaxType);
      void              setSyntaxCLM      (void);
      void              setSyntaxLISP     (void);
      void              setSyntaxMK       (void);
      void              setSyntaxMMA      (void);
      void              setSyntaxPLAIN    (void);
      void              setSyntaxSIG      (void);

   protected:
      char*             envelope;         // string to hold in/out envelope
      int               stickIndex;       // location of the stick point
      int               outputType;       // syntax for output envelope
      int               modified;         // Q if need to recalc envelope
      char              absoluteType;     // secs or samples used for time
      double            normalization;    // last time value in envelope
      int               dimension;        // num items in env point
      char              defaultInterpolation;  // interp if local interp = 0
      double            defaultParameter; // parameter for default interp
      Array<double>*    points;           // points of envelope
      Array<char>       pointInterp;      // local interpolation types
      Array<double>     pointParameter;   // local interpolation parameters
 
   private:
      int               determineType     (const char* aString);
      double            extractNumber     (const char* aString, int& index);
      void              makeEnvelope      (void);
      void              makeCLMenv        (void);
      void              makeLISPenv       (void);
      void              makeMKenv         (void);
      void              makeMMAenv        (void);
      void              makePLAINenv      (void);
      void              makeSIGenv        (void);
      void              processCLMenv     (const char* aString, 
                                                           int dimension = -1);
      void              processLISPenv    (const char* aString);
      void              processMKenv      (const char* aString);
      void              processMMAenv     (const char* aString);
      void              processPLAINenv   (const char* aString,
                                                           int dimension = -1);
      void              processSIGenv     (const char* aString);
      void              skipSpace         (const char* aString, int& index,
                                             char* spaceString = "\n \t,");
      int               validateInterpolation (char anInterp);

      void              adjustForStick    (void);

};


ostream& operator<<(ostream& out, EnvelopeString& aString);



#endif  // _ENVELOPESTRING_H_INCLUDED



// md5sum: 2d992418356fa04f2805756450d4cfd7  EnvelopeString.h 
