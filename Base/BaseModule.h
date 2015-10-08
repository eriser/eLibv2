#ifndef MODBASEMODULE_H_
#define MODBASEMODULE_H_

#include <Base/BaseConnection.h>
#include <Util/Defines.h>

// std headers
#include <iostream>
#include <string>
#include <cstdarg>

#include <aeffect.h>

namespace eLibV2
{
    namespace Base
    {
        /**
        this module provides basic methods used in most of all classes in the framework
        */
        class BaseModule : virtual public BaseConnection
        {
        public:
            BaseModule() :
                BaseName("BaseModule")
            {}
            virtual ~BaseModule() {}

            /**
            initialization method for subclasses
            */
            virtual void Init(void) = 0;

            /**
            get currently set samplerate
            @return samplerate in Hz
            */
            virtual double getSamplerate(void) { return mSamplerate; }

            /**
            set samplerate for module
            @param Samplerate samplerate in Hz
            */
            virtual void setSamplerate(const double Samplerate) { mSamplerate = Samplerate; }

        protected:
            double mSamplerate; ///< internally used samplerate
        };
    }
}

#endif
