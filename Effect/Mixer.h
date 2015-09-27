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

        class FxMixer : public Base::BaseName
        {
        public:
            FxMixer(std::string name = "FxMixer") :
                Base::BaseName(name)
            {
                Init();
            }
            virtual ~FxMixer() {}

            /* inherited */
            virtual void Init(void);
            virtual void Reset(void);

            void setVolume(double Volume) { dVolume = Volume; }
            double getVolume(void) { return dVolume; }

            void setMode(MixerMode Mode) { eMode = Mode; }
            MixerMode getMode(void) { return eMode; }

            double Process2(const double Input1, const double Input2);

        private:
            virtual double Process(const double Input) { return Input; }

        private:
            double dVolume;
            MixerMode eMode;
        };
    }
}

#endif
