#ifndef MODBASEEFFECT_H_
#define MODBASEEFFECT_H_

#include <Base/BaseModule.h>

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
                BaseName(name),
                BaseConnection(1, 1)
            {
                Init();
            }
            virtual ~BaseEffect() {}

            virtual void Init(void)
            {
                mVSA = (1.0 / 4294967295.0);
                mFilterOffset = 1.192092896e-07;
                mMinimumThetaC = 0.0005;
            }

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
            double mMinimumThetaC;  ///< minimum value of theta_c used in filter-modules
        };
    }
}

#endif
