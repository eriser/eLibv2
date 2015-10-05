#include <Effect/Flanger.h>

using namespace eLibV2::Effect;

double Flanger::Process(const double Input)
{
    double res = 0.0;

    if (m_pDelay)
        res = m_pDelay->processConnection();

    return res;
}

double Flanger::processConnection()
{
    double input = 0.0, res;

    if (inputConnections[FLANGER_CONNECTION_LFOFREQ] != NULL)
    {
        if (m_pLFO)
            m_pLFO->setFreq(inputConnections[FLANGER_CONNECTION_LFOFREQ]->processConnection());
    }
    if (inputConnections[FLANGER_CONNECTION_INPUT] != NULL)
        input = inputConnections[FLANGER_CONNECTION_INPUT]->processConnection();

    res = Process(input);
    return res;
}

void Flanger::setSamplerate(double Samplerate)
{
    BaseModule::setSamplerate(Samplerate);
    if (m_pLFO)
        m_pLFO->setSamplerate(Samplerate);
    if (m_pDelay)
        m_pDelay->setSamplerate(Samplerate);
}

void Flanger::setLFOFreq(double Freq)
{
    if (m_pLFO)
        m_pLFO->setFreq(Freq);
}
