#ifndef MODCOMBFILTER_H_
#define MODCOMBFILTER_H_

#include <Base/BaseFilter.h>
#include <Effect/Delay.h>
#include <Util/Defines.h>

namespace eLibV2
{
    namespace Filter
    {
        /**
        Implements a second-order Butterworth Bandpass-Filter
        */
        class CombFilter : public Base::BaseFilter
        {
        public:
            CombFilter(std::string name = "CombFilter") :
                BaseName(name),
                BaseFilter(),
                BaseConnection(FILTER_CONNECTION_NUM)
            {
                Init();
            }
            virtual ~CombFilter()
            {
                if (m_pDelay)
                    delete m_pDelay;
                m_pDelay = NULL;
            }

            void Init(void)
            {
                m_bBypass = false;
                m_dGain = 0.0;
                m_pDelay = new Effect::Delay();
                m_pDelay->setMixLevel(1.0);
            }

        public:
            virtual double Process(const double Input)
            {
                double dOutput = 0.0;

                if (m_pDelay)
                {
                    m_pDelay->setFeedbackLevel(m_dGain);
                    dOutput = m_pDelay->Process(Input);
                }

                return dOutput;
            }

            void calcCoefficients(void) {}

        public:
            /* setter/getter */
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
            Effect::Delay *m_pDelay;
        };
    }
}

#endif
