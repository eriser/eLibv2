#ifndef MODBASELFO_H_
#define MODBASELFO_H_

#include <Generator/BaseOsc.h>

namespace eLibV2
{
    namespace Generator
    {
        class BaseLFO : public BaseOscillator
        {
        public:
            enum
            {
                LFO_CONNECTION_FREQ = 0,
                LFO_CONNECTION_WAVEFORM,
                LFO_CONNECTION_NUM
            };

        public:
            BaseLFO(std::string name = "BaseLFO") :
                BaseName(name),
                BaseConnection(LFO_CONNECTION_NUM)
            {
                Init();
            }
            virtual ~BaseLFO() {}

        public:
            /* inherited methods */
            virtual void Init(void);
            virtual double processConnection(void);

            virtual void adjustPhases(VstInt16 Note);

        public:
            /* set and get methods */
            void setFreq(double Freq) { m_dFreq = ModuleHelper::clamp(Freq, 0.005, 100.0); }
            double getFreq() { return m_dFreq; }

        public:
            /* attach methods */
            void attachFreq(Base::BaseConnection *connection) { inputConnections[LFO_CONNECTION_FREQ] = connection; }
            void attachWaveform(Base::BaseConnection *connection) { inputConnections[LFO_CONNECTION_WAVEFORM] = connection; }

        private:
            double m_dFreq;
        };
    }
}

#endif
