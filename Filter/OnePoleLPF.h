#ifndef MODONEPOLELPF_H_
#define MODONEPOLELPF_H_

#include <Filter/BaseFilter.h>
#include <Util/Defines.h>

namespace eLibV2
{
    namespace Filter
    {
        /**
        this class implements a one-pole Lowpass-Filter
        */
        class OnePoleLPF : public BaseFilter
        {
        public:
            OnePoleLPF(std::string name = "OnePoleLPF") :
                BaseName(name),
                BaseConnection(FILTER_CONNECTION_NUM),
                BaseFilter()
            {
                Init();
            }

            void Init(void)
            {
                m_bBypass = false;
                m_dGain = 0.0;
            }

        protected:
            void calcCoefficients(void)
            {
                // only a0 and b1 are used and set accordigly to use the enhanced biquad
                double a0 = 1.0 - m_dGain;
                double b1 = -m_dGain;

                if (m_pInternalBiquad)
                {
                    m_pInternalBiquad->setCoefficients(a0, 0.0, 0.0, b1, 0.0);
                    m_pInternalBiquad->setWetDryLevel(1.0, 0.0);
                }
            }
        };
    }
}

#endif
