#include <Effect/modClip.h>

void FxClip::setClipLevel(double ClipLevel)
{
    if ((ClipLevel >= CLIP_LEVEL_MIN) && (ClipLevel <= CLIP_LEVEL_MAX))
        dClipLevel = ClipLevel;
    else
        dbgOutputF("level out of range: %lf -> using %lf", ClipLevel, dClipLevel);
}

void FxClip::setClipMode(long ClipMode)
{
    if ((ClipMode >= CLIP_MODE_MIN) && (ClipMode < CLIP_MODE_MAX))
        lClipMode = ClipMode;
    else
        dbgOutputF("mode out of range: %li -> using %li", ClipMode, lClipMode);
}

void FxClip::Init()
{
    setClipLevel(1.0);
    setClipMode(CLIP_MODE_POSITIVE);
}

double FxClip::Process(double Input)
{
    double res = 0.0;

    switch (lClipMode)
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

    TestBeginMsg();
    for (long TestMode = CLIP_MODE_POSITIVE; TestMode <= CLIP_MODE_BOTH; TestMode++)
    {
        dbgOutputF("setting mode to: %li", TestMode);
        setClipMode(TestMode);
        dbgOutputF("setting level to: 0.5");
        setClipLevel(0.8);
        for (long ii = 0; ii < 10; ii++)
        {
            In = GenerateTestSignal();
            Out = Process(In);
            dbgOutputF("mode: %li in: %lf out: %lf", TestMode, In, Out);
        }
    }
    TestEndMsg();

	return true;
}
