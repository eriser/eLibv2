#ifndef MODBASEEFFECT_H_
#define MODBASEEFFECT_H_

#include <Base/BaseModule.h>

#include <cmath>

namespace eLibV2
{
    namespace Base
    {
        /**
        this module provides basic methods used for signal processing in its subclasses
        */
        class BaseEffect : public BaseModule
        {
        public:
            BaseEffect(std::string name = "BaseEffect") :
                BaseName(name)
            {
                Init();
            }
            virtual ~BaseEffect() {}

            virtual void Init(void)
            {
                mVSA = (1.0 / 4294967295.0);
                mFilterOffset = 1.192092896e-07;
                m_bBypass = false;
            }

            /* getter/setter */
            virtual bool getBypass(void) { return m_bBypass; }
            virtual void setBypass(const bool Bypass) { m_bBypass = Bypass; }

            /**
            reset internal module state to initialization
            */
            virtual void Reset(void) = 0;

            /**
            process given input signal
            @param Input input signal
            @return the processed input signal
            */
            virtual double Process(const double Input) = 0;

        protected:
            double *pBuffer;        ///< internal memory buffer used in subclasses
            double mVSA;            ///< Very small amount (Denormal Fix)
            double mFilterOffset;
            bool m_bBypass;         ///< bypass processing of effect
        };
    }
}

#endif
