//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Nov  2 23:39:21 PST 1996
// Last Modified: Tue Jan 27 22:44:01 GMT-0800 1998
// Filename:      ...sig/code/misc/sigSignal.h
// Syntax:        C++ 
//

#ifndef _SIGSIGNAL_H_INCLUDED
#define _SIGSIGNAL_H_INCLUDED

#include "sigTypes.h"
#include "sigDefines.h"
#include "helperFunctions.h"

#include "sigUtility.h"
#include "sigBase.h"


// Abstract Classes:
#include "Signal.h"
#include "Generator.h"
#include "Filter.h"

// Generators:
#include "Constant.h"
#include "Envelope.h"
#include "FixedSine.h"
#include "Function.h"
#include "Impulse.h"
#include "SoundFileIn.h"
#include "SoundFileInMemory.h"
#include "SoundLoop.h"
#include "UserIn.h"
#include "WhiteNoise.h"

// Filters:

#include "Add.h"
#include "Allpass.h"
#include "Allpass1.h"
#include "Comb.h"
#include "DCBlock.h"
#include "Delayline.h"
#include "Downsample.h"
#include "Empty.h"
#include "LogisticLimiter.h"
#include "Multiply.h"
#include "OnePole.h"
#include "Osc.h"
#include "Pan.h"
#include "Pan2.h"
#include "RandomDist.h"
#include "Resonator.h"
#include "Reverb.h"
#include "ReverbA.h"
#include "Reverberator.h"
#include "Scale.h"
#include "Selector.h"
#include "Sign.h"
#include "Smoother.h"
#include "SoundFileOut.h"
#include "Switch.h"
#include "UnitDelay.h"
#include "UserOut.h"
//#include "WaveTable.h"


// Computer Specific classes: (will be changed later)
#ifdef LINUX
   #include "LinuxSoundOut.h"
#endif


#endif  _SIGSIGNAL_H_INCLUDED


// md5sum:	83074fa6a25219e0490c242f0979dd52  sigSignal.h
