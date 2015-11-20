#ifndef MODMIXER_H_
#define MODMIXER_H_

#include <Effect/BaseEffect.h>

namespace eLibV2
{
    namespace Effect
    {
        class Mixer : public Base::BaseName
        {
        public:
            enum MixerMode
            {
                MIXER_MODE_UNDEF = 0,
                MIXER_MODE_ADD,
                MIXER_MODE_SUBTRACT,
                MIXER_MODE_INVSUBTRACT
            };

        public:
            Mixer(std::string name = "Mixer") :
                BaseName(name)
            {
                Init();
            }
            virtual ~Mixer() {}

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
