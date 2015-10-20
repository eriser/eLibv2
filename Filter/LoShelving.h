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
                    double ThetaC = (PI * m_dCutoff) / getSamplerate();
                    ThetaC = ModuleHelper::clamp((ThetaC), -PI_DIV_2, PI_DIV_2);

                    double Mu = pow(10, (m_dGain / 20.0));
                    double Beta = 4.0 / (1.0 + Mu);

                    double Delta = Beta * tan(ThetaC);
                    double Gamma = (1.0 - Delta) / (1.0 + Delta);

                    a0 = (1.0 - Gamma) * 0.5;
                    a1 = (1.0 - Gamma) * 0.5;
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

                    double Alpha = 1.0 + SQRT_2 * sqrt(V0) * K + V0 * K2;
                    double Beta = 2.0 * (V0 * K2 - 1.0);
                    double Gamma = 1.0 - SQRT_2 * sqrt(V0) * K + V0 * K2;
                    double Delta = 2.0 * (K2 - 1.0);
                    double Eta = 1.0 - SQRT_2 * K + K2;

                    double Alpha2 = 1.0 + SQRT_2 * K / sqrt(V0) + K2 / V0;
                    double Beta2 = 2.0 * ((K2 / V0) - 1.0);
                    double Gamma2 = 1.0 - SQRT_2 * K / sqrt(V0) + K2 / V0;

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
                        a0 = D0 / Alpha2;
                        a1 = Delta / Alpha2;
                        a2 = Eta / Alpha2;
                        b1 = Beta2 / Alpha2;
                        b2 = Gamma2 / Alpha2;
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
