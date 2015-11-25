#ifndef MODBASEENVELOPE_H_
#define MODBASEENVELOPE_H_

#include <Base/BaseModule.h>

namespace eLibV2
{
    namespace Envelope
    {
        /**
        this module provides methods used by envelope-modules
        */
        class BaseEnvelope : public Base::BaseModule
        {
        public:
            /**
            enumeration of possible envelope modes:
            - linear
            - logarithmic
            - exponential
            */
            enum EnvelopeScale
            {
                SCALE_UNDEF = 0,
                SCALE_LINEAR,
                SCALE_LOGARITHMIC,
                SCALE_EXPONENTIAL
            };

            enum EnvelopeMode
            {
                MODE_UNDEF = 0,
                MODE_ANALOG,
                MODE_DIGITAL
            };

            enum EnvelopeState
            {
                STATE_OFF = 0,
                STATE_DELAY,
                STATE_ATTACK,
                STATE_HOLD,
                STATE_DECAY,
                STATE_SUSTAIN,
                STATE_RELEASE,
                STATE_SHUTDOWN
            };

        public:
            BaseEnvelope(std::string name = "BaseEnvelope") :
                Base::BaseName(name) {}

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
            virtual void setActive(const bool Active) { m_bActive = Active; }

            virtual bool getActive(void) const { return m_bActive; }

            /**
            set envelope scale
            @param Scale envelope state to set
            */
            virtual void setScale(const EnvelopeScale Scale) { m_eScale = Scale; }

            virtual EnvelopeScale getScale(void) const { return m_eScale; }

            /**
            set envelope mode
            @param Mode envelope mode to set
            */
            virtual void setMode(const EnvelopeMode Mode) { m_eMode = Mode; }

            virtual EnvelopeState getState(void) const { return m_eState; }

            /**
            set envelope state
            @param State envelope state to set
            */
            virtual void setState(const EnvelopeState State) { m_eState = State; }

            virtual void start(void) {}
            virtual void stop(void) {}

        protected:
            bool m_bActive; ///< internal active state
            double m_dCurrentOutput; ///< last calculated level for aliasing

            EnvelopeScale m_eScale;
            EnvelopeMode m_eMode; ///< internal envelope mode
            EnvelopeState m_eState;
        };
    }
}

#endif
