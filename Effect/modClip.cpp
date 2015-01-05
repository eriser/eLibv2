#include <Effect/modClip.h>

using namespace eLibV2;

void FxClip::setClipLevel(double ClipLevel)
{
	dClipLevel = clamp(ClipLevel, CLIP_LEVEL_MIN, CLIP_LEVEL_MAX);
}

void FxClip::setClipMode(long ClipMode)
{
	lClipMode = clamp(ClipMode, CLIP_MODE_MIN, CLIP_MODE_MAX);
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

	ModuleLogger::print("Begin Test");
    for (long TestMode = CLIP_MODE_POSITIVE; TestMode <= CLIP_MODE_BOTH; TestMode++)
    {
        ModuleLogger::print("setting mode to: %li", TestMode);
        setClipMode(TestMode);
		ModuleLogger::print("setting level to: 0.5");
        setClipLevel(0.8);
        for (long ii = 0; ii < 10; ii++)
        {
            In = GenerateTestSignal();
            Out = Process(In);
			ModuleLogger::print("mode: %li in: %lf out: %lf", TestMode, In, Out);
        }
    }
	ModuleLogger::print("End Test");

	return true;
}
