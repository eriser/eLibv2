#ifndef MODVADIODELADDERFILTER_H_
#define MODVADIODELADDERFILTER_H_

#include <Filter/BaseVAFilter.h>
#include <Filter/VAOnePoleFilter.h>
#include <Util/Defines.h>

namespace eLibV2
{
    namespace Filter
    {
        class VADiodeLadderFilter : public BaseVAFilter
        {
        public:
            VADiodeLadderFilter(std::string name = "VADiodeLadderFilter") :
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
            virtual ~VADiodeLadderFilter() {}

            void Init(void)
            {
                m_bBypass = false;
                m_eType = TYPE_LPF;
                m_iOrder = 4;
                m_dCutoff = 22050.0;
                m_dQ = 0.5;

                m_dSG1 = m_dSG2 = m_dSG3 = m_dSG4 = 0.0;
                m_dK = 0.0;
                m_dGamma = 0.0;

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
                    (m_iOrder == 4) &&
                    (m_eType == TYPE_LPF)
                    )
                {
                    m_pLPF4->setFeedback(0.0);
                    m_pLPF3->setFeedback(m_pLPF4->getFeedbackOutput());
                    m_pLPF2->setFeedback(m_pLPF3->getFeedbackOutput());
                    m_pLPF1->setFeedback(m_pLPF2->getFeedbackOutput());

                    double dSigma = m_dSG1 * m_pLPF1->getFeedbackOutput() + m_dSG2 * m_pLPF2->getFeedbackOutput() +
                        m_dSG3 * m_pLPF3->getFeedbackOutput() + m_dSG4 * m_pLPF4->getFeedbackOutput();
                    double dU = (input - m_dK * dSigma) * (1.0 + m_dK * m_dGamma);

                    if (m_bNLPActive)
                        dU = tanh(m_dSaturation * dU);

                    double dLP1 = m_pLPF1->Process(dU);
                    double dLP2 = m_pLPF2->Process(dLP1);
                    double dLP3 = m_pLPF3->Process(dLP2);
                    double dLP4 = m_pLPF4->Process(dLP3);

                    dOutput = dLP4;
                }
                return dOutput;
            }

            virtual void setQ(const double Q)
            {
                m_dK = Q * 0.6;
                calcCoefficients();
            }

        protected:
            void calcCoefficients(void)
            {
                double wd = 2.0 * PI * m_dCutoff;
                double T = 1.0 / getSamplerate();
                double argtan = ModuleHelper::clamp(PI * m_dCutoff / getSamplerate(), 0.000001, PI_DIV_2);
                double g = tan(argtan);

                double G4 = 0.5 * g / (1.0 + g);
                double G3 = 0.5 * g / (1.0 + g - 0.5 * g * G4);
                double G2 = 0.5 * g / (1.0 + g - 0.5 * g * G3);
                double G1 = g / (1.0 + g - g * G2);

                m_dGamma = G4 * G3 * G2 * G1;

                m_dSG1 = G4 * G3 * G2;
                m_dSG2 = G4 * G3;
                m_dSG3 = G4;
                m_dSG4 = 1.0;

                double G = g / (1.0 + g);

                m_pLPF1->setCoefficients(G, 1.0 / (1.0 + g - g * G2), 1.0 + G1 * G2, g, G2);
                m_pLPF1->setA0(1.0);
                m_pLPF2->setCoefficients(G, 1.0 / (1.0 + g - 0.5 * g * G3), 1.0 + G2 * G3, 0.5 * g, G3);
                m_pLPF2->setA0(0.5);
                m_pLPF3->setCoefficients(G, 1.0 / (1.0 + g - 0.5 * g * G4), 1.0 + G3 * G4, 0.5 * g, G4);
                m_pLPF3->setA0(0.5);
                m_pLPF4->setCoefficients(G, 1.0 / (1.0 + g), 1.0, 0.0, 0.0);
                m_pLPF4->setA0(0.5);
            }

        private:
            double m_dK, m_dGamma;
            VAOnePoleFilter *m_pLPF1, *m_pLPF2, *m_pLPF3, *m_pLPF4;
            double m_dSG1, m_dSG2, m_dSG3, m_dSG4;
        };
    }
}

#endif
