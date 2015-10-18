#ifndef MODDELAY_H_
#define MODDELAY_H_

#include <Base/BaseEffect.h>
#include <Util/Logger.h>
#include <Util/Helper.h>

#include <math.h>

namespace eLibV2
{
    namespace Effect
    {
        class Delay : public Base::BaseEffect
        {
        public:
            enum FeedbackMode
            {
                FEEDBACK_MODE_UNDEF = 0,
                FEEDBACK_MODE_INTERNAL,
                FEEDBACK_MODE_EXTERNAL
            };

            enum
            {
                DELAY_BUFFER_SIZE = 441000,
                DELAY_CONNECTION_BYPASS = 0,
                DELAY_CONNECTION_INPUT,
                DELAY_CONNECTION_FEEDBACK,
                DELAY_CONNECTION_LENGTH,
                DELAY_CONNECTION_NUM
            };

        public:
            Delay(std::string name = "Delay") :
                BaseName(name),
                BaseConnection(DELAY_CONNECTION_NUM)
            {
                m_pBuffer = new double[DELAY_BUFFER_SIZE];
                memset(m_pBuffer, 0, sizeof(double) * DELAY_BUFFER_SIZE);

                Init();
            }
            virtual ~Delay() {}

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

            void setFeedbackMode(const FeedbackMode Mode) { m_eFeedbackMode = Mode; }
            FeedbackMode getFeedbackMode(void) { return m_eFeedbackMode; }

            void setFeedbackLevel(const double FeedbackLevel) { m_dFeedbackLevel = FeedbackLevel; }
            double getFeedbackLevel(void) { return m_dFeedbackLevel; }

            void setFeedback(const double Feedback) { m_dExternalFeedback = Feedback; }
            double getFeedback(void) { return m_dInternalFeedback; }

        public:
            void attachBypass(BaseConnection *connection) { inputConnections[DELAY_CONNECTION_BYPASS] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[DELAY_CONNECTION_INPUT] = connection; }
            void attachFeedback(BaseConnection *connection) { inputConnections[DELAY_CONNECTION_FEEDBACK] = connection; }
            void attachDelayLength(BaseConnection *connection) { inputConnections[DELAY_CONNECTION_LENGTH] = connection; }

        private:
            void adjustIndices();

        private:
            double *m_pBuffer;
            double m_dMixLevel;
            unsigned int m_iDelayLength;
            int m_iReadIndex, m_iWriteIndex;
            double m_dInternalFeedback, m_dExternalFeedback;
            double m_dFeedbackLevel;
            FeedbackMode m_eFeedbackMode;
        };
    }
}

#endif
