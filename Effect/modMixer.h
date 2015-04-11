#ifndef MODMIXER_H_
#define MODMIXER_H_

#include <Base/modBaseEffect.h>

namespace eLibV2
{
    namespace Effect
    {
        enum MixerModes
        {
            MODE_MIXER_NONE = 0,
            MODE_MIXER_ADD,
            MODE_MIXER_SUBTRACT,
            MODE_MIXER_INVSUBTRACT,
            MODE_MIXER_COUNT
        };

        class FxMixer : public BaseEffect
        {
        public:
            FxMixer(std::string name = "FxMixer")
                : BaseName(name) {
                Init();
            }

            void Init(void);
            void Reset(void);

            void setVolume(double Volume) { dVolume = Volume; }
            double getVolume(void) { return dVolume; }

            void setMode(MixerModes Mode) { eMode = Mode; }
            MixerModes getMode(void) { return eMode; }

            double Process2(double Input1, double Input2);

        private:
            double Process(double Input) { return Input; }

        private:
            double dVolume;
            MixerModes eMode;
        };
    }
}

#endif
