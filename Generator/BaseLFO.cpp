#include <Generator/BaseLFO.h>

using namespace eLibV2::Generator;

void BaseLFO::Init(void)
{
    m_dFreq = 1.0;
    m_lWaveform = 1;
}

void BaseLFO::adjustPhases(VstInt16 Note)
{
    double dFreq = m_dFreq * m_dScaler;
    m_dPhase += dFreq;
    m_dPhase = BaseWavetable::getInstance()->adjustPhase(m_lWaveform, m_dPhase);
    m_dQuadPhase = m_dPhase + BaseWavetable::getInstance()->getWaveSize(m_lWaveform) / QUADPHASE_DIVIDER;
}

double BaseLFO::processConnection()
{
    double dOutput = 0.0;

    if (isInputConnected(LFO_CONNECTION_FREQ))
        setFreq(inputConnections[LFO_CONNECTION_FREQ]->processConnection());
    if (isInputConnected(LFO_CONNECTION_WAVEFORM))
        setWaveform((VstInt16)inputConnections[LFO_CONNECTION_WAVEFORM]->processConnection());

    dOutput = Process(0);
    return dOutput;
}
