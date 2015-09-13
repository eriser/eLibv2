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
            BaseEffect(std::string name = "BaseEffect") : BaseName(name), BaseConnection(1, 1)
            {
                vsa = (1.0 / 4294967295.0);   // Very small amount (Denormal Fix)
                filterOffset = 1.192092896e-07;
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
            virtual double Process(double Input) = 0;

        protected:
            double *pBuffer; ///< internal memory buffer used in subclasses
            double vsa;
            double filterOffset;
        };
    }
}

#endif
