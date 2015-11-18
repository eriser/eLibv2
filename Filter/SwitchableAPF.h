#ifndef MODSWITCHABLEAPF_H_
#define MODSWITCHABLEAPF_H_

#include <Base/BaseFilter.h>
#include <Util/Defines.h>

namespace eLibV2
{
    namespace Filter
    {
        /**
        Implements a switchable Allpass-Filter
        */
        class SwitchableAPF : public Base::BaseFilter
        {
        public:
            SwitchableAPF(std::string name = "SwitchableAPF") :
                BaseName(name),
                BaseConnection(FILTER_CONNECTION_NUM),
                BaseFilter()
            {
                Init();
            }

            void Init(void)
            {
                m_bBypass = false;
                m_dCutoff = 22050.0;
                m_dQ = 0.707;
                m_iOrder = 2;
                calcCoefficients();
            }

        public:
            /* getter/setter */
            bool getSecondOrder() { return m_bSecondOrder; }
            void setSecondOrder(const bool SecondOrder)
            {
                m_bSecondOrder = SecondOrder;
                calcCoefficients();
            }

            void calcCoefficients(void)
            {
                double a0 = 0.0, a1 = 0.0, a2 = 0.0, b1 = 0.0, b2 = 0.0;
                double dSamplerate = getSamplerate();

                if (m_iOrder == 1)
                {
                    double argtan = ModuleHelper::clamp(((PI * m_dCutoff) / dSamplerate), -PI_DIV_2, PI_DIV_2);
                    double Alpha = (tan(argtan) - 1.0) / (tan(argtan) + 1.0);

                    a0 = Alpha;
                    a1 = 1.0;
                    a2 = 0.0;
                    b1 = Alpha;
                    b2 = 0.0;
                }
                else if (m_iOrder == 2)
                {
                    double argtan = ModuleHelper::clamp(((PI * m_dQ) / dSamplerate), -PI_DIV_2, PI_DIV_2);
                    double Alpha = (tan(argtan) - 1.0) / (tan(argtan) + 1.0);
                    double ThetaC = 2.0 * PI * m_dCutoff / dSamplerate;
                    double Beta = -cos(ThetaC);

                    a0 = -Alpha;
                    a1 = Beta * (1.0 - Alpha);
                    a2 = 1.0;
                    b1 = Beta * (1.0 - Alpha);
                    b2 = -Alpha;
                }

                if (m_pInternalBiquad)
                {
                    m_pInternalBiquad->setCoefficients(a0, a1, a2, b1, b2);
                    m_pInternalBiquad->setWetDryLevel(1.0, 0.0);
                }
            }

        private:
            bool m_bSecondOrder;
        };
    }
}

#endif
