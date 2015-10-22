#ifndef MODROOMREVERB_H_
#define MODROOMREVERB_H_

#include <Base/BaseEffect.h>
#include <Effect/Delay.h>
#include <Filter/OnePoleLPF.h>
#include <Filter/DelayAPF.h>
#include <Filter/CombFilter.h>
#include <Filter/CombFilterLPF.h>
#include <Util/Helper.h>

namespace eLibV2
{
    namespace Effect
    {
        class RoomReverb : public Base::BaseEffect
        {
        public:
            enum
            {
                ROOMREVERB_CONNECTION_BYPASS = 0,
                ROOMREVERB_CONNECTION_INPUT,
                ROOMREVERB_CONNECTION_NUM
            };

        public:
            RoomReverb(std::string name = "RoomReverb") :
                BaseName(name),
                BaseConnection(ROOMREVERB_CONNECTION_NUM)
            {
                m_pPreDelay = new Delay();
                m_pInputLPF = new Filter::OnePoleLPF();
                for (int i = 0; i < 2; ++i)
                {
                    m_pInputAPF[i] = new Filter::DelayAPF();
                    m_pCombLeft[i] = new Filter::CombFilter();
                    m_pCombLPFLeft[i] = new Filter::CombFilterLPF();
                    m_pCombRight[i] = new Filter::CombFilter();
                    m_pCombLPFRight[i] = new Filter::CombFilterLPF();
                    m_pOutputLPF[i] = new Filter::OnePoleLPF();
                    m_pOutputAPF[i] = new Filter::DelayAPF();
                }
                Init();
            }
            virtual ~RoomReverb() {}

        public:
            /* inherited */
            virtual void Init(void);
            virtual void Reset(void) {}
            virtual double Process(const double Input);
            virtual double processConnection(void);
            virtual void setSamplerate(const double Samplerate);

        public:
            void setMixLevel(const double MixLevel) { m_dMixLevel = MixLevel; }
            void setRT60(const double RT60);
            void setPreDelayGain(const double PreDelayGain) { m_dPreDelayGain = PreDelayGain; }
            void setPreDelayLength(const double PreDelayLength)
            {
                m_dPreDelayLength = PreDelayLength;
                m_pPreDelay->setDelayLength(ModuleHelper::ms2samples(m_dPreDelayLength, getSamplerate()));
            }
            void setInputBandwidth(const double InputBandwidth)
            {
                m_dInputBandwidth = InputBandwidth;
                m_pInputLPF->setGain(m_dInputBandwidth);
            }
            void setOutputDamping(const double OutputDamping)
            {
                m_dOutputDamping = OutputDamping;
                m_pOutputLPF[0]->setGain(m_dOutputDamping);
                m_pOutputLPF[1]->setGain(m_dOutputDamping);
            }

            double getSecondOutput(void) { return m_dSecondOutput; }

        public:
            void attachBypass(BaseConnection *connection) { inputConnections[ROOMREVERB_CONNECTION_BYPASS] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[ROOMREVERB_CONNECTION_INPUT] = connection; }

        private:
            double m_dPreDelayGain, m_dPreDelayLength;
            double m_dInputBandwidth, m_dOutputDamping;
            double m_dReverbTimeRT60;
            double m_dMixLevel;
            double m_dSecondOutput;
            Delay *m_pPreDelay;
            Filter::OnePoleLPF *m_pInputLPF;
            Filter::DelayAPF *m_pInputAPF[2];
            Filter::CombFilter *m_pCombLeft[2];
            Filter::CombFilterLPF *m_pCombLPFLeft[2];
            Filter::CombFilter *m_pCombRight[2];
            Filter::CombFilterLPF *m_pCombLPFRight[2];
            Filter::OnePoleLPF *m_pOutputLPF[2];
            Filter::DelayAPF *m_pOutputAPF[2];
        };
    }
}

#endif
