#ifndef MODLINKWITZRILEYLPF_H_
#define MODLINKWITZRILEYLPF_H_

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
        class LinkwitzRileyLPF : public Base::BaseFilter
        {
        public:
            LinkwitzRileyLPF(std::string name = "LinkwitzRileyLPF") :
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
                calcCoefficients();
            }

            void setCutoff(const double cutoff)
            {
                m_dCutoff = cutoff;
                calcCoefficients();
            }

            virtual void setSamplerate(const double Samplerate)
            {
                BaseModule::setSamplerate(Samplerate);
                calcCoefficients();
            }

            void calcCoefficients(void)
            {
                double a0 = 0.0, a1 = 0.0, a2 = 0.0, b1 = 0.0, b2 = 0.0;

                if (m_iOrder == 2)
                {
                    double OmegaC = PI * m_dCutoff;
                    double OmegaC2 = OmegaC * OmegaC;
                    double ThetaC = ModuleHelper::clamp((OmegaC / mSamplerate), -PI_DIV_2, PI_DIV_2);

                    double Kappa = OmegaC / tan(ThetaC);
                    double Kappa2 = Kappa * Kappa;
                    double Delta = Kappa2 + 2.0 * Kappa * OmegaC + OmegaC2;

                    a0 = OmegaC2 / Delta;
                    a1 = 2.0 * a0;
                    a2 = a0;
                    b1 = (2.0 * OmegaC2 - 2.0 * Kappa2) / Delta;
                    b2 = (Kappa2 - 2.0 * Kappa * OmegaC + OmegaC2) / Delta;
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
