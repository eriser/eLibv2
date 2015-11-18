#ifndef MODDELAYAPF_H_
#define MODDELAYAPF_H_

#include <Base/BaseFilter.h>
#include <Effect/Delay.h>
#include <Util/Defines.h>

namespace eLibV2
{
    namespace Filter
    {
        /**
        this class implements a delayed Allpass-Filter
        */
        class DelayAPF : public Base::BaseFilter
        {
        public:
            DelayAPF(std::string name = "DelayAPF") :
                BaseName(name),
                BaseConnection(FILTER_CONNECTION_NUM),
                BaseFilter()
            {
                Init();
            }
            virtual ~DelayAPF()
            {
                if (m_pDelay)
                    delete m_pDelay;
                m_pDelay = NULL;
            }

            void Init(void)
            {
                m_bBypass = false;
                m_dGain = 0.0;
                m_dLastDelayOutput = 0.0;

                m_pDelay = new Effect::Delay();
                m_pDelay->setMixLevel(1.0);
            }

        public:
            virtual double Process(const double Input)
            {
                double dOutput = 0.0;

                // delay = w(n) = x(n) + gw(n-D)
                // y(n) = -gw(n) + x(n) * w(n-D)
                if (m_pDelay)
                {
                    double dDelayInput = Input + m_dLastDelayOutput;
                    m_dLastDelayOutput = m_pDelay->Process(dDelayInput);
                    dOutput = dDelayInput * -m_dGain + m_dLastDelayOutput;
                    m_dLastDelayOutput *= m_dGain;
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

        private:
            double m_dLastDelayOutput;
            Effect::Delay *m_pDelay;
        };
    }
}

#endif
