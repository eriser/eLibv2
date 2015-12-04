#ifndef MODAR_H_
#define MODAR_H_

#include <Envelope/BaseEnvelope.h>

namespace eLibV2
{
    namespace Envelope
    {
        /// @todo transfer test-case to googletest
        class AR : public BaseEnvelope
        {
        public:
            enum
            {
                ADSR_CONNECTION_INPUT = 0,
                ADSR_CONNECTION_NUM
            };

        public:
            AR(std::string name = "AR") :
                BaseName(name),
                BaseConnection(ADSR_CONNECTION_NUM)
            {
                Init();
            }
            virtual ~AR() {}

            virtual void Init(void);
            virtual void Reset(void);
            virtual double Process(void);

            virtual void setAttack(const double Attack)
            {
                m_dAttackTime_mSec = Attack;
                calcAttack();
            }

            virtual void setRelease(const double Release)
            {
                m_dReleaseTime_mSec = Release;
                calcRelease();
            }

            virtual void setMode(const EnvelopeMode Mode);

            virtual void start(void);
            virtual void stop(void);
            virtual void shutdown(void);

            virtual double processConnection(void) { return Process(); }

        private:
            void calcAttack(void)
            {
                double dSamples = m_dSamplerate * (m_dAttackTime_mSec / 1000.0);
                m_dAttackCoeff = exp(-log((1.0 + m_dAttackTCO) / m_dAttackTCO) / dSamples);
                m_dAttackOffset = (1.0 + m_dAttackTCO) * (1.0 - m_dAttackCoeff);
            }

            void calcRelease(void)
            {
                double dSamples = m_dSamplerate * (m_dReleaseTime_mSec / 1000.0);
                m_dReleaseCoeff = exp(-log((1.0 + m_dReleaseTCO) / m_dReleaseTCO) / dSamples);
                m_dReleaseOffset = -m_dReleaseTCO * (1.0 - m_dReleaseCoeff);
            }

        public:
            void attachInput(BaseConnection *connection) { inputConnections[ADSR_CONNECTION_INPUT] = connection; }

        protected:
            bool m_bResetToZero, m_bLegatoMode;

            double m_dAttackCoeff, m_dAttackOffset, m_dAttackTCO;
            double m_dReleaseCoeff, m_dReleaseOffset, m_dReleaseTCO;

            double m_dAttackTime_mSec, m_dReleaseTime_mSec, m_dShutdownTime_mSec;
            double m_dIncShutdown;
        };
    }
}

#endif /* MODENVELOPEDADSR_H_ */
