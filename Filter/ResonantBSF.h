#ifndef MODRESONANTBSF_H_
#define MODRESONANTBSF_H_

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
        class ResonantBSF : public Base::BaseFilter
        {
        public:
            ResonantBSF(std::string name = "ResonantBSF") :
                BaseName(name),
                BaseFilter(),
                BaseConnection(FILTER_CONNECTION_NUM)
            {
                Init();
            }

            void Init()
            {
                m_bBypass = false;
                m_iOrder = 2;
                m_dCutoff = 22050.0;
                m_dBW = 0.5;
                calcCoefficients();
            }

            void calcCoefficients(void)
            {
                double a0 = 0.0, a1 = 0.0, a2 = 0.0, b1 = 0.0, b2 = 0.0;

                if (m_iOrder == 2)
                {
                    double ThetaC = (2.0 * PI * m_dCutoff) / mSamplerate;
                    ThetaC = ModuleHelper::minval(ThetaC, mMinimumThetaC);

                    double argtan = ThetaC * (m_dBW / 2.0);
                    double BetaNumerator = 1.0 - tan(argtan);
                    double BetaDenominator = 1.0 + tan(argtan);
                    double Beta = 0.5 * (BetaNumerator / BetaDenominator);

                    double Gamma = (0.5 + Beta) * (cos(ThetaC));
                    a0 = 0.5 + Beta;
                    a1 = -2.0 * Gamma;
                    a2 = 0.5 + Beta;
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
