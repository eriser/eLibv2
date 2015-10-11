#ifndef MODLOSHELVING_H_
#define MODLOSHELVING_H_

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
        class LoShelving : public Base::BaseFilter
        {
        public:
            LoShelving(std::string name = "LoShelving") :
                BaseName(name),
                BaseFilter(),
                BaseConnection(FILTER_CONNECTION_NUM)
            {
                Init();
            }

            void Init()
            {
                m_bBypass = false;
                m_iOrder = 1;
                m_dCutoff = 22050.0;
                m_dGain = 0.0;
                calcCoefficients();
            }

            void calcCoefficients(void)
            {
                double a0 = 0.0, a1 = 0.0, a2 = 0.0, b1 = 0.0, b2 = 0.0;
                double c0 = 0.0, d0 = 0.0;

                if (m_iOrder == 1)
                {
                    double ThetaC = (2.0 * PI * m_dCutoff) / mSamplerate;
                    ThetaC = ModuleHelper::minval(ThetaC, mMinimumThetaC);

                    double Mu = pow(10, (m_dGain / 20.0));
                    double Beta = 4.0 / (1.0 + Mu);

                    double argtan = ModuleHelper::clamp((ThetaC / 2.0), -PI_DIV_2, PI_DIV_2);
                    double Delta = Beta * tan(argtan);
                    double Gamma = (1.0 - Delta) / (1.0 + Delta);

                    a0 = (1.0 - Gamma) * 0.5;
                    a1 = (1.0 - Gamma) * 0.5;
                    a2 = 0.0;
                    b1 = -Gamma;
                    b2 = 0.0;
                    c0 = Mu - 1.0;
                    d0 = 1.0;
                }

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
