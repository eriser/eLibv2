#include <Effect/modEnvFollower.h>

using namespace eLibV2::Effect;

void FxEnvFollower::Init(void)
{
    setAttack(5.0);
    setRelease(50.0);
    setSamplerate(44100.0);

    Reset();
}

void FxEnvFollower::Reset(void)
{
    tAttack = 0;
    tRelease = 0;
    dLastInput = 0.0;
}

double FxEnvFollower::Process(double input)
{
    double res = 0.0;
    long tATime, tRTime;

    tATime = (long)(Util::ModuleHelper::ms2samples(dAttack, dSampleRate)) * 10;
    tRTime = (long)(Util::ModuleHelper::ms2samples(dRelease, dSampleRate)) * 10;

    tATime = (tATime == 0) ? 1 : tATime;
    tRTime = (tRTime == 0) ? 1 : tRTime;

    // absolute value
    if (input < 0.0)
        input = -input;

    // increasing
    if (input < dLastInput)
    {
        tRelease = 0;
        // in Attack phase
        if (tAttack < tATime)
        {
            res = input * tAttack * (input - dLastInput) / tATime;
            tAttack++;
        }
        else
        {
            res = input;
        }
    }
    // decreasing
    else
    {
        tAttack = 0;
        // in Release phase
        if (tRelease < tRTime)
        {
            res = input * tRelease * (dLastInput - input) / tRTime;
            tRelease++;
        }
        else
        {
            res = input;
        }
    }
    dLastInput = input;

    return res;
}
