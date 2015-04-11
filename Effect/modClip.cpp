#include <Effect/modClip.h>

using namespace eLibV2::Effect;

void FxClip::Init()
{
    setClipLevel(1.0);
    setClipMode(CLIP_MODE_POSITIVE);
}

double FxClip::Process(double Input)
{
    double res = 0.0;

    switch (eClipMode)
    {
        case CLIP_MODE_POSITIVE:
            if (Input > dClipLevel)
                res = dClipLevel;
            else
                res = Input;
            break;

        case CLIP_MODE_NEGATIVE:
            if (Input < -dClipLevel)
                res = -dClipLevel;
            else
                res = Input;
            break;

        case CLIP_MODE_BOTH:
            if (Input < -dClipLevel)
                res = -dClipLevel;
            else if (Input > dClipLevel)
                res = dClipLevel;
            else
                res = Input;
            break;
    }
    return res;
}
