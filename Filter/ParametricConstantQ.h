#ifndef MODPARAMETRICCONSTANTQ_H_
#define MODPARAMETRICCONSTANTQ_H_

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
        class ParametricConstantQ : public Base::BaseFilter
        {
        public:
            ParametricConstantQ(std::string name = "ParametricConstantQ") :
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
                m_dQ = 0.707;
                calcCoefficients();
            }

            void calcCoefficients(void)
            {
                double argtan = ModuleHelper::clamp(((PI * m_dCutoff) / mSamplerate), -PI_DIV_2, PI_DIV_2);

                double K = tan(argtan);
                double K2 = K * K;
                double Mu = pow(10.0, (m_dGain / 20.0));
                double D0 = 1.0 + (1.0 / m_dQ) * K + K2;
                double E0 = 1.0 + (1.0 / (Mu * m_dQ)) * K + K2;

                double Alpha = 1.0 + (Mu / m_dQ) * K + K2;
                double Beta = 2.0 * (K2 - 1.0);
                double Gamma = 1.0 - (Mu / m_dQ) * K + K2;
                double Delta = 1.0 - (1.0 / m_dQ) * K + K2;
                double Eta = 1.0 - (1.0 / (Mu * m_dQ)) * K + K2;

                double a0, a1, a2, b1, b2, c0, d0;
                if (m_dGain > 0.0)
                {
                    a0 = Alpha / D0;
                    a1 = Beta / D0;
                    a2 = Gamma / D0;
                    b1 = Beta / D0;
                    b2 = Delta / D0;
                }
                else
                {
                    a0 = D0 / E0;
                    a1 = Beta / E0;
                    a2 = Delta / E0;
                    b1 = Beta / E0;
                    b2 = Eta / E0;
                }
                c0 = 1.0;
                d0 = 0.0;

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
