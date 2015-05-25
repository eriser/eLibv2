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
                CONNECTION_OSC_INPUT_NOTE,
                CONNECTION_OSC_INPUT_WAVEFORM,
                CONNECTION_OSC_INPUT_COARSE,
                CONNECTION_OSC_INPUT_FINETUNE
            };

        public:
            BaseOscillator(std::string name = "BaseOscillator") : Base::BaseName(name) { Init(); }

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
            void attachNote(Base::BaseConnection *controller) { connect(CONNECTION_OSC_INPUT_NOTE, controller); }
            void attachWaveform(Base::BaseConnection *controller) { connect(CONNECTION_OSC_INPUT_WAVEFORM, controller); }
            void attachCoarse(Base::BaseConnection *controller) { connect(CONNECTION_OSC_INPUT_COARSE, controller); }
            void attachFinetune(Base::BaseConnection *controller) { connect(CONNECTION_OSC_INPUT_FINETUNE, controller); }

        protected:
            VstInt32 lWaveform;
            double dCoarse;
            double dFinetune;
            double dPhase;
            double dSamplerate;
            double dScaler;
        };
    }
}

#endif

