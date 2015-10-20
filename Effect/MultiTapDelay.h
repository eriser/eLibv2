#ifndef MODMULTITAPDELAY_H_
#define MODMULTITAPDELAY_H_

#include <Base/BaseEffect.h>
#include <Util/Logger.h>
#include <Util/Helper.h>

#include <math.h>

namespace eLibV2
{
    namespace Effect
    {
        class MultiTapDelay : public Base::BaseEffect
        {
        public:
            enum
            {
                DELAY_BUFFER_SIZE = 441000,
                DELAY_CONNECTION_BYPASS = 0,
                DELAY_CONNECTION_INPUT,
                DELAY_CONNECTION_LENGTH,
                DELAY_CONNECTION_NUM
            };

        public:
            MultiTapDelay(std::string name = "MultiTapDelay") :
                BaseName(name),
                BaseConnection(DELAY_CONNECTION_NUM)
            {
                m_pBuffer = new double[DELAY_BUFFER_SIZE];
                memset(m_pBuffer, 0, sizeof(double) * DELAY_BUFFER_SIZE);

                Init();
            }
            virtual ~MultiTapDelay() {}

            /* inherited */
            virtual void Init(void);
            virtual void Reset(void);
            virtual double Process(const double Input);
            virtual double processConnection(void);

        public:
            /* setter/getter */
            void setDelayLength(const unsigned int length);
            unsigned int getDelayLength(void) { m_iDelayLength; }

            void setMixLevel(const double MixLevel) { m_dMixLevel = MixLevel; }
            double getMixLevel(void) { return m_dMixLevel; }

            void setFeedbackLevel(const double FeedbackLevel) { m_dFeedbackLevel = FeedbackLevel; }
            double getFeedbackLevel(void) { return m_dFeedbackLevel; }

        public:
            void attachBypass(BaseConnection *connection) { inputConnections[DELAY_CONNECTION_BYPASS] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[DELAY_CONNECTION_INPUT] = connection; }
            void attachDelayLength(BaseConnection *connection) { inputConnections[DELAY_CONNECTION_LENGTH] = connection; }

        private:
            void adjustIndices();

        private:
            double *m_pBuffer;
            double m_dMixLevel;
            unsigned int m_iDelayLength;
            int m_iReadIndex[4], m_iWriteIndex;
            double m_dInternalFeedback;
            double m_dFeedbackLevel;
        };
    }
}

#endif