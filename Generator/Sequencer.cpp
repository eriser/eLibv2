#include <Generator/Sequencer.h>

using namespace eLibV2::Generator;
using namespace eLibV2::Util;

void BaseSequencer::Init()
{
    setSamplerate(44100.0);
    setBpm(120.0);
    setSpeed(2);
    setSync(false);
    lCurrentSamplePosition = 0;
    lCurrentStepIndex = 0;
}

SInt32 BaseSequencer::getStepCount(void)
{
    return SequencerData.size();
}

void BaseSequencer::setStepValue(UInt32 Index, double Value)
{
    double StepValue;

    if (Index >= SequencerData.size())
        return;

    StepValue = Value;
    SequencerData.at(Index) = StepValue;
}

double BaseSequencer::getStepValue(UInt32 Index)
{
    double res = 0.0;

    if (Index < SequencerData.size())
        res = SequencerData.at(Index);

    return res;
}

void BaseSequencer::addSteps(SInt32 Count)
{
    double temp;
    for (SInt32 StepIndex = 0; StepIndex < Count; StepIndex++)
    {
        temp = 0.0;
        SequencerData.push_back(temp);
    }
}

double BaseSequencer::Process(const double Input)
{
    double dOutput;
    SInt32 lNeededSamplePosition = 0;
    double dSamplerate = getSamplerate();

    dOutput = getStepValue(lCurrentStepIndex);

    lCurrentSamplePosition++;
    if (bSync)
        lNeededSamplePosition = (SInt32)ModuleHelper::quarter2samples(dSpeed, dBpm, dSamplerate);
    else
        lNeededSamplePosition = (SInt32)ModuleHelper::ms2samples(dSpeed * 1000, dSamplerate);

    if (lCurrentSamplePosition >= lNeededSamplePosition)
    {
        lCurrentSamplePosition -= lNeededSamplePosition;
        lCurrentStepIndex++;
        lCurrentStepIndex %= SequencerData.size();
    }

    return dOutput;
}
