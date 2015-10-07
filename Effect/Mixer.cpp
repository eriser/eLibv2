#include <Effect/Mixer.h>

using namespace eLibV2::Effect;

void Mixer::Init(void)
{
    setVolume(0.0);
    setMode(MIXER_MODE_ADD);
}

void Mixer::Reset(void)
{
    Init();
}

double Mixer::Process2(const double Input1, const double Input2)
{
    double res = 0.0;

    switch (eMode)
    {
        case MIXER_MODE_ADD:
            res = Input1 + Input2;
            break;

        case MIXER_MODE_SUBTRACT:
            res = Input1 - Input2;
            break;

        case MIXER_MODE_INVSUBTRACT:
            res = Input2 - Input1;
            break;
    }

    return res;
}
