#ifndef MODENVELOPEDETECTOR_H_
#define MODENVELOPEDETECTOR_H_

#include <Base/BaseEffect.h>
#include <Util/Logger.h>
#include <Util/Helper.h>

namespace eLibV2
{
    namespace Effect
    {
        static const double DIGITAL_TC = -2.0; // log(1%)
        static const double ANALOG_TC = -0.43533393574791066201247090699309; // (log(36.7%)

        class EnvelopeDetector : public Base::BaseEffect
        {
        public:
            enum
            {
                ENVELOPEDETECTOR_CONNECTION_INPUT = 0,
                ENVELOPEDETECTOR_CONNECTION_ATTACK,
                ENVELOPEDETECTOR_CONNECTION_RELEASE,
                ENVELOPEDETECTOR_CONNECTION_NUM
            };

            enum TCMode
            {
                ENVELOPEDETECTOR_TCMODE_UNDEF = 0,
                ENVELOPEDETECTOR_TCMODE_ANALOG,
                ENVELOPEDETECTOR_TCMODE_DIGITAL
            };

            enum DetectMode
            {
                ENVELOPEDETECTOR_DETECTMODE_UNDEF = 0,
                ENVELOPEDETECTOR_DETECTMODE_PEAK,
                ENVELOPEDETECTOR_DETECTMODE_MS,
                ENVELOPEDETECTOR_DETECTMODE_RMS
            };

        public:
            EnvelopeDetector(std::string name = "EnvelopeDetector") :
                BaseName(name),
                BaseConnection(ENVELOPEDETECTOR_CONNECTION_NUM)
            {
                Init();
            }
            virtual ~EnvelopeDetector() {}

            /* inherited */
            virtual void Init(void);
            virtual void Reset(void);
            virtual double Process(const double Input);
            virtual double processConnection(void);
            virtual void setSamplerate(const double Samplerate);

        public:
            /* getter/setter */
            double getAttack() { return m_dAttack; }
            double getRelease() { return m_dRelease; }
            TCMode getTCMode() { return m_eTCMode; }
            DetectMode getDetectMode() { return m_eDetectMode; }

            void setAttack(const double Attack);
            void setRelease(const double Release);
            void setTCMode(const TCMode TCMode);
            void setDetectMode(const DetectMode DetectMode);

        public:
            void attachInput(BaseConnection *connection) { inputConnections[ENVELOPEDETECTOR_CONNECTION_INPUT] = connection; }
            void attachAttack(BaseConnection *connection) { inputConnections[ENVELOPEDETECTOR_CONNECTION_ATTACK] = connection; }
            void attachRelease(BaseConnection *connection) { inputConnections[ENVELOPEDETECTOR_CONNECTION_RELEASE] = connection; }

        private:
            bool bBypass;
            double m_dAttack, m_dAttackScaled;
            double m_dRelease, m_dReleaseScaled;
            TCMode m_eTCMode;
            DetectMode m_eDetectMode;
            double m_dCurrentLevel;
        };
    }
}

#endif
