#ifndef MODTREMOLOPANNER_H_
#define MODTREMOLOPANNER_H_

#include <Effect/BaseEffect.h>
#include <Connection/InputConnection.h>
#include <Generator/BaseLFO.h>
#include <Util/Helper.h>

namespace eLibV2
{
    namespace Effect
    {
        class TremoloPanner : public BaseEffect
        {
        public:
            enum
            {
                TREMOLOPANNER_CONNECTION_BYPASS = 0,
                TREMOLOPANNER_CONNECTION_INPUT1,
                TREMOLOPANNER_CONNECTION_INPUT2,
                TREMOLOPANNER_CONNECTION_LFOFREQ,
                TREMOLOPANNER_CONNECTION_NUM
            };

        public:
            enum TremoloPannerMode
            {
                TREMOLOPANNER_MODE_UNDEF = 0,
                TREMOLOPANNER_MODE_TREMOLO,
                TREMOLOPANNER_MODE_PANNER
            };

        public:
            TremoloPanner(std::string name = "TremoloPanner") :
                BaseName(name),
                BaseConnection(TREMOLOPANNER_CONNECTION_NUM)
            {
                m_pLFO = new Generator::BaseLFO();
                m_pOutputConnection = new Connection::InputConnection();

                Init();
            }
            virtual ~TremoloPanner()
            {
                if (m_pLFO)
                    delete m_pLFO;
                m_pLFO = NULL;

                if (m_pOutputConnection)
                    delete m_pOutputConnection;
                m_pOutputConnection = NULL;
            }

            /* inherited */
            virtual void Init(void);
            virtual void Reset(void) {}
            virtual double Process(const double Input) { return 0.0; }
            virtual double processConnection(void);
            virtual void setSamplerate(const double Samplerate);

        public:
            /* getter/setter */
            void setMode(const TremoloPannerMode Mode) { m_eMode = Mode; }
            TremoloPannerMode getMode(void) const { return m_eMode; }

            void setModulationDepth(const double ModulationDepth) { m_dModulationDepth = ModulationDepth; }
            double getModulationDepth(void) const { return m_dModulationDepth; }

            void setLFOFreq(const double LFOFreq)
            {
                if (m_pLFO)
                    m_pLFO->setFreq(LFOFreq);
            }

            double getLFOFreq(void) const
            {
                double dFreq = 0.0;
                if (m_pLFO)
                    dFreq = m_pLFO->getFreq();
                return dFreq;
            }

            // own process-method for 2 inputs
            virtual double Process(const double Input1, const double Input2);

            BaseConnection* getSecondOutput(void) const { return m_pOutputConnection; }

        public:
            void attachBypass(BaseConnection *connection) { inputConnections[TREMOLOPANNER_CONNECTION_BYPASS] = connection; }
            void attachInput1(BaseConnection *connection) { inputConnections[TREMOLOPANNER_CONNECTION_INPUT1] = connection; }
            void attachInput2(BaseConnection *connection) { inputConnections[TREMOLOPANNER_CONNECTION_INPUT2] = connection; }
            void attachLFORate(BaseConnection *connection) { inputConnections[TREMOLOPANNER_CONNECTION_LFOFREQ] = connection; }

        private:
            Connection::InputConnection *m_pOutputConnection;
            Generator::BaseLFO *m_pLFO;
            TremoloPannerMode m_eMode;
            double m_dModulationDepth;
        };
    }
}

#endif
