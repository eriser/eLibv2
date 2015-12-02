#ifndef MODVAMOOGLADDERFILTER_H_
#define MODVAMOOGLADDERFILTER_H_

#include <Filter/BaseVAFilter.h>
#include <Filter/VAOnePoleFilter.h>
#include <Util/Defines.h>

namespace eLibV2
{
    namespace Filter
    {
        class VAMoogLadderFilter : public BaseVAFilter
        {
        public:
            VAMoogLadderFilter(std::string name = "VAMoogLadderFilter") :
                BaseName(name),
                BaseConnection(FILTER_CONNECTION_NUM),
                BaseVAFilter()
            {
                m_pLPF1 = new VAOnePoleFilter();
                m_pLPF2 = new VAOnePoleFilter();
                m_pLPF3 = new VAOnePoleFilter();
                m_pLPF4 = new VAOnePoleFilter();

                Init();
            }
            virtual ~VAMoogLadderFilter() {}

            void Init(void)
            {
                m_bBypass = false;
                m_eType = TYPE_LPF;
                m_iOrder = 4;
                m_dCutoff = 22050.0;
                m_dQ = 0.5;

                m_dAlpha0 = 1.0;
                m_dA = m_dB = m_dC = m_dD = m_dE = 0.0;
                m_dK = 0.0;

                if (m_pLPF1)
                    m_pLPF1->setType(TYPE_LPF);
                if (m_pLPF2)
                    m_pLPF2->setType(TYPE_LPF);
                if (m_pLPF3)
                    m_pLPF3->setType(TYPE_LPF);
                if (m_pLPF4)
                    m_pLPF4->setType(TYPE_LPF);

                Reset();
                calcCoefficients();
            }

            virtual void Reset(void)
            {
                if (m_pLPF1)
                    m_pLPF1->Reset();
                if (m_pLPF2)
                    m_pLPF2->Reset();
                if (m_pLPF3)
                    m_pLPF3->Reset();
                if (m_pLPF4)
                    m_pLPF4->Reset();
            }

            virtual double Process(double input)
            {
                double dOutput = input;

                if (
                    (m_iOrder == 2) || (m_iOrder == 4) &&
                    ((m_eType == TYPE_LPF) || (m_eType == TYPE_HPF) || (m_eType == TYPE_BPF))
                    )
                {
                    double dSigma = m_pLPF1->getFeedbackOutput() + m_pLPF2->getFeedbackOutput() + m_pLPF3->getFeedbackOutput() + m_pLPF4->getFeedbackOutput();
                    double dU = (input - m_dK * dSigma) * m_dAlpha0;

                    if (m_bNLPActive)
                        dU = tanh(m_dSaturation * dU);

                    double dLP1 = m_pLPF1->Process(dU);
                    double dLP2 = m_pLPF2->Process(dLP1);
                    double dLP3 = m_pLPF3->Process(dLP2);
                    double dLP4 = m_pLPF4->Process(dLP3);

                    dOutput = m_dA * dU + m_dB * dLP1 + m_dC * dLP2 + m_dD * dLP3 + m_dE * dLP4;
                }
                return dOutput;
            }

            virtual void setQ(const double Q)
            {
                m_dK = Q * 4.0;
                calcCoefficients();
            }

        protected:
            void calcCoefficients(void)
            {
                double wd = 2.0 * PI * m_dCutoff;
                double T = 1.0 / getSamplerate();
                double argtan = ModuleHelper::clamp(PI * m_dCutoff / getSamplerate(), 0.000001, PI_DIV_2);
                double g = tan(argtan);

                double G = g / (1.0 + g);
                m_dGamma = G * G * G * G;
                m_dAlpha0 = 1.0 / (1.0 + m_dK * m_dGamma);

                m_pLPF1->setCoefficients(G, G * G * G / (1.0 + g), 1.0, 0.0, 0.0);
                m_pLPF2->setCoefficients(G, G* G / (1.0 + g), 1.0, 0.0, 0.0);
                m_pLPF3->setCoefficients(G, G / (1.0 + g), 1.0, 0.0, 0.0);
                m_pLPF4->setCoefficients(G, 1.0 / (1.0 + g), 1.0, 0.0, 0.0);

                m_dA = 0.0; m_dB = 0.0; m_dC = 0.0; m_dD = 0.0; m_dE = 0.0;
                if (m_iOrder == 2)
                {
                    switch (m_eType)
                    {
                        case TYPE_LPF:
                            m_dA = 0.0; m_dB = 0.0; m_dC = 1.0; m_dD = 0.0; m_dE = 0.0;
                            break;

                        case TYPE_HPF:
                            m_dA = 1.0; m_dB = -2.0; m_dC = 1.0; m_dD = 0.0; m_dE = 0.0;
                            break;

                        case TYPE_BPF:
                            m_dA = 0.0; m_dB = 2.0; m_dC = -2.0; m_dD = 0.0; m_dE = 0.0;
                            break;
                    }
                }
                else if (m_iOrder == 4)
                {
                    switch (m_eType)
                    {
                        case TYPE_LPF:
                            m_dA = 0.0; m_dB = 0.0; m_dC = 0.0; m_dD = 0.0; m_dE = 1.0;
                            break;

                        case TYPE_HPF:
                            m_dA = 1.0; m_dB = -4.0; m_dC = 6.0; m_dD = -4.0; m_dE = 1.0;
                            break;

                        case TYPE_BPF:
                            m_dA = 0.0; m_dB = 0.0; m_dC = 4.0; m_dD = -8.0; m_dE = 4.0;
                            break;
                    }
                }
            }

        private:
            double m_dK, m_dAlpha0, m_dGamma;
            VAOnePoleFilter *m_pLPF1, *m_pLPF2, *m_pLPF3, *m_pLPF4;
            double m_dA, m_dB, m_dC, m_dD, m_dE;
        };
    }
}

#endif
