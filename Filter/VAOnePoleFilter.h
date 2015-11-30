#ifndef MODVAONEPOLEFILTER_H_
#define MODVAONEPOLEFILTER_H_

#include <Filter/BaseVAFilter.h>
#include <Util/Defines.h>

namespace eLibV2
{
    namespace Filter
    {
        /**
        Implements a second-order Butterworth Bandpass-Filter
        */
        class VAOnePoleFilter : public BaseVAFilter
        {
        public:
            VAOnePoleFilter(std::string name = "VAOnePoleFilter") :
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
                m_iOrder = 1;
                m_dCutoff = 22050.0;

                m_dAlpha = 1.0;
                m_dBeta = 0.0;
                m_dGamma = 1.0;
                m_dDelta = 0.0;
                m_dEpsilon = 0.0;
                m_dA0 = 1.0;

                calcCoefficients();
            }

            virtual void Reset(void)
            {
                m_dZ1 = 0.0;
                m_dFeedback = 0.0;
            }

            virtual double Process(double input)
            {
                double dOutput = input;

                if (
                    (m_iOrder == 1) &&
                    ((m_eType == TYPE_LPF) || (m_eType == TYPE_HPF))
                    )
                {
                    input = input * m_dGamma + m_dFeedback + m_dEpsilon * getFeedbackOutput();
                    double vn = (m_dA0 * input - m_dZ1) * m_dAlpha;
                    double lpf = vn + m_dZ1;
                    m_dZ1 = vn + lpf;
                    double hpf = input - lpf;

                    if (m_eType == TYPE_LPF)
                        dOutput = lpf;
                    else if (m_eType == TYPE_HPF)
                        dOutput = hpf;
                }
                return dOutput;
            }

        public:
            void setFeedback(const double Feedback) { m_dFeedback = Feedback; }
            double getFeedbackOutput(void) const { return m_dBeta * (m_dZ1 + m_dFeedback * m_dDelta); }

        protected:
            void calcCoefficients(void)
            {
                double wd = 2.0 * PI * m_dCutoff;
                double T = 1.0 / getSamplerate();
                double argtan = ModuleHelper::clamp(PI * m_dCutoff / getSamplerate(), 0.000001, PI_DIV_2);
                double g = tan(argtan);

                m_dAlpha = g / (1.0 + g);
            }

        private:
            double m_dAlpha, m_dBeta, m_dGamma, m_dDelta, m_dEpsilon;
            double m_dZ1;
            double m_dA0;
            double m_dFeedback;
        };
    }
}

#endif
