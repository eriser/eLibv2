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
                Base::BaseName(name),
                BaseConnection(OSC_CONNECTION_NUM)
            {
                Init();
            }

        public:
            virtual void Init(void);
            virtual void Reset(void);
            virtual double Process(VstInt16 Note);
            virtual double processConnection();

        public:
            /* set and get methods */
            void setWaveform(VstInt32 Waveform);
            void setCoarse(double Coarse);
            void setFinetune(double Finetune);
            void setSamplerate(double Samplerate);
            void adjustScaler();

            VstInt32 getWaveform(void) { return lWaveform; }
            double getCoarse(void) { return dCoarse; }
            double getFinetune(void) { return dFinetune; }
            VstInt32 getNumWaveforms(void);

        public:
            /* attach methods */
            void attachNote(Base::BaseConnection *connection) { inputConnections[OSC_CONNECTION_NOTE] = connection; }
            void attachWaveform(Base::BaseConnection *connection) { inputConnections[OSC_CONNECTION_WAVEFORM] = connection; }
            void attachCoarse(Base::BaseConnection *connection) { inputConnections[OSC_CONNECTION_COARSE] = connection; }
            void attachFinetune(Base::BaseConnection *connection) { inputConnections[OSC_CONNECTION_FINETUNE] = connection; }

        protected:
            VstInt32 lWaveform;
            double dCoarse, dFinetune, dPhase, dSamplerate, dScaler;
        };
    }
}

#endif

