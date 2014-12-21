#ifndef MODMIXER_H_
#define MODMIXER_H_

#include <Base/modBaseEffect.h>

using namespace eLibV2;

namespace eLibV2
{
    enum
    {
        MODE_MIXER_ADD = 0,
        MODE_MIXER_SUBTRACT,
        MODE_MIXER_INVSUBTRACT
    };

    class FxMixer : public BaseEffect
    {
    public:
		FxMixer() : BaseName("FxMixer") { Init(); }

        void Init(void);
        void Reset(void);
		bool Test(void) {}

        void setVolume(double Volume) {dVolume = Volume;}
        void setMode(long Mode) {lMode = Mode;}

        double getVolume(void) {return dVolume;}
        long getMode(void) {return lMode;}

        double Process(double Input) {return Input;}
        double Process2(double Input1, double Input2);

    private:
        double dVolume;
        long lMode;
    };
}

#endif
