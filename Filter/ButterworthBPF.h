#ifndef MODBUTTERWORTHBPF_H_
#define MODBUTTERWORTHBPF_H_

#include <Base/BaseFilter.h>
#include <Util/Defines.h>

namespace eLibV2
{
    namespace Filter
    {
        /**
        Implements a second-order Butterworth Bandpass-Filter
        */
        class ButterworthBPF : public Base::BaseFilter
        {
        public:
            ButterworthBPF(std::string name = "ButterworthBPF") :
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
                m_dBW = 0.5;
                calcCoefficients();
            }

        public:
            void calcCoefficients(void)
            {
                double a0 = 0.0, a1 = 0.0, a2 = 0.0, b1 = 0.0, b2 = 0.0;
                double dSamplerate = getSamplerate();

                if (m_iOrder == 2)
                {
                    double argtan = ModuleHelper::clamp(((PI * m_dCutoff * m_dBW) / dSamplerate), 0.000001, PI_DIV_2);
                    double C = 1.0 / tan(argtan);
                    double D = 2.0 * cos((2.0 * PI * m_dCutoff) / dSamplerate);

                    a0 = 1.0 / (1.0 + C);
                    a1 = 0.0;
                    a2 = -a0;
                    b1 = -a0 * (C * D);
                    b2 = a0 * (C - 1.0);
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
