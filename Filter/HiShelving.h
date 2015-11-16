#ifndef MODHISHELVING_H_
#define MODHISHELVING_H_

#include <Base/BaseFilter.h>
#include <Filter/EnhancedBiQuad.h>
#include <Util/Defines.h>

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

            void Init(void)
            {
                m_bBypass = false;
                m_iOrder = 2;
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
                    double ThetaC = (2.0 * PI * m_dCutoff) / getSamplerate();
                    ThetaC = ModuleHelper::minval(ThetaC, mMinimumThetaC);

                    double Mu = pow(10, (m_dGain / 20.0));
                    double Beta = (1.0 + Mu) / 4.0;

                    double argtan = ModuleHelper::clamp((ThetaC / 2.0), -PI_DIV_2, PI_DIV_2);
                    double Delta = Beta * tan(argtan);
                    double Gamma = (1.0 - Delta) / (1.0 + Delta);

                    a0 = (1.0 + Gamma) * 0.5;
                    a1 = (1.0 + Gamma) * -0.5;
                    a2 = 0.0;
                    b1 = -Gamma;
                    b2 = 0.0;
                    c0 = Mu - 1.0;
                    d0 = 1.0;
                }
                else if (m_iOrder == 2)
                {
                    double argtan = ModuleHelper::clamp(((PI * m_dCutoff) / getSamplerate()), -PI_DIV_2, PI_DIV_2);

                    double K = tan(argtan);
                    double K2 = K * K;
                    double V0 = pow(10.0, (m_dGain / 20.0));
                    double D0 = 1.0 + SQRT_2 * K + K2;
                    double E0 = 1.0 / V0 + SQRT_2 / sqrt(V0) * K + K2;
                    double E1 = 1.0 + SQRT_2 * sqrt(V0) * K + K2 * V0;

                    double Alpha = V0 + SQRT_2 * sqrt(V0) * K + K2;
                    double Beta = 2.0 * (K2 - V0);
                    double Gamma = V0 - SQRT_2 * sqrt(V0) * K + K2;
                    double Delta = 2.0 * (K2 - 1.0);
                    double Eta = 1.0 - SQRT_2 * K + K2;
                    double Zeta = 2.0 * (K2 * V0 - 1.0);
                    double Iota = 1.0 - SQRT_2 * sqrt(V0) * K + K2 * V0;

                    if (m_dGain > 0.0)
                    {
                        a0 = Alpha / D0;
                        a1 = Beta / D0;
                        a2 = Gamma / D0;
                        b1 = Delta / D0;
                        b2 = Eta / D0;
                    }
                    else
                    {
                        a0 = D0 / E0;
                        a1 = Delta / E0;
                        a2 = Eta / E0;
                        b1 = Zeta / E1;
                        b2 = Iota / E1;
                    }
                    c0 = 1.0;
                    d0 = 0.0;
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
