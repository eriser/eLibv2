#include <Generator/modSequencer.h>

void BaseSequencer::Init()
{
    setSamplerate(44100.0);
    setBpm(120.0);
    setSpeed(2);
    setSync(false);
    lCurrentSamplePosition = 0;
    lCurrentStepIndex = 0;
}

long BaseSequencer::getStepCount(void)
{
    return SequencerData.size();
}

void BaseSequencer::setStepValue(unsigned long Index, double Value)
{
    double StepValue;

    if (Index >= SequencerData.size())
        return;

    StepValue = Value;
    SequencerData.at(Index) = StepValue;
}

double BaseSequencer::getStepValue(unsigned long Index)
{
	double res = 0.0;

    if (Index < SequencerData.size())
		res = SequencerData.at(Index);

	return res;
}

void BaseSequencer::addSteps(long Count)
{
    double temp;
    for (long StepIndex = 0; StepIndex < Count; StepIndex++)
    {
        temp = 0.0;
        SequencerData.push_back(temp);
    }
}

double BaseSequencer::Process(void)
{
    double ret;
    bool advance = false;
    long lNeededSamplePosition = 0;

    ret = getStepValue(lCurrentStepIndex);

    lCurrentSamplePosition++;
    if (bSync)
        lNeededSamplePosition = (long)quarter2samples(dSpeed, dBpm, dSamplerate);
    else
        lNeededSamplePosition = (long)ms2samples(dSpeed * 1000, dSamplerate);

    if (lCurrentSamplePosition >= lNeededSamplePosition)
    {
        lCurrentSamplePosition -= lNeededSamplePosition;
        lCurrentStepIndex++;
        lCurrentStepIndex %= SequencerData.size();
    }

    return ret ;
}
