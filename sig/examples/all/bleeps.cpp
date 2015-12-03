//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Apr 14 19:05:47 PDT 1998
// Last Modified: Wed Apr 15 00:18:13 PDT 1998
// Filename:      ...sig/doc/examples/sig/sigfile/bleeps/bleeps.cpp
// Syntax:        C++; sig
//
// Description:   creates a unique sonic texture 
//

#include "sig.h"
#include <iostream.h>
#include <stdlib.h>
#include <math.h>
#include <strstream.h>

void checkOptions(Options& opts, int argc, char** argv);
void distributions(void);
void example(void);
void getRange(const char* aString, double& p1, double& p2, double& p3);
void usage(const char* command);


///////////////////////////////////////////////////////////////////////////

Distribution randFrequency;         // random number generator for freq
Distribution randAmplitude;         // random number generator for amp
Distribution randTime;              // random number generator for time

double freq1, freq2, freq3;         // parameters for frequencies in Hertz
double amp1, amp2, amp3;            // parameters for amplitudes in dB
double time1, time2, time3;         // parameters for time durations in seconds

double amp = 0;
double ampold = 0;

int main(int argc, char* argv[]) {
   Options options;
   checkOptions(options, argc, argv);

   int maxSamples;
   int numSamples;
   int sampleCount = 0;
   if (options.getInt("samples") > 0) {
      maxSamples = options.getInt("samples");
   } else {
      maxSamples = (int)(options.getDouble("duration") * 44100 + 0.5);  
   }

   // prepare for a monophonic output file at 44.1 kHz sampling rate
   SoundHeader header;
   header.setHighMono();

   // Elements:
   Constant     frequency;
   Envelope     ampenv("0 0 1 1", 1000);
   Osc          osc;
   SoundFileOut outsound(options.getArg(1), header);

   // Connections:
   osc.connect(frequency, 0);
   osc.connect(ampenv, 1);
   outsound.connect(osc);

   Action action;

   strstream *ampstring;

   numSamples = (int)(fabs(randTime.value()) * 44100 + 0.5);
   ampold = amp;
   amp = pow(10.0, randAmplitude.value()/20);
   ampstring = new strstream;
   *ampstring << "0 " << ampold << " 1 " << amp << ends;
   ampenv.setDuration(numSamples+1);
   ampenv.setEnvelope(ampstring->str());
   delete ampstring;

   do {
      ampstring = new strstream;
      frequency.setValue(fabs(randFrequency.value()));
      numSamples = (int)(fabs(randTime.value()) * 44100 + 0.5);
      ampold = amp;
      amp = pow(10.0, randAmplitude.value()/20);
      *ampstring << "0 " << ampold << " 1 " << amp << ends;
      ampenv.setDuration(numSamples+1);
      ampenv.setEnvelope(ampstring->str());
      sampleCount += numSamples;
      action.tick(outsound, numSamples);
      delete ampstring;
   } while (sampleCount < maxSamples);

   return 0;
}


///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// checkOptions -- handle command-line options.
//

void checkOptions(Options& opts, int argc, char** argv) {
   opts.define("f|freq|frequency=s:440 880 Hertz");
   opts.define("a|amp|amplitude=s:-20 -5 dB");
   opts.define("t|time=s:0.1 1.0 second");
   opts.define("ftype|f-type=s:uniform");
   opts.define("atype|a-type=s:uniform");
   opts.define("ttype|t-type=s:uniform");
   opts.define("d|dur|duration=s:0.1 1.0 second");
   opts.define("s|samples=i");
   opts.define("distributions=b");
   opts.define("author=b");
   opts.define("version=b");
   opts.define("example=b");
   opts.define("help=b");
   opts.process(argc, argv);

   if (opts.getBoolean("author")) {
      cout << "Written by Craig Stuart Sapp, "
           << "craig@ccrma.stanford.edu, April 1998" << endl;
      exit(0);
   }
   if (opts.getBoolean("version")) {
      cout << "compiled: " << __DATE__ << endl;
      cout << SIG_VERSION << endl;
      exit(0);
   }
   if (opts.getBoolean("help")) {
      usage(opts.getCommand());
      exit(0);
   }
   if (opts.getBoolean("example")) {
      example();
      exit(0);
   }
   if (opts.getBoolean("distributions")) {
      distributions();
      exit(0);
   }

   getRange(opts.getString("frequency"), freq1, freq2, freq3);
   getRange(opts.getString("amplitude"), amp1, amp2, amp3);
   getRange(opts.getString("time"), time1, time2, time3);


   if (strcmp("gaussian", opts.getString("ftype")) == 0) {
      randFrequency.doGaussian(freq2, freq1);
   } else if (strcmp("triangular", opts.getString("ftype")) == 0) {
      randFrequency.doTriangular(freq1, freq3, freq2);
   } else if (strcmp("cauchy", opts.getString("ftype")) == 0) {
      randFrequency.doCauchy(freq2, freq1);
   } else {
      randFrequency.doUniform(freq1, freq2);
   }
      

   if (strcmp("gaussian", opts.getString("atype")) == 0) {
      randAmplitude.doGaussian(amp2, amp1);
   } else if (strcmp("triangular", opts.getString("atype")) == 0) {
      randAmplitude.doTriangular(amp1, amp3, amp2);
   } else if (strcmp("cauchy", opts.getString("atype")) == 0) {
      randAmplitude.doCauchy(amp2, amp1);
   } else {
      randAmplitude.doUniform(amp1, amp2);
   }
      

   if (strcmp("gaussian", opts.getString("ttype")) == 0) {
      randTime.doGaussian(time2, time1);
   } else if (strcmp("triangular", opts.getString("ttype")) == 0) {
      randTime.doTriangular(time1, time3, time2);
   } else if (strcmp("cauchy", opts.getString("ttype")) == 0) {
      randTime.doCauchy(time2, time1);
   } else {
      randTime.doUniform(time1, time2);
   }
      

   // can only have one output filename
   if (opts.getArgCount() == 0) {
      cout << "Error: need one output file name." << endl;
      usage(opts.getCommand());
      exit(1);
   } else if (opts.getArgCount() > 1) {
      cout << "Error: too many arguments.  Given "
           << opts.getArgCount() << " but need only 1." << endl;
      usage(opts.getCommand());
      exit(1);
   }

}



//////////////////////////////
//
// distributions -- print the types of distributions available
//

void distributions(void) {
   cout << 
" Types of random distributions availble:                                   \n"
"                                                                           \n"
"   uniform                                                                 \n"
"	parameter 1 = minimum value                                         \n"
" 	parameter 2 = maximum value                                         \n"
"                                                                           \n"
"   gaussian                                                                \n"
"	parameter 1 = center value                                          \n"
"	parameter 2 = deviation value                                       \n"
"                                                                           \n"
"   triangle                                                                \n"
"	parameter 1 = minimum value                                         \n"
"	parameter 2 = maximum value                                         \n"
"	parameter 3 = peak frequency (betweem min and max frequency)        \n"
"                                                                           \n"
"   cauchy                                                                  \n"
"	parameter 1 = center value                                          \n"
"	parameter 2 = spread value                                          \n"
"                                                                           \n"
" note negative frequencies and times are converted to positive frequencies.\n"
   << endl;
};



//////////////////////////////
//
// example -- gives example calls to the osc program.
//

void example(void) {
   cout <<
   "#                                                                      \n"
   "#                                                                      \n"
   << endl;
}



//////////////////////////////
//
// getRange -- extracts three numbers from a string.
//

void getRange(const char* aString, double& p1, double& p2, double& p3) {
   char *temp = new char[strlen(aString)+1];
   char *number;

   strcpy(temp, aString);
   number = strtok(temp, " \n\t,(){}[]|");
   if (number == NULL) {
      cout << "Error: string \"" << aString << "\" contains no numbers" << endl;
      exit(1);
   }
   p1 = strtod(number, NULL);
   number = strtok(NULL, " \n\t,(){}[]|");
   if (number == NULL) {
      p3 = p2 = p1;
      return;
   } 
   p2 = strtod(number, NULL);

   number = strtok(NULL, " \n\t,(){}[]|");
   if (number == NULL) {
      p3 = p2;
      return;
   } else {
      p3 = strtod(number, NULL);
   }
}



//////////////////////////////
//
// usage -- how to run the bleeps program on the command line.
//

void usage(const char* command) {
   cout << endl;
   cout << "Generates random sinewaves." << endl;
   cout << endl;
   cout << "Usage: " << command << " [-d duration|-s samples][-a amp-env]"
            "[-h hold] outsound" << endl;
   cout << endl;
   cout << "Options:" << endl;
   cout << "   -d = duration of the output in seconds. (default 1.0) \n";
   cout << "   -s = duration of the output in samples. (deault is null)\n";
   cout << "        will override the -d option if specified\n";
   cout << "   -a = amplitude range (default \"-20 -5\") \n";
   cout << "   -f = frequency range (default \"440 880\") \n";
   cout << "   -t = time range (default \"0.1 1.0 seconds\") \n";
   cout << "   --ftype = frequency distribution type (default uniform) \n";
   cout << "   --atype = amplitude distribution type (default uniform) \n";
   cout << "   --ttype = time distribution type (default uniform) \n";
   cout << "   --options = list of all options, aliases and defaults \n";
   cout << "\n";
   cout << "Distribution types: uniform, gaussian, triangle, and cauchy.\n";
   cout << "   use the --distributions options to see more details on\n";
   cout << "   the distribution types\n";
   cout << endl;
}



