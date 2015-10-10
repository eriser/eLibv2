#ifndef MODSIMPLECOMP_H_
#define MODSIMPLECOMP_H_

#include <Base/BaseEffect.h>
#include <Util/Helper.h>

#include <math.h>

namespace eLibV2
{
    namespace Effect
    {
        class SimpleComp : public Base::BaseEffect
        {
        public:
            enum
            {
                SIMPLECOMP_CONNECTION_BYPASS = 0,
                SIMPLECOMP_CONNECTION_INPUT,
                SIMPLECOMP_CONNECTION_ATTACK,
                SIMPLECOMP_CONNECTION_RELEASE,
                SIMPLECOMP_CONNECTION_THRESHOLD,
                SIMPLECOMP_CONNECTION_RATIO,
                SIMPLECOMP_CONNECTION_GAIN,
                SIMPLECOMP_CONNECTION_NUM
            };

        public:
            SimpleComp(std::string name = "SimpleComp") :
                Base::BaseName(name),
                BaseConnection(SIMPLECOMP_CONNECTION_NUM)
            {
                Init();
            }
            virtual ~SimpleComp() {}

        public:
            /* inherited */
            virtual void Init(void);
            virtual void Reset(void);
            virtual double Process(const double Input);
            virtual double processConnection();

        public:
            /* getter/setter */
            void setThreshold(const double Threshold);
            void setRatio(const double Ratio);
            void setAttack(const double Attack);
            void setRelease(const double Release);
            void setOutput(const double Output);

            double getThreshold(void) { return m_dThreshold; }
            double getRatio(void) { return m_dRatio; }
            double getAttack(void) { return m_dAttack; }
            double getRelease(void) { return m_dRelease; }
            double getOutput(void) { return m_dOutput; }

        public:
            void attachBypass(BaseConnection *connection) { inputConnections[SIMPLECOMP_CONNECTION_BYPASS] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[SIMPLECOMP_CONNECTION_INPUT] = connection; }
            void attachAttack(BaseConnection *connection) { inputConnections[SIMPLECOMP_CONNECTION_ATTACK] = connection; }
            void attachRelease(BaseConnection *connection) { inputConnections[SIMPLECOMP_CONNECTION_RELEASE] = connection; }
            void attachThreshold(BaseConnection *connection) { inputConnections[SIMPLECOMP_CONNECTION_THRESHOLD] = connection; }
            void attachRatio(BaseConnection *connection) { inputConnections[SIMPLECOMP_CONNECTION_RATIO] = connection; }
            void attachGain(BaseConnection *connection) { inputConnections[SIMPLECOMP_CONNECTION_GAIN] = connection; }

        private:
            double m_dThreshold;
            double m_dAttack, m_dRelease, m_dEnvDecay;
            double m_dOutput;
            double m_dTransferA, m_dTransferB;
            double m_dEnv, m_dGain, m_dRatio;
        };
    }
}

#endif
