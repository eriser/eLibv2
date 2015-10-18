#ifndef MODBUTTERWORTHHPF_H_
#define MODBUTTERWORTHHPF_H_

#include <Base/BaseFilter.h>
#include <Util/Defines.h>
#include <cmath>

namespace eLibV2
{
    namespace Filter
    {
        /**
        Implements a second-order Butterworth Highpass-Filter
        */
        class ButterworthHPF : public Base::BaseFilter
        {
        public:
            ButterworthHPF(std::string name = "ButterworthHPF") :
                BaseName(name),
                BaseFilter(),
                BaseConnection(FILTER_CONNECTION_NUM)
            {
                Init();
            }

            void Init(void)
            {
                m_bBypass = false;
                m_iOrder = 2;
                m_dCutoff = 22050.0;
                calcCoefficients();
            }

            void calcCoefficients(void)
            {
                double a0 = 0.0, a1 = 0.0, a2 = 0.0, b1 = 0.0, b2 = 0.0;

                if (m_iOrder == 2)
                {
                    double argtan = ModuleHelper::clamp(((PI * m_dCutoff) / mSamplerate), -PI_DIV_2, PI_DIV_2);
                    double C = tan(argtan);
                    double C2 = C * C;

                    a0 = 1.0 / (1.0 + SQRT_2 * C + C2);
                    a1 = -2.0 * a0;
                    a2 = a0;
                    b1 = 2.0 * a0 * (C2 - 1.0);
                    b2 = a0 * (1.0 - SQRT_2 * C + C2);
                }

                if (m_pInternalBiquad)
                {
                    m_pInternalBiquad->setCoefficients(a0, a1, a2, b1, b2);
                    m_pInternalBiquad->setWetDryLevel(1.0, 0.0);
                }
            }
        };
    }
}

#endif
