#ifndef MODMIXER_H_
#define MODMIXER_H_

#include <Base/BaseEffect.h>

namespace eLibV2
{
    namespace Effect
    {
        enum MixerMode
        {
            MIXER_MODE_UNDEF = 0,
            MIXER_MODE_ADD,
            MIXER_MODE_SUBTRACT,
            MIXER_MODE_INVSUBTRACT
        };

        class FxMixer : public Base::BaseEffect
        {
        public:
            FxMixer(std::string name = "FxMixer")
                : Base::BaseName(name) {
                Init();
            }

            void Init(void);
            void Reset(void);

            void setVolume(double Volume) { dVolume = Volume; }
            double getVolume(void) { return dVolume; }

            void setMode(MixerMode Mode) { eMode = Mode; }
            MixerMode getMode(void) { return eMode; }

            double Process2(double Input1, double Input2);

        private:
            double Process(double Input) { return Input; }

        private:
            double dVolume;
            MixerMode eMode;
        };
    }
}

#endif
