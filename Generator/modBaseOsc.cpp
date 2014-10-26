/*
 * modBaseOsc.cpp
 *
 *  Created on: 24.11.2011
 *      Author: dedokter
 */

#include <Generator/modBaseOsc.h>
#include <Loader/modLPSLoader.h>

using namespace eLibV2;

BaseOscillator::BaseOscillator()
{
    Init();
}

BaseOscillator::~BaseOscillator()
{
    if (pBaseWavetable)
        delete pBaseWavetable;
}

void BaseOscillator::Init(void)
{
    setModuleName("BaseOscillator");
    pBaseWavetable = new BaseWavetable();

    setWaveform(1);
    setCoarse(0.0);
    setFinetune(0.0);
    setSamplerate(44100.0);
    dPhase = 0.0;

    SetupFreqs();
}

void BaseOscillator::Reset(void)
{
    setSamplerate(dSamplerate);
    dPhase = 0.0;
}

void BaseOscillator::SetupFreqs(void)
{
    // make frequency (Hz) table
    double k = 1.059463094359;  // 12th root of 2
    double a = 1.71875; // a
    a *= k;     // b
    a *= k;     // bb
    a *= k;     // c, frequency of midi note 0

    // 128 midi notes
    for (VstInt16 FreqIndex = 0; FreqIndex < kNumFrequencies; FreqIndex++)
    {
        freqtab[FreqIndex] = a;
        a *= k;
    }
}

void BaseOscillator::setWaveform(VstInt32 Waveform)
{
    lWaveform = Waveform;
    setSamplerate(dSamplerate);
    setScaler();
}

void BaseOscillator::setSamplerate(double Samplerate)
{
    dSamplerate = Samplerate;
    setScaler();
}

void BaseOscillator::setScaler()
{
    if (pBaseWavetable)
        dScaler = pBaseWavetable->getWaveSize(lWaveform) / dSamplerate;
}

void BaseOscillator::setCoarse(double Coarse)
{
//    if ((Coarse >= -BASEOSC_COARSE_RANGE) && (Coarse <= BASEOSC_COARSE_RANGE))
        dCoarse = Coarse;
//    else
//        dbgOutputF("coarse out of range: %lf (%lf - %lf) -> using %lf", Coarse, -BASEOSC_COARSE_RANGE, BASEOSC_COARSE_RANGE, dCoarse);
}

void BaseOscillator::setFinetune(double Finetune)
{
    if ((Finetune >= -BASEOSC_FINE_RANGE) && (Finetune <= BASEOSC_FINE_RANGE))
        dFinetune = Finetune;
    else
        dbgOutputF("finetune out of range: %lf (%lf - %lf) -> using %lf", Finetune, -BASEOSC_FINE_RANGE, BASEOSC_FINE_RANGE, dFinetune);
}

VstInt32 BaseOscillator::getNumWaveforms(void)
{
    if (pBaseWavetable)
        return pBaseWavetable->getNumLoadedWaveforms();
    else
        return 0;
}

double BaseOscillator::Process(VstInt16 Note)
{
    // modulation in simetones
    double data, dBase, dTune, dFreq;

    data = pBaseWavetable->getWaveData(lWaveform, dPhase);

    dBase = freqtab[(Note + (VstInt16)(dCoarse)) & 0x7f];
    if (dFinetune >= 0.0)
    {
        dTune = (freqtab[(Note + (VstInt16)(dCoarse) + 1) & 0x7f] - dBase) * (((dFinetune + 100.0) / 200.0) - 0.5) * 2;
        dFreq = (dBase + dTune) * dScaler;
    }
    else
    {
        dTune = (dBase - freqtab[(Note + (VstInt16)(dCoarse) - 1) & 0x7f]) * (0.5 - ((dFinetune + 100.0) / 200.0)) * 2;
        dFreq = (dBase - dTune) * dScaler;
    }
    dPhase += dFreq;
    dPhase = pBaseWavetable->adjustPhase(lWaveform, dPhase);

    return data;
}

void BaseOscillator::runTests(void)
{
}