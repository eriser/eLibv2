#ifndef MODBASEMODULE_H_
#define MODBASEMODULE_H_

#include <Connection/BaseConnection.h>
#include <Util/Defines.h>

// std headers
#include <iostream>
#include <string>
#include <cstdarg>

namespace eLibV2
{
    namespace Base
    {
        /**
        this module provides basic methods used in most of all classes in the framework
        */
        class BaseModule : virtual public Connection::BaseConnection
        {
        public:
            BaseModule() :
                BaseName("BaseModule")
            {
                m_dSamplerate = 44100.0;
                m_dTempo = 120.0;
            }
            virtual ~BaseModule() {}

            /**
            initialization method for subclasses
            */
            virtual void Init(void) = 0;

            /**
            get currently set samplerate
            @return samplerate in Hz
            */
            virtual double getSamplerate(void) const { return m_dSamplerate; }

            /**
            set samplerate for module
            @param Samplerate samplerate in Hz
            */
            virtual void setSamplerate(const double Samplerate) { m_dSamplerate = Samplerate; }

            /**
            get currently set tempo
            @return tempo in beats-per-minute
            */
            virtual double getTempo(void) const { return m_dTempo; }

            /**
            set tempo for module
            @param Tempo tempo in beats-per-minute
            */
            virtual void setTempo(const double Tempo) { m_dTempo = Tempo; }

        protected:
            double m_dSamplerate;   ///< internally used samplerate
            double m_dTempo;        ///< Tempo to sync with
        };
    }
}

#endif
