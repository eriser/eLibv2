#ifndef MODCFBDELAY_H_
#define MODCFBDELAY_H_

#include <Base/BaseEffect.h>
#include <Effect/Delay.h>
#include <Util/Helper.h>

namespace eLibV2
{
    namespace Effect
    {
        class CrossedFeedbackDelay : public Base::BaseEffect
        {
        public:
            enum DelayMode
            {
                DELAY_MODE_UNDEF = 0,
                DELAY_MODE_NORMAL,
                DELAY_MODE_CROSSED
            };

            enum
            {
                CFBDELAY_CONNECTION_BYPASS = 0,
                CFBDELAY_CONNECTION_INPUT,
                CFBDELAY_CONNECTION_NUM
            };

        public:
            CrossedFeedbackDelay(std::string name = "CrossedFeedbackDelay") :
                BaseName(name)
            {
                m_pDelayLeft = new Delay();
                m_pDelayLeft->setFeedbackLevel(0.707);
                m_pDelayRight = new Delay();
                m_pDelayRight->setFeedbackLevel(0.707);
                Init();
            }

            virtual ~CrossedFeedbackDelay()
            {
                if (m_pDelayLeft)
                    delete m_pDelayLeft;
                m_pDelayLeft = NULL;
                if (m_pDelayRight)
                    delete m_pDelayRight;
                m_pDelayRight = NULL;
            }

            /* inherited */
            virtual void Init(void);
            virtual void Reset(void) {}
            virtual double Process(const double Input) { return 0.0; }
            virtual double processConnection(void) { return 0.0; }
            virtual void setSamplerate(const double Samplerate);

            void Process(const double input1, const double input2, double* output1, double* output2);

        public:
            /* getter/setter */
            void setDelayMode(const DelayMode delayMode);
            DelayMode getDelayMode(void) { return m_eDelayMode; }

            void setFeedbackLevel(const double FeedbackLevel)
            {
                if (m_pDelayLeft)
                    m_pDelayLeft->setFeedbackLevel(FeedbackLevel);
                if (m_pDelayRight)
                    m_pDelayRight->setFeedbackLevel(FeedbackLevel);
            }

            void setDelayLength(const unsigned int DelayLength)
            {
                if (m_pDelayLeft)
                    m_pDelayLeft->setDelayLength(DelayLength);
                if (m_pDelayRight)
                    m_pDelayRight->setDelayLength(DelayLength);
            }

            void setMixLevel(const double MixLevel)
            {
                if (m_pDelayLeft)
                    m_pDelayLeft->setMixLevel(MixLevel);
                if (m_pDelayRight)
                    m_pDelayRight->setMixLevel(MixLevel);
            }

        public:
            void attachBypass(BaseConnection *connection) { inputConnections[CFBDELAY_CONNECTION_BYPASS] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[CFBDELAY_CONNECTION_INPUT] = connection; }

        private:
            Delay *m_pDelayLeft, *m_pDelayRight;
            DelayMode m_eDelayMode;
        };
    }
}

#endif
