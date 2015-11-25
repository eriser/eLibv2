#ifndef MODADSR_H_
#define MODADSR_H_

#include <Envelope/BaseEnvelope.h>

namespace eLibV2
{
    namespace Envelope
    {
        /// @todo transfer test-case to googletest
        class ADSR : public BaseEnvelope
        {
        public:
            enum
            {
                ADSR_CONNECTION_INPUT = 0,
                ADSR_CONNECTION_NUM
            };

        public:
            ADSR(std::string name = "ADSR") :
                BaseName(name),
                BaseConnection(ADSR_CONNECTION_NUM)
            {
                Init();
            }
            virtual ~ADSR() {}

            virtual void Init(void);
            virtual void Reset(void);
            virtual double Process(void);

            virtual void setAttack(const double Attack)
            {
                m_dAttackTime_mSec = Attack;
                calcAttack();
            }

            virtual void setDecay(const double Decay)
            {
                m_dDecayTime_mSec = Decay;
                calcDecay();
            }

            virtual void setSustain(const double Sustain)
            {
                m_dSustainLevel = Sustain;
                calcDecay();
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

            virtual double getBiasedOutput(void) { return m_dBiasedOutput; }

        private:
            void calcAttack(void)
            {
                double dSamples = m_dSamplerate * (m_dAttackTime_mSec / 1000.0);
                m_dAttackCoeff = exp(-log((1.0 + m_dAttackTCO) / m_dAttackTCO) / dSamples);
                m_dAttackOffset = (1.0 + m_dAttackTCO) * (1.0 - m_dAttackCoeff);
            }

            void calcDecay(void)
            {
                double dSamples = m_dSamplerate * (m_dDecayTime_mSec / 1000.0);
                m_dDecayCoeff = exp(-log((1.0 + m_dDecayTCO) / m_dDecayTCO) / dSamples);
                m_dDecayOffset = (m_dSustainLevel + m_dDecayTCO) * (1.0 - m_dDecayCoeff);
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
            double m_dDecayCoeff, m_dDecayOffset, m_dDecayTCO;
            double m_dReleaseCoeff, m_dReleaseOffset, m_dReleaseTCO;

            double m_dAttackTime_mSec, m_dDecayTime_mSec, m_dReleaseTime_mSec, m_dShutdownTime_mSec;
            double m_dSustainLevel, m_dIncShutdown;
            double m_dBiasedOutput;
        };
    }
}

#endif /* MODENVELOPEDADSR_H_ */
