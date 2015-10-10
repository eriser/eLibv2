#include <Generator/BaseOsc.h>

using namespace eLibV2::Generator;
using namespace eLibV2::Util;

void BaseOscillator::Init(void)
{
    setWaveform(1);
    setCoarse(0.0);
    setFinetune(0.0);
    m_dPhase = 0.0;

    FrequencyTable::SetupFreqs();
}

void BaseOscillator::Reset(void)
{
    // reset phase to begin at start of waveform
    m_dPhase = 0.0;
}

void BaseOscillator::setWaveform(VstInt32 Waveform)
{
    if (m_lWaveform != Waveform)
    {
        m_lWaveform = Waveform;
        adjustScaler();
    }
}

void BaseOscillator::setSamplerate(const double Samplerate)
{
    BaseModule::setSamplerate(Samplerate);
    adjustScaler();
}

void BaseOscillator::adjustScaler()
{
    m_dScaler = BaseWavetable::getInstance()->getWaveSize(m_lWaveform) / mSamplerate;
}

void BaseOscillator::setCoarse(double Coarse)
{
    m_dCoarse = ModuleHelper::clamp(Coarse, -BASEOSC_COARSE_RANGE, BASEOSC_COARSE_RANGE);
}

void BaseOscillator::setFinetune(double Finetune)
{
    m_dFinetune = ModuleHelper::clamp(Finetune, -BASEOSC_FINE_RANGE, BASEOSC_FINE_RANGE);
}

VstInt32 BaseOscillator::getNumWaveforms(void)
{
    return BaseWavetable::getInstance()->getNumLoadedWaveforms();
}

double BaseOscillator::Process(VstInt16 Note)
{
    // modulation in simetones
    double wavedata, dCoarseFreq, dFineFreq, dFreq;

    wavedata = BaseWavetable::getInstance()->getWaveData(m_lWaveform, m_dPhase);

    dCoarseFreq = FrequencyTable::ForNote((Note + (VstInt16)(m_dCoarse)) & 0x7f);
    if (m_dFinetune >= 0.0)
    {
        dFineFreq = (FrequencyTable::ForNote((Note + (VstInt16)(m_dCoarse)+1) & 0x7f) - dCoarseFreq) * (((m_dFinetune + BASEOSC_FINE_RANGE) / (2 * BASEOSC_FINE_RANGE)) - 0.5) * 2;
        dFreq = (dCoarseFreq + dFineFreq) * m_dScaler;
    }
    else
    {
        dFineFreq = (dCoarseFreq - FrequencyTable::ForNote((Note + (VstInt16)(m_dCoarse)-1) & 0x7f)) * (0.5 - ((m_dFinetune + BASEOSC_FINE_RANGE) / (2 * BASEOSC_FINE_RANGE))) * 2;
        dFreq = (dCoarseFreq - dFineFreq) * m_dScaler;
    }
    m_dPhase += dFreq;
    m_dPhase = BaseWavetable::getInstance()->adjustPhase(m_lWaveform, m_dPhase);

    return wavedata;
}

double BaseOscillator::processConnection(void)
{
    double dInput = 0.0, res;

    if (isInputConnected(OSC_CONNECTION_NOTE))
        dInput = inputConnections[OSC_CONNECTION_NOTE]->processConnection();
    if (isInputConnected(OSC_CONNECTION_WAVEFORM))
        setWaveform((VstInt16)inputConnections[OSC_CONNECTION_WAVEFORM]->processConnection());
    if (isInputConnected(OSC_CONNECTION_COARSE))
        setCoarse(inputConnections[OSC_CONNECTION_COARSE]->processConnection());
    if (isInputConnected(OSC_CONNECTION_FINETUNE))
        setFinetune(inputConnections[OSC_CONNECTION_FINETUNE]->processConnection());

    // ModuleLogger::print(LOG_CLASS_GENERATOR, "%s::processIOs C:%lf/F:%lf/W:%ld/I:%lf", getModuleName().c_str(), getCoarse(), getFinetune(), getWaveform(), input);
    res = Process((VstInt16)dInput);
    // ModuleLogger::print(LOG_CLASS_GENERATOR, "osc output: %lf", res);

    return res;
}
