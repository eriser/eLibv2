#ifndef MODDATASAMPLER_H_
#define MODDATASAMPLER_H_

#ifdef WIN32
#include <windows.h>
#endif

#include <cstdio>
#include <iostream>
#include <sstream>

#include <Util/Types.h>

namespace eLibV2
{
    namespace Util
    {
        /**
        The DataSampler is used for sampling huge amounts of data while debugging.
        Since output of every value is usually slow the output is done only after SampleDuration samples have been received.
        This outputs only a few values but the series of values can be mostly considered monotonous and thus analyzed.

        By default SampleDuration is set to 1000 but it can be modified using setSampleDuration.
        */
        class DataSampler
        {
        public:
            /**
            Set sample duration
            @param sampleDuration the number of samples to be taken before output is done
            */
            static void setSampleDuration(const UInt16 sampleDuration) { ms_SampleDuration = sampleDuration; }

            /**
            Set the message prefix used for output
            @param messagePrefix string to be prepended to sampled value on output
            */
            static void setMessagePrefix(const std::string& messagePrefix) { ms_MessagePrefix = messagePrefix; }

            /**
            Sample an integer value
            @param input the integer value to be sampled
            */
            static void sample(const SInt16 input);

            /**
            Sample a double value
            @param input the double value to be sampled
            */
            static void sample(const double input);

        private:
            DataSampler();
            static UInt16 ms_SampleDuration;
            static std::string ms_MessagePrefix;
            static UInt16 ms_CurrentlySampled;
        };
    }
}

#endif
