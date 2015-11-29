#ifndef AUHELPER_H_
#define AUHELPER_H_

#ifdef __APPLE__

#include <AudioToolbox/AudioUnitUtilities.h>
#include <AudioUnit/AudioUnit.h>
#include <CoreAudio/CoreAudio.h>
#include <CoreServices/CoreServices.h>

namespace eLibV2
{
    namespace AU
    {
        class AUHelper
        {
        public:
            // --- helper method for setting up Parameter Info
            static void setAUParameterInfo(AudioUnitParameterInfo& outParameterInfo, 
                                           CFStringRef paramUnits,  
                                           Float32 fMinValue, 
                                           Float32 fMaxValue, 
                                           Float32 fDefaultValue,  
                                           bool bLogControl = false, 
                                           bool bStringListControl = false);
            
            // --- helper method for dealing with string-list control setup
            static void setAUParameterStringList(CFStringRef stringList, 
                                                 CFArrayRef* outStrings);
        };
    }
}

#endif

#endif