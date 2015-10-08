#ifndef MODFLANGER_H_
#define MODFLANGER_H_

#include <Base/BaseEffect.h>
#include <Util/Helper.h>

#include <Generator/BaseLFO.h>
#include <Effect/Delay.h>
#include <Connection/ScalerConnection.h>

namespace eLibV2
{
    namespace Effect
    {
        class Flanger : public Base::BaseEffect
        {
        public:
            enum
            {
                FLANGER_CONNECTION_BYPASS = 0,
                FLANGER_CONNECTION_INPUT,
                FLANGER_CONNECTION_LFOFREQ,
                FLANGER_CONNECTION_NUM
            };

        public:
            Flanger(std::string name = "Flanger") :
                BaseName(name),
                BaseConnection(FLANGER_CONNECTION_NUM)
            {
                m_pLFO = new Generator::BaseLFO();
                m_pLFO->setFreq(0.01);
                m_pLFO->setWaveform(Generator::BaseWavetable::WAVETABLE_WAVEFORM_TRIANGLE_BL);

                m_pScaler = new Connection::ScalerConnection("phaser.lfo.scaler", 500.0, 500.0);
                m_pScaler->attachInput(m_pLFO);

                m_pDelay = new Effect::Delay();
                m_pDelay->attachDelayLength(m_pScaler);

                m_bBypass = false;

                Init();
            }

            virtual ~Flanger()
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
            virtual void setSamplerate(const double Samplerate);

        public:
            /* getter/setter */
            bool getBypass() { return m_bBypass; }

            void setBypass(const bool Bypass) { m_bBypass = Bypass; }
            void setLFOFreq(const double Freq);

        public:
            /* attach methods */
            void attachLFOFreq(BaseConnection *connection) { inputConnections[FLANGER_CONNECTION_LFOFREQ] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[FLANGER_CONNECTION_INPUT] = connection; m_pDelay->attachInput(connection); }

        private:
            bool m_bBypass;
            Generator::BaseLFO *m_pLFO;
            Effect::Delay *m_pDelay;
            Connection::ScalerConnection *m_pScaler;
        };
    }
}

#endif