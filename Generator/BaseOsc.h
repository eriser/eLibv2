#ifndef MODBASEOSC_H_
#define MODBASEOSC_H_

#include <Base/BaseGenerator.h>
#include <Generator/Wavetable.h>

#include <Util/FrequencyTable.h>
#include <Util/Logger.h>
#include <Util/Helper.h>

namespace eLibV2
{
    namespace Generator
    {
        static const double BASEOSC_COARSE_RANGE = 24.0;
        static const double BASEOSC_FINE_RANGE = 100.0;
        static const double QUADPHASE_DIVIDER = 8.0;

        /**
        this class implements a basic oscillator module using a wavetable
        */
        class BaseOscillator : public Base::BaseGenerator
        {
        protected:
            enum
            {
                OSC_CONNECTION_NOTE = 0,
                OSC_CONNECTION_WAVEFORM,
                OSC_CONNECTION_COARSE,
                OSC_CONNECTION_FINETUNE,
                OSC_CONNECTION_NUM
            };

        public:
            BaseOscillator(std::string name = "BaseOscillator") :
                BaseName(name),
                BaseConnection(OSC_CONNECTION_NUM)
            {
                Init();
            }
            virtual ~BaseOscillator() {}

        public:
            /* inherited methods */
            virtual void Init(void);
            virtual void Reset(void);
            virtual double Process(const VstInt16 Note);
            virtual double processConnection(void);
            virtual void setSamplerate(const double Samplerate);

            virtual void adjustPhases(VstInt16 Note);

        public:
            /* set and get methods */
            void setWaveform(VstInt32 Waveform);
            void setCoarse(const double Coarse);
            void setFinetune(const double Finetune);
            void adjustScaler();

            VstInt32 getWaveform(void) { return m_lWaveform; }
            double getCoarse(void) { return m_dCoarse; }
            double getFinetune(void) { return m_dFinetune; }
            VstInt32 getNumWaveforms(void);

            double getQuadOutput(void) { return m_dQuadOutput; }

        public:
            /* attach methods */
            void attachNote(Base::BaseConnection *connection) { inputConnections[OSC_CONNECTION_NOTE] = connection; }
            void attachWaveform(Base::BaseConnection *connection) { inputConnections[OSC_CONNECTION_WAVEFORM] = connection; }
            void attachCoarse(Base::BaseConnection *connection) { inputConnections[OSC_CONNECTION_COARSE] = connection; }
            void attachFinetune(Base::BaseConnection *connection) { inputConnections[OSC_CONNECTION_FINETUNE] = connection; }

        protected:
            VstInt32 m_lWaveform;
            double m_dCoarse, m_dFinetune;
            double m_dPhase, m_dQuadPhase, m_dScaler;
            double m_dQuadOutput;
        };
    }
}

#endif

