#ifndef MODCOMPRESSOR_H_
#define MODCOMPRESSOR_H_

#include <Base/BaseEffect.h>
#include <Effect/EnvelopeDetector.h>
#include <Util/Helper.h>

namespace eLibV2
{
    namespace Effect
    {
        /**
        this class implements a simple compressor
        */
        class Compressor : public Base::BaseEffect
        {
        public:
            enum
            {
                COMPRESSOR_CONNECTION_BYPASS = 0,
                COMPRESSOR_CONNECTION_INPUT,
                COMPRESSOR_CONNECTION_ATTACK,
                COMPRESSOR_CONNECTION_RELEASE,
                COMPRESSOR_CONNECTION_THRESHOLD,
                COMPRESSOR_CONNECTION_RATIO,
                COMPRESSOR_CONNECTION_KNEEWIDTH,
                COMPRESSOR_CONNECTION_INPUTGAIN,
                COMPRESSOR_CONNECTION_OUTPUTGAIN,
                COMPRESSOR_CONNECTION_NUM
            };

        public:
            Compressor(std::string name = "Compressor") :
                BaseName(name),
                BaseConnection(COMPRESSOR_CONNECTION_NUM)
            {
                m_pEnvelopeDetector = new EnvelopeDetector();
                Init();
            }

            virtual ~Compressor()
            {
                if (m_pEnvelopeDetector)
                    delete m_pEnvelopeDetector;
                m_pEnvelopeDetector = NULL;
            }

            /* inherited */
            virtual void Init(void);
            virtual void Reset(void) {}
            virtual double Process(const double Input);
            virtual double processConnection();

        public:
            /* getter/setter */
            void setAttack(const double Attack) { if (m_pEnvelopeDetector) m_pEnvelopeDetector->setAttack(Attack); }
            double getAttack(void)
            {
                double dAttack = 0.0;
                if (m_pEnvelopeDetector)
                    dAttack = m_pEnvelopeDetector->getAttack();
                return dAttack;
            }

            void setRelease(const double Release) { if (m_pEnvelopeDetector) m_pEnvelopeDetector->setRelease(Release); }
            double getRelease(void)
            {
                double dRelease = 0.0;
                if (m_pEnvelopeDetector)
                    dRelease = m_pEnvelopeDetector->getRelease();
                return dRelease;
            }

            void setDetectMode(const EnvelopeDetector::DetectMode DetectMode) { if (m_pEnvelopeDetector) m_pEnvelopeDetector->setDetectMode(DetectMode); }
            EnvelopeDetector::DetectMode getDetectMode(void)
            {
                EnvelopeDetector::DetectMode DetectMode;
                if (m_pEnvelopeDetector)
                    DetectMode = m_pEnvelopeDetector->getDetectMode();
                return DetectMode;
            }

            void setTCMode(const EnvelopeDetector::TCMode TCMode) { if (m_pEnvelopeDetector) m_pEnvelopeDetector->setTCMode(TCMode); }
            EnvelopeDetector::TCMode getTCMode(void)
            {
                EnvelopeDetector::TCMode TCMode;
                if (m_pEnvelopeDetector)
                    TCMode = m_pEnvelopeDetector->getTCMode();
                return TCMode;
            }

            void setThreshold(const double Threshold) { m_dThreshold = Threshold; }
            double getThreshold(void) { return m_dThreshold; }

            void setRatio(const double Ratio) { m_dRatio = Ratio; }
            double getRatio(void) { return m_dRatio; }

            void setKneeWidth(const double KneeWidth) { m_dKneeWidth = KneeWidth; }
            double getKneeWidth(void) { return m_dKneeWidth; }

            void setInputGain(const double InputGain) { m_dInputGain = InputGain; }
            double getInputGain(void) { return m_dInputGain; }

            void setOutputGain(const double OutputGain) { m_dOutputGain = OutputGain; }
            double getOutputGain(void) { return m_dOutputGain; }

        public:
            void attachBypass(BaseConnection *connection) { inputConnections[COMPRESSOR_CONNECTION_BYPASS] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[COMPRESSOR_CONNECTION_INPUT] = connection; }
            void attachAttack(BaseConnection *connection) { inputConnections[COMPRESSOR_CONNECTION_ATTACK] = connection; }
            void attachRelease(BaseConnection *connection) { inputConnections[COMPRESSOR_CONNECTION_RELEASE] = connection; }
            void attachThreshold(BaseConnection *connection) { inputConnections[COMPRESSOR_CONNECTION_THRESHOLD] = connection; }
            void attachRatio(BaseConnection *connection) { inputConnections[COMPRESSOR_CONNECTION_RATIO] = connection; }
            void attachKneeWidth(BaseConnection *connection) { inputConnections[COMPRESSOR_CONNECTION_KNEEWIDTH] = connection; }
            void attachInputGain(BaseConnection *connection) { inputConnections[COMPRESSOR_CONNECTION_INPUTGAIN] = connection; }
            void attachOutputGain(BaseConnection *connection) { inputConnections[COMPRESSOR_CONNECTION_OUTPUTGAIN] = connection; }

        private:
            EnvelopeDetector *m_pEnvelopeDetector;
            double m_dThreshold, m_dRatio;
            double m_dKneeWidth;
            double m_dInputGain, m_dOutputGain;
        };
    }
}

#endif
