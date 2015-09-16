#include <Effect/RMSCalc.h>

using namespace eLibV2::Effect;

void RMSCalculator::Init(void)
{
    setMode(RMS_MODE_UNDEF);
    mRms = 0.0;
    mGain = 1.0;
    mAlpha = 0.9;
    mRmsSmoothed = 0.0;
    mSamples = 0;
}

void RMSCalculator::Reset(void)
{
    Init();
}

void RMSCalculator::InputSample(const double Input)
{
    mRms += (Input * Input);
    mSamples++;
}

double RMSCalculator::Process(const double Input)
{
    double res = 0.0;

    res = sqrt(mRms / mSamples);
    mRmsSmoothed = mRmsSmoothed * mAlpha + (1 - mAlpha) * res;
    mRms = 0.0;
    mSamples = 0;

    double rmsdB = 20.0 * log10(mGain * mRmsSmoothed);
    return mRmsSmoothed;
}
