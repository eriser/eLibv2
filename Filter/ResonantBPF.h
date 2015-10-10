#ifndef MODRESONANTBPF_H_
#define MODRESONANTBPF_H_

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
        class ResonantBPF : public Base::BaseFilter
        {
        public:
            ResonantBPF(std::string name = "ResonantBPF") :
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

            void calcCoefficients(void)
            {
                double ThetaC = (2.0 * PI * m_dCutoff) / mSamplerate;
                ThetaC = ModuleHelper::minval(ThetaC, mMinimumThetaC);

                double argtan = ThetaC * (m_dBW / 2.0);
                double BetaNumerator = 1.0 - tan(argtan);
                double BetaDenominator = 1.0 + tan(argtan);
                double Beta = 0.5 * (BetaNumerator / BetaDenominator);

                double Gamma = (0.5 + Beta) * (cos(ThetaC));
                double a0 = 0.5 - Beta;
                double a1 = 0.0;
                double a2 = -0.5 + Beta;
                double b1 = -2.0 * Gamma;
                double b2 = 2.0 * Beta;

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
