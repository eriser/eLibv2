#ifndef MODRESONANTLPF_H_
#define MODRESONANTLPF_H_

#include <Base/BaseFilter.h>
#include <Util/Defines.h>
#include <cmath>

namespace eLibV2
{
    namespace Filter
    {
        /**
        Implements a single Bi-Quad Structure
        */
        class ResonantLPF : public Base::BaseFilter
        {
        public:
            ResonantLPF(std::string name = "ResonantLPF") :
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
                m_dQ = 5.0;
                calcCoefficients();
            }

            void calcCoefficients(void)
            {
                double ThetaC = (2.0 * PI * m_dCutoff) / mSamplerate;
                ThetaC = ModuleHelper::minval(ThetaC, mMinimumThetaC);

                double a0 = 0.0, a1 = 0.0, a2 = 0.0, b1 = 0.0, b2 = 0.0;

                if (m_iOrder == 1)
                {
                    double Gamma = cos(ThetaC) / (1.0 + sin(ThetaC));

                    a0 = (1.0 - Gamma) * 0.5;
                    a1 = a0;
                    a2 = 0.0;
                    b1 = -Gamma;
                    b2 = 0.0;
                }
                else if (m_iOrder == 2)
                {
                    double d = 1.0 / m_dQ;

                    double BetaNumerator = 1.0 - ((d / 2.0) * (sin(ThetaC)));
                    double BetaDenominator = 1.0 + ((d / 2.0) * (sin(ThetaC)));
                    double Beta = 0.5 * (BetaNumerator / BetaDenominator);

                    double Gamma = (0.5 + Beta) * (cos(ThetaC));
                    a0 = (0.5 + Beta - Gamma) / 2.0;
                    a1 = (0.5 + Beta - Gamma);
                    a2 = (0.5 + Beta - Gamma) / 2.0;
                    b1 = -2.0 * Gamma;
                    b2 = 2.0 * Beta;
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
