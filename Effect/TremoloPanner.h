#ifndef MODTREMOLOPANNER_H_
#define MODTREMOLOPANNER_H_

#include <Base/BaseEffect.h>
#include <Generator/BaseLFO.h>
#include <Util/Helper.h>

namespace eLibV2
{
    namespace Effect
    {
        class TremoloPanner : public Base::BaseEffect
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

                Init();
            }
            virtual ~TremoloPanner()
            {
                if (m_pLFO)
                    delete m_pLFO;
                m_pLFO = NULL;
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
            TremoloPannerMode getMode(void) { return m_eMode; }

            void setModulationDepth(const double ModulationDepth) { m_dModulationDepth = ModulationDepth; }
            double getModulationDepth(void) { return m_dModulationDepth; }

            void setLFOFreq(const double LFOFreq)
            {
                if (m_pLFO)
                    m_pLFO->setFreq(LFOFreq);
            }

            double getLFOFreq(void)
            {
                double dFreq = 0.0;
                if (m_pLFO)
                    dFreq = m_pLFO->getFreq();
                return dFreq;
            }

            double getSecondOutput(void) { return m_dSecondOutput; }

            // own process-method for 2 inputs
            virtual double Process(const double Input1, const double Input2);

        public:
            void attachBypass(BaseConnection *connection) { inputConnections[TREMOLOPANNER_CONNECTION_BYPASS] = connection; }
            void attachInput1(BaseConnection *connection) { inputConnections[TREMOLOPANNER_CONNECTION_INPUT1] = connection; }
            void attachInput2(BaseConnection *connection) { inputConnections[TREMOLOPANNER_CONNECTION_INPUT2] = connection; }
            void attachLFORate(BaseConnection *connection) { inputConnections[TREMOLOPANNER_CONNECTION_LFOFREQ] = connection; }

        private:
            Generator::BaseLFO *m_pLFO;
            TremoloPannerMode m_eMode;
            double m_dModulationDepth;
            double m_dSecondOutput;
        };
    }
}

#endif