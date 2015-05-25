#ifndef MODBASEENVELOPE_H_
#define MODBASEENVELOPE_H_

#include <Base/BaseModule.h>

namespace eLibV2
{
    namespace Base
    {
        /**
        this module provides methods used by envelope-modules
        */
        class BaseEnvelope : public BaseModule
        {
        protected:

            /**
            enumeration of possible envelope modes:
            - linear
            - logarithmic
            - exponential
            */
            enum EnvelopeMode
            {
                ENVELOPE_MODE_UNDEF = 0,
                ENVELOPE_MODE_LINEAR,
                ENVELOPE_MODE_LOGARITHMIC,
                ENVELOPE_MODE_EXPONENTIAL
            };

        public:
            BaseEnvelope(std::string name = "BaseEnvelope") : BaseName(name) {}

            /**
            reset internal module state to initialization
            */
            virtual void Reset(void) = 0;

            /**
            generate envelope output and adjust internal state
            */
            virtual double Process(void) = 0;

            /**
            activate/deactivate processing
            @param Active active state of module
            */
            virtual void setActive(bool Active) { bActive = Active; }

            /**
            set envelope mode
            @param Mode envelope mode to set
            */
            virtual void setEnvelopeMode(EnvelopeMode Mode) { lEnvelopeMode = Mode; }

            /**
            set trigger for internal state machine
            @param Trigger trigger active/inactive
            */
            virtual void setTrigger(bool Trigger) { bTrigger = Trigger; }

        protected:
            bool bActive; ///< internal active state
            bool bTrigger; ///< internal trigger state
            bool bOldTrigger; ///< last set trigger state
            double dLastLevel; ///< last calculated level for aliasing

            EnvelopeMode lEnvelopeMode; ///< internal envelope mode
        };
    }
}

#endif
