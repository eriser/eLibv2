#ifndef MODCOMBFILTERLPF_H_
#define MODCOMBFILTERLPF_H_

#include <Base/BaseFilter.h>
#include <Filter/OnePoleLPF.h>
#include <Effect/Delay.h>
#include <Util/Defines.h>
#include <cmath>

namespace eLibV2
{
    namespace Filter
    {
        /**
        Implements a second-order Butterworth Bandpass-Filter
        */
        class CombFilterLPF : public Base::BaseFilter
        {
        public:
            CombFilterLPF(std::string name = "CombFilterLPF") :
                BaseName(name),
                BaseFilter(),
                BaseConnection(FILTER_CONNECTION_NUM)
            {
                Init();
            }
            virtual ~CombFilterLPF()
            {
                if (m_pDelay)
                    delete m_pDelay;
                m_pDelay = NULL;

                if (m_pLPF)
                    delete m_pLPF;
                m_pLPF = NULL;
            }

            void Init(void)
            {
                m_bBypass = false;
                m_dFeedbackGain = 0.0;
                m_dLPFGain = 0.0;

                m_pDelay = new Effect::Delay();
                m_pDelay->setMixLevel(1.0);
                m_pDelay->setFeedbackMode(Effect::Delay::FEEDBACK_MODE_EXTERNAL);
                m_pDelay->setFeedback(0.0);

                m_pLPF = new OnePoleLPF();
                m_pLPF->setGain(0.0);
            }

        public:
            virtual double Process(const double Input)
            {
                double dOutput = 0.0;

                if (m_pDelay && m_pLPF)
                {
                    double dLPFInput = m_pDelay->Process(Input);
                    m_pLPF->setGain(m_dLPFGain);
                    double dDelayFeedback = m_pLPF->Process(dLPFInput);
                    m_pDelay->setFeedbackLevel(m_dFeedbackGain);
                    m_pDelay->setFeedback(dDelayFeedback);
                    dOutput = dLPFInput;
                }

                return dOutput;
            }

            void calcCoefficients(void) {}

        public:
            /* setter/getter */
            void setFeedbackGain(const double FeedbackGain) { m_dFeedbackGain = FeedbackGain; }
            double getFeedbackGain(void) { return m_dFeedbackGain; }

            void setLPFGain(const double LPFGain) { m_dLPFGain = LPFGain; }
            double getLPFGain(void) { return m_dLPFGain; }

            void setDelayLength(const double length)
            {
                if (m_pDelay)
                    m_pDelay->setDelayLength(length);
            }

            void setGainRT60(const double Gain)
            {
                double dExponent = -3.0 * m_pDelay->getDelayLength() * (1.0 / getSamplerate());
                double dRT = Gain / 1000.0; // RT is in mSec!

                setGain(pow(10.0, dExponent / dRT));
            }

        private:
            double m_dFeedbackGain, m_dLPFGain;
            Effect::Delay *m_pDelay;
            OnePoleLPF *m_pLPF;
        };
    }
}

#endif
