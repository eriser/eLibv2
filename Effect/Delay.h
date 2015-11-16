#ifndef MODDELAY_H_
#define MODDELAY_H_

#include <Base/BaseEffect.h>
#include <Util/Logger.h>
#include <Util/Helper.h>

namespace eLibV2
{
    namespace Effect
    {
        class Delay : public Base::BaseEffect
        {
        public:
            static const UInt8 NUM_SYNC_FACTORS = 9;

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

                for (UInt8 syncIndex = 0; syncIndex < NUM_SYNC_FACTORS; ++syncIndex)
                    m_dSyncFactors[syncIndex] = pow(2.0, (syncIndex - 4));

                Init();
            }
            virtual ~Delay()
            {
                if (m_pBuffer)
                    delete[] m_pBuffer;
                m_pBuffer = NULL;
            }

            /* inherited */
            virtual void Init(void);
            virtual void Reset(void);
            virtual double Process(const double Input);
            virtual double processConnection(void);

        public:
            /* setter/getter */

            /**
            set length of delay-line in samples
            default-value: 1000
            @param length length of delay-line in samples
            */
            void setDelayLength(const double length);

            /**
            get length of delay-line
            @return length of delay-line in samples
            */
            UInt16 getDelayLength(void) { return m_iDelayLengthInSamples; }

            /**
            set mix-level of wet signal
            default-value: 0.5
            @param MixLevel the factor for the wet signal mixed to the output (1.0 - mixlevel) for dry signal
            */
            void setMixLevel(const double MixLevel) { m_dMixLevel = MixLevel; }

            /**
            get mix-level of wet signal
            @return mix-level of wet signal
            */
            double getMixLevel(void) { return m_dMixLevel; }

            /**
            set feedback-mode used
            default-value: INTERNAL
            @param Mode feedback-mode to set @see FeedbackMode
            */
            void setFeedbackMode(const FeedbackMode Mode) { m_eFeedbackMode = Mode; }

            /**
            get currently set feedback-mode
            @return the feedback-mode which is currently set
            */
            FeedbackMode getFeedbackMode(void) { return m_eFeedbackMode; }

            /**
            set the level the feedback is mixed with the input-signal
            default-value: 0.0
            @param FeedbackLevel level
            */
            void setFeedbackLevel(const double FeedbackLevel) { m_dFeedbackLevel = FeedbackLevel; }

            /**
            get currently set feedback-level for the feedback
            @return feedback-level
            */
            double getFeedbackLevel(void) { return m_dFeedbackLevel; }

            /**
            set externally feedback value, only works when feedback-mode set to external
            @param Feedback value of external feedback
            */
            void setFeedback(const double Feedback) { m_dExternalFeedback = Feedback; }

            /**
            get internal feedback which was the last output of the delay
            @return internal feedback
            */
            double getFeedback(void) { return m_dInternalFeedback; }

            void setSyncTempo(const bool SyncTempo) { m_bSyncTempo = SyncTempo; }
            bool getSyncTempo(void) { return m_bSyncTempo; }

        public:
            void attachBypass(BaseConnection *connection) { inputConnections[DELAY_CONNECTION_BYPASS] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[DELAY_CONNECTION_INPUT] = connection; }
            void attachFeedback(BaseConnection *connection) { inputConnections[DELAY_CONNECTION_FEEDBACK] = connection; }
            void attachDelayLength(BaseConnection *connection) { inputConnections[DELAY_CONNECTION_LENGTH] = connection; }

        private:
            void adjustIndices();

        private:
            double *m_pBuffer;
            double m_dMixLevel, m_dDelayLength;
            UInt16 m_iDelayLengthInSamples;
            SInt16 m_iReadIndex, m_iWriteIndex;
            double m_dInternalFeedback, m_dExternalFeedback;
            double m_dFeedbackLevel;
            FeedbackMode m_eFeedbackMode;
            bool m_bSyncTempo;
            double m_dSyncFactors[NUM_SYNC_FACTORS];
        };
    }
}

#endif
