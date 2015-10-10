#ifndef MODBUTTERWORTHBSF_H_
#define MODBUTTERWORTHBSF_H_

#include <Base/BaseFilter.h>
#include <Util/Defines.h>
#include <cmath>

namespace eLibV2
{
    namespace Filter
    {
        /**
        Implements a second-order Butterworth Bandstop-Filter
        */
        class ButterworthBSF : public Base::BaseFilter
        {
        public:
            ButterworthBSF(std::string name = "ButterworthBSF") :
                BaseName(name),
                BaseFilter(),
                BaseConnection(FILTER_CONNECTION_NUM)
            {
                Init();
            }

            void Init()
            {
                m_bBypass = false;
                m_dCutoff = 22050.0;
                m_dBW = 0.5;
                calcCoefficients();
            }

        public:
            void calcCoefficients(void)
            {
                double argtan = ModuleHelper::clamp(((PI * m_dCutoff * m_dBW) / mSamplerate), -PI_DIV_2, PI_DIV_2);
                double C = tan(argtan);
                double D = 2.0 * cos((2.0 * PI * m_dCutoff) / mSamplerate);

                double a0 = 1.0 / (1.0 + C);
                double a1 = -a0 * D;
                double a2 = a0;
                double b1 = -a0 * D;
                double b2 = a0 * (1.0 - C);

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
