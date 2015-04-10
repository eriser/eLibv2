#include <Effect/modClip.h>

using namespace eLibV2;
using namespace eLibV2::Effect;

void FxClip::setClipLevel(double ClipLevel)
{
    dClipLevel = ModuleHelper::clamp(ClipLevel, CLIP_LEVEL_MIN, CLIP_LEVEL_MAX);
}

void FxClip::setClipMode(ClipModes ClipMode)
{
    eClipMode = ClipMode;
}

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
            if ((Input > dClipLevel) || (Input < -dClipLevel))
                res = dClipLevel;
            else
                res = Input;
            break;
    }
    return res;
}

bool FxClip::Test(void)
{
    double In, Out;
    ClipModes modes[] = { CLIP_MODE_POSITIVE, CLIP_MODE_NEGATIVE, CLIP_MODE_BOTH };

    ModuleLogger::print("Begin Test");
    for (unsigned int TestMode = 0; TestMode <= sizeof(modes) / sizeof(ClipModes); TestMode++)
    {
        ModuleLogger::print("setting mode to: %li", TestMode);
        setClipMode(modes[TestMode]);
        ModuleLogger::print("setting level to: 0.5");
        setClipLevel(0.8);
        for (long ii = 0; ii < 10; ii++)
        {
            In = ModuleHelper::GenerateTestSignal();
            Out = Process(In);
            ModuleLogger::print("mode: %li in: %lf out: %lf", TestMode, In, Out);
        }
    }
    ModuleLogger::print("End Test");

    return true;
}
