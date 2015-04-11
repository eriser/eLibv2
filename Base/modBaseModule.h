#ifndef MODBASEMODULE_H_
#define MODBASEMODULE_H_

#include <Base/modBaseConnection.h>

#include <Util/modDefines.h>

// std headers
#include <iostream>
#include <string>
#include <cstdarg>

// vst headers
#include <audioeffectx.h>

namespace eLibV2
{
    namespace Base
    {
        /**
        this module provides basic methods used in its subclasses
        */
        class BaseModule : virtual public BaseConnection
        {
        public:
            BaseModule() : BaseName("BaseModule") {}

            /**
            initialization method for subclasses
            */
            virtual void Init(void) = 0;

            /**
            get currently set samplerate
            @return samplerate in Hz
            */
            virtual double getSamplerate(void) { return samplerate; }

            /**
            set samplerate for module
            @param Samplerate samplerate in Hz
            */
            virtual void setSamplerate(double Samplerate) { samplerate = Samplerate; }

        protected:
            double samplerate; ///< internally used samplerate
        };
    }
}

#endif
