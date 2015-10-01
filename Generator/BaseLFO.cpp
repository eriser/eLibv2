#include <Generator/BaseLFO.h>

using namespace eLibV2::Generator;

void BaseLFO::Init(void)
{
    m_dFreq = 1.0;
    m_lWaveform = 1;
    adjustScaler();
}

void BaseLFO::Reset(void)
{
    Init();
}

double BaseLFO::Process(void)
{
    // modulation in simetones
    double data, dBase, dTune, dFreq;

    data = BaseWavetable::getInstance()->getWaveData(m_lWaveform, m_dPhase);

    dFreq = m_dFreq * m_dScaler;
    m_dPhase += dFreq;
    m_dPhase = BaseWavetable::getInstance()->adjustPhase(m_lWaveform, m_dPhase);

    return data;
}

double BaseLFO::processConnection()
{
    double input = 0.0, res;

    if (inputConnections[LFO_CONNECTION_FREQ] != NULL)
        setFreq(inputConnections[LFO_CONNECTION_FREQ]->processConnection());
    if (inputConnections[LFO_CONNECTION_WAVEFORM] != NULL)
        setWaveform((VstInt16)inputConnections[LFO_CONNECTION_WAVEFORM]->processConnection());

    res = Process();
    return res;
}
