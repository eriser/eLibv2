#ifndef MODVAK35FILTER_H_
#define MODVAK35FILTER_H_

#include <Filter/BaseVAFilter.h>
#include <Filter/VAOnePoleFilter.h>
#include <Util/Defines.h>

namespace eLibV2
{
    namespace Filter
    {
        class VAK36Filter : public BaseVAFilter
        {
        public:
            VAK36Filter(std::string name = "VAK36Filter") :
                BaseName(name),
                BaseConnection(FILTER_CONNECTION_NUM),
                BaseVAFilter()
            {
                m_pLPF1 = new VAOnePoleFilter();
                m_pLPF1->setType(TYPE_LPF);
                m_pLPF2 = new VAOnePoleFilter();
                m_pLPF2->setType(TYPE_LPF);
                m_pHPF1 = new VAOnePoleFilter();
                m_pHPF1->setType(TYPE_HPF);
                m_pHPF2 = new VAOnePoleFilter();
                m_pHPF2->setType(TYPE_HPF);

                Init();
            }

            void Init(void)
            {
                m_bBypass = false;
                m_eType = TYPE_LPF;
                m_iOrder = 2;
                m_dCutoff = 22050.0;
                m_dQ = 0.5;

                m_dAlpha0 = 1.0;
                m_dK = 0.01;

                Reset();

                calcCoefficients();
            }

            virtual void Reset(void)
            {
                if (m_pLPF1)
                    m_pLPF1->Reset();
                if (m_pLPF2)
                    m_pLPF2->Reset();
                if (m_pHPF1)
                    m_pHPF1->Reset();
                if (m_pHPF2)
                    m_pHPF2->Reset();
            }

            virtual double Process(double input)
            {
                double dOutput = input;

                if (
                    (m_iOrder == 2) &&
                    ((m_eType == TYPE_LPF) || (m_eType == TYPE_HPF))
                    )
                {
                    if (m_eType == TYPE_LPF)
                    {
                        double y1 = m_pLPF1->Process(input);
                        double S35 = m_pHPF1->getFeedbackOutput() + m_pLPF2->getFeedbackOutput();

                        double u = m_dAlpha0 * (y1 + S35);

                        if (m_bNLPActive)
                            u = tanh(m_dSaturation * u);

                        dOutput = m_dK * m_pLPF2->Process(u);

                        m_pHPF1->Process(dOutput);
                    }
                    else
                    {
                        double y1 = m_pHPF1->Process(input);
                        double S35 = m_pHPF2->getFeedbackOutput() + m_pLPF1->getFeedbackOutput();

                        double u = m_dAlpha0 * (y1 + S35);
                        dOutput = m_dK * u;

                        if (m_bNLPActive)
                            dOutput = tanh(m_dSaturation * dOutput);

                        m_pLPF1->Process(m_pHPF2->Process(dOutput));
                    }

                    if (m_dK > 0.0)
                        dOutput *= 1.0 / m_dK;
                }
                return dOutput;
            }

        protected:
            void calcCoefficients(void)
            {
                double wd = 2.0 * PI * m_dCutoff;
                double T = 1.0 / getSamplerate();
                double argtan = ModuleHelper::clamp(PI * m_dCutoff / getSamplerate(), 0.000001, PI_DIV_2);
                double g = tan(argtan);

                double G = g / (1.0 + g);

                m_dAlpha0 = 1.0 / (1.0 - m_dK * G + m_dK * G * G);

                double R = 1.0 / (2.0 * m_dQ);

                m_dAlpha0 = 1.0 / (1.0 + 2.0 * R * g + g * g);

                if (m_eType == TYPE_LPF)
                {
                    m_pLPF2->setCoefficients(G, (m_dK - m_dK * G) / (1.0 + g), 1.0, 0.0, 0.0);
                    m_pHPF1->setCoefficients(G, -1.0 / (1.0 + g), 1.0, 0.0, 0.0);
                }
                else
                {
                    m_pHPF2->setCoefficients(G, -1.0 * G / (1.0 + g), 1.0, 0.0, 0.0);
                    m_pLPF1->setCoefficients(G, 1.0 / (1.0 + g), 1.0, 0.0, 0.0);
                }
            }

        private:
            double m_dAlpha0, m_dK;
            VAOnePoleFilter *m_pLPF1, *m_pLPF2, *m_pHPF1, *m_pHPF2;
        };
    }
}

#endif
