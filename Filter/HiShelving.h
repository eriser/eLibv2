#ifndef MODHISHELVING_H_
#define MODHISHELVING_H_

#include <Base/BaseFilter.h>
#include <Filter/EnhancedBiQuad.h>
#include <Util/Defines.h>
#include <cmath>

namespace eLibV2
{
    namespace Filter
    {
        /**
        Implements a hi-shelving filter
        */
        class HiShelving : public Base::BaseFilter
        {
        public:
            HiShelving(std::string name = "HiShelving") :
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
                m_dGain = 0.0;
                calcCoefficients();
            }

            void calcCoefficients(void)
            {
                double ThetaC = (2.0 * PI * m_dCutoff) / mSamplerate;
                ThetaC = ModuleHelper::minval(ThetaC, mMinimumThetaC);

                double Mu = pow(10, (m_dGain / 40.0));
                double Beta = (1.0 + Mu) / 4.0;

                double argtan = ModuleHelper::clamp((ThetaC / 2.0), -PI_DIV_2, PI_DIV_2);
                double Delta = Beta * tan(argtan);
                double Gamma = (1.0 - Delta) / (1.0 + Delta);

                double a0 = (1.0 + Delta) / 2.0;
                double a1 = (1.0 + Gamma) / -2.0;
                double a2 = 0.0;
                double b1 = -Gamma;
                double b2 = 0.0;
                double c0 = Mu - 1.0;
                double d0 = 1.0;

                if (m_pInternalBiquad)
                {
                    m_pInternalBiquad->setCoefficients(a0, a1, a2, b1, b2);
                    m_pInternalBiquad->setWetDryLevel(c0, d0);
                }
            }
        };
    }
}

#endif
