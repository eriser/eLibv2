#ifndef MODPHASER_H_
#define MODPHASER_H_

#include <Base/BaseEffect.h>
#include <Util/Helper.h>

#include <Generator/BaseLFO.h>
#include <Effect/Delay.h>
#include <Connection/ScalerConnection.h>

namespace eLibV2
{
    namespace Effect
    {
        class Phaser : public Base::BaseEffect
        {
        public:
            enum
            {
                PHASER_CONNECTION_INPUT = 0,
                PHASER_CONNECTION_LFOFREQ,
                PHASER_CONNECTION_NUM
            };

        public:
            Phaser(std::string name = "Phaser") :
                Base::BaseName(name),
                BaseConnection(PHASER_CONNECTION_NUM)
            {
                m_pLFO = new Generator::BaseLFO();
                m_pLFO->setFreq(0.01);
                m_pLFO->setWaveform(Generator::BaseWavetable::WAVETABLE_WAVEFORM_TRIANGLE_BL);

                m_pScaler = new Connection::ScalerConnection("phase.lfo.scaler", 250.0, 500.0);
                m_pScaler->attachInput(m_pLFO);

                m_pDelay = new Effect::Delay();
                m_pDelay->attachDelayLength(m_pScaler);

                Init();
            }

            virtual ~Phaser()
            {
                delete m_pLFO;
                delete m_pDelay;
                delete m_pScaler;
            }

        public:
            /* inherited */
            virtual void Init(void) {}
            virtual void Reset(void) {}
            virtual double Process(const double Input);
            virtual double processConnection();
            virtual void setSamplerate(double Samplerate);

        public:
            void setLFOFreq(double Freq);

        public:
            /* attach methods */
            void attachLFOFreq(Base::BaseConnection *connection) { inputConnections[PHASER_CONNECTION_LFOFREQ] = connection; }
            void attachInput(Base::BaseConnection *connection) { inputConnections[PHASER_CONNECTION_INPUT] = connection; m_pDelay->attachInput(connection); }

        private:
            Generator::BaseLFO *m_pLFO;
            Effect::Delay *m_pDelay;
            Connection::ScalerConnection *m_pScaler;
        };
    }
}

#endif
