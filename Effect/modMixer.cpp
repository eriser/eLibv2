#include <Effect/modMixer.h>

using namespace eLibV2::Effect;

void FxMixer::Init(void)
{
    setVolume(0.0);
    setMode(MODE_MIXER_ADD);
}

void FxMixer::Reset(void)
{
    Init();
}

double FxMixer::Process2(double Input1, double Input2)
{
    double res = 0.0;

    switch (eMode)
    {
        case MODE_MIXER_ADD:
            res = Input1 + Input2;
            break;

        case MODE_MIXER_SUBTRACT:
            res = Input1 - Input2;
            break;

        case MODE_MIXER_INVSUBTRACT:
            res = Input2 - Input1;
            break;
    }

    return res;
}
