#include <Generator/BaseOsc.h>

using namespace eLibV2::Generator;
using namespace eLibV2::Util;

void BaseOscillator::Init(void)
{
    setWaveform(1);
    setCoarse(0.0);
    setFinetune(0.0);
    setSamplerate(44100.0);
    dPhase = 0.0;

    FrequencyTable::SetupFreqs();
}

void BaseOscillator::Reset(void)
{
    setSamplerate(dSamplerate);
    dPhase = 0.0;
}

void BaseOscillator::setWaveform(VstInt32 Waveform)
{
    if (lWaveform != Waveform)
    {
        lWaveform = Waveform;
        setSamplerate(dSamplerate);
        adjustScaler();
    }
}

void BaseOscillator::setSamplerate(double Samplerate)
{
    if (dSamplerate != Samplerate)
    {
        dSamplerate = Samplerate;
        adjustScaler();
    }
}

void BaseOscillator::adjustScaler()
{
    dScaler = BaseWavetable::getInstance()->getWaveSize(lWaveform) / dSamplerate;
}

void BaseOscillator::setCoarse(double Coarse)
{
    dCoarse = ModuleHelper::clamp(Coarse, -BASEOSC_COARSE_RANGE, BASEOSC_COARSE_RANGE);
}

void BaseOscillator::setFinetune(double Finetune)
{
    dFinetune = ModuleHelper::clamp(Finetune, -BASEOSC_FINE_RANGE, BASEOSC_FINE_RANGE);
}

VstInt32 BaseOscillator::getNumWaveforms(void)
{
    return BaseWavetable::getInstance()->getNumLoadedWaveforms();
}

double BaseOscillator::Process(VstInt16 Note)
{
    // modulation in simetones
    double data, dBase, dTune, dFreq;

    data = BaseWavetable::getInstance()->getWaveData(lWaveform, dPhase);

    dBase = FrequencyTable::ForNote((Note + (VstInt16)(dCoarse)) & 0x7f);
    if (dFinetune >= 0.0)
    {
        dTune = (FrequencyTable::ForNote((Note + (VstInt16)(dCoarse)+1) & 0x7f) - dBase) * (((dFinetune + BASEOSC_FINE_RANGE) / (2 * BASEOSC_FINE_RANGE)) - 0.5) * 2;
        dFreq = (dBase + dTune) * dScaler;
    }
    else
    {
        dTune = (dBase - FrequencyTable::ForNote((Note + (VstInt16)(dCoarse)-1) & 0x7f)) * (0.5 - ((dFinetune + BASEOSC_FINE_RANGE) / (2 * BASEOSC_FINE_RANGE))) * 2;
        dFreq = (dBase - dTune) * dScaler;
    }
    dPhase += dFreq;
    dPhase = BaseWavetable::getInstance()->adjustPhase(lWaveform, dPhase);

    return data;
}

double BaseOscillator::processConnection(void)
{
    double input = 0.0, res;

    if (inputConnections[OSC_CONNECTION_NOTE] != NULL)
        input = inputConnections[OSC_CONNECTION_NOTE]->processConnection();
    if (inputConnections[OSC_CONNECTION_WAVEFORM] != NULL)
        setWaveform((VstInt16)inputConnections[OSC_CONNECTION_WAVEFORM]->processConnection());
    if (inputConnections[OSC_CONNECTION_COARSE] != NULL)
        setCoarse(inputConnections[OSC_CONNECTION_COARSE]->processConnection());
    if (inputConnections[OSC_CONNECTION_FINETUNE] != NULL)
        setFinetune(inputConnections[OSC_CONNECTION_FINETUNE]->processConnection());

    // ModuleLogger::print(LOG_CLASS_GENERATOR, "%s::processIOs C:%lf/F:%lf/W:%ld/I:%lf", getModuleName().c_str(), getCoarse(), getFinetune(), getWaveform(), input);
    res = Process((VstInt16)input);
    // ModuleLogger::print(LOG_CLASS_GENERATOR, "osc output: %lf", res);

    return res;
}
