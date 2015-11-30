#ifndef MODVASEMFILTER_H_
#define MODVASEMFILTER_H_

#include <Filter/BaseVAFilter.h>
#include <Util/Defines.h>

namespace eLibV2
{
    namespace Filter
    {
        /**
        Implements a second-order Butterworth Bandpass-Filter
        */
        class VASEMFilter : public BaseVAFilter
        {
        public:
            VASEMFilter(std::string name = "VASEMFilter") :
                BaseName(name),
                BaseConnection(FILTER_CONNECTION_NUM),
                BaseVAFilter()
            {
                Init();
            }

            void Init(void)
            {
                m_bBypass = false;
                m_eType = TYPE_LPF;
                m_iOrder = 2;
                m_dCutoff = 22050.0;
                m_dQ = 0.5;

                m_dAlpha = 1.0;
                m_dAlpha0 = 1.0;
                m_dRho = 1.0;

                calcCoefficients();
            }

            virtual void Reset(void)
            {
                m_dZ11 = m_dZ12 = 0.0;
            }

            virtual double Process(double input)
            {
                double dOutput = input;

                if (
                    (m_iOrder == 2) &&
                    ((m_eType == TYPE_LPF) || (m_eType == TYPE_HPF) || (m_eType == TYPE_BPF) || (m_eType == TYPE_BSF))
                    )
                {
                    double hpf = m_dAlpha0 * (input - m_dRho * m_dZ11 - m_dZ12);
                    double bpf = m_dAlpha * hpf + m_dZ11;

                    if (m_bNLPActive)
                        bpf = tanh(m_dSaturation * bpf);

                    double lpf = m_dAlpha * bpf + m_dZ12;

                    double R = 1.0 / (2.0 * m_dQ);
                    double bsf = input - 2.0 * R * bpf;

                    double semBSF = m_dBSFRatio * hpf + (1.0 - m_dBSFRatio) * lpf;

                    m_dZ11 = m_dAlpha * hpf + bpf;
                    m_dZ12 = m_dAlpha * bpf + lpf;

                    if (m_eType == TYPE_LPF)
                        dOutput = lpf;
                    else if (m_eType == TYPE_HPF)
                        dOutput = hpf;
                    else if (m_eType == TYPE_BPF)
                        dOutput = bpf;
                    else if (m_eType == TYPE_BSF)
                        dOutput = semBSF;
                }
                return dOutput;
            }

        public:
            void setBSFRatio(const double BSFRatio) { m_dBSFRatio = BSFRatio; }
            double getBSFRatio(void) const { return m_dBSFRatio; }

        protected:
            void calcCoefficients(void)
            {
                double wd = 2.0 * PI * m_dCutoff;
                double T = 1.0 / getSamplerate();
                double argtan = ModuleHelper::clamp(PI * m_dCutoff / getSamplerate(), 0.000001, PI_DIV_2);
                double g = tan(argtan);

                double R = 1.0 / (2.0 * m_dQ);

                m_dAlpha0 = 1.0 / (1.0 + 2.0 * R * g + g * g);
                m_dAlpha = g;
                m_dRho = 2.0 * R + g;
            }

        private:
            double m_dAlpha, m_dAlpha0, m_dRho;
            double m_dZ11, m_dZ12;
            double m_dBSFRatio;
        };
    }
}

#endif
