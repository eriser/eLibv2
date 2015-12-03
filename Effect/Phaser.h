#ifndef MODPHASER_H_
#define MODPHASER_H_

#include <Effect/BaseEffect.h>
#include <Util/Helper.h>

#include <Connection/ScalerConnection.h>
#include <Connection/InputConnection.h>
#include <Filter/SwitchableAPF.h>
#include <Generator/BaseLFO.h>

namespace eLibV2
{
    namespace Effect
    {
        /**
        this module implements a simple phaser consisting of 6 first-order APFs
        */
        class Phaser : public BaseEffect
        {
        public:
            enum
            {
                PHASER_FILTERSTAGES_NUM = 6,
                PHASER_CONNECTION_BYPASS = 0,
                PHASER_CONNECTION_INPUT,
                PHASER_CONNECTION_LFOFREQ,
                PHASER_CONNECTION_NUM
            };

        public:
            Phaser(std::string name = "Phaser") :
                BaseName(name),
                BaseConnection(PHASER_CONNECTION_NUM)
            {
                m_dMinCutoffs[0] = 16.0;
                m_dMinCutoffs[1] = 33.0;
                m_dMinCutoffs[2] = 48.0;
                m_dMinCutoffs[3] = 98.0;
                m_dMinCutoffs[4] = 160.0;
                m_dMinCutoffs[5] = 260.0;
                m_dMaxCutoffs[0] = 1600.0;
                m_dMaxCutoffs[1] = 3300.0;
                m_dMaxCutoffs[2] = 4800.0;
                m_dMaxCutoffs[3] = 9800.0;
                m_dMaxCutoffs[4] = 16000.0;
                m_dMaxCutoffs[5] = 26000.0;

                m_pInput = new Connection::InputConnection();

                m_pLFO = new Generator::BaseLFO();
                m_pLFO->setFreq(0.01);
                m_pLFO->setWaveform(Generator::BaseWavetable::WAVEFORM_TRIANGLE_BL);

                m_pScaler = new Connection::ScalerConnection("phaser.lfo.scaler", 500.0, 500.0);
                m_pScaler->attachInput(m_pLFO);

                for (UInt8 stageIndex = 0; stageIndex < PHASER_FILTERSTAGES_NUM; ++stageIndex)
                {
                    m_pFilterStages[stageIndex] = new Filter::SwitchableAPF();
                    m_pFilterStages[stageIndex]->setSecondOrder(false);
                    m_pFilterStages[stageIndex]->setQ(0.707);
                    m_pFilterStages[stageIndex]->setCutoff(m_dMinCutoffs[stageIndex]);

                    if (stageIndex == 0)
                        m_pFilterStages[stageIndex]->attachInput(m_pInput);
                    else
                        m_pFilterStages[stageIndex]->attachInput(m_pFilterStages[stageIndex - 1]);
                }

                Init();
            }

            virtual ~Phaser()
            {
                if (m_pInput)
                    delete m_pInput;
                m_pInput = NULL;

                if (m_pLFO)
                    delete m_pLFO;
                m_pLFO = NULL;

                if (m_pScaler)
                    delete m_pScaler;
                m_pScaler = NULL;

                if (m_pFilterStages)
                {
                    for (UInt8 stageIndex = 0; stageIndex < PHASER_FILTERSTAGES_NUM; ++stageIndex)
                        delete m_pFilterStages[stageIndex];
                }
            }

        public:
            /* getter/setter */
            virtual void setDepth(const double Depth) { m_dDepth = Depth; }
            virtual double getDepth(void) { return m_dDepth; }

        public:
            /* inherited */
            virtual void Init(void)
            {
                m_bBypass = false;
                m_dDepth = 0.5;
                m_dLastFeedback = 0.0;
            }

            virtual void Reset(void)
            {
                for (UInt8 stageIndex = 0; stageIndex < PHASER_FILTERSTAGES_NUM; ++stageIndex)
                {
                    if (m_pFilterStages[stageIndex])
                        m_pFilterStages[stageIndex]->Reset();
                }
            }
            virtual double Process(const double Input);
            virtual double processConnection(void);
            virtual void setSamplerate(const double Samplerate);

        public:
            /* getter/setter */
            void setLFOFreq(const double Freq);

        public:
            /* attach methods */
            void attachLFOFreq(BaseConnection *connection) { inputConnections[PHASER_CONNECTION_LFOFREQ] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[PHASER_CONNECTION_INPUT] = connection; }

        private:
            Generator::BaseLFO *m_pLFO;
            Filter::SwitchableAPF *m_pFilterStages[6];
            Connection::ScalerConnection *m_pScaler;
            Connection::InputConnection *m_pInput;

            double m_dDepth;
            double m_dLastFeedback;
            double m_dMinCutoffs[PHASER_FILTERSTAGES_NUM];
            double m_dMaxCutoffs[PHASER_FILTERSTAGES_NUM];
        };
    }
}

#endif
