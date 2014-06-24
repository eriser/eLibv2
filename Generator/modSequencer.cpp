/*
 * modSequencer.cpp
 *
 *  Created on: 06.01.2012
 *      Author: dedokter
 */

#include "modSequencer.h"

using namespace eLibV2;

BaseSequencer::BaseSequencer()
{
    Init();
}

void BaseSequencer::Init()
{
    ModuleName.assign("Sequencer");

    setSamplerate(44100.0);
    setBpm(120.0);
    setSpeed(2);
    setSync(false);
    lCurrentSamplePosition = 0;
    lCurrentStepIndex = 0;
}

void BaseSequencer::Reset(void)
{
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

void BaseSequencer::getStepValue(unsigned long Index, double *Value)
{
    if ((Index >= SequencerData.size()) || (!Value))
        return;

    *Value = SequencerData.at(Index);
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

    getStepValue(lCurrentStepIndex, &ret);

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

    return(ret);
}
