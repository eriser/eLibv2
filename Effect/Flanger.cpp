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
    double dInput = 0.0, dOutput = 0.0;

    if (inputConnections[FLANGER_CONNECTION_BYPASS] != NULL)
        m_bBypass = ModuleHelper::double2bool(inputConnections[FLANGER_CONNECTION_BYPASS]->processConnection(), 0.5);
    if (inputConnections[FLANGER_CONNECTION_INPUT] != NULL)
        dInput = inputConnections[FLANGER_CONNECTION_INPUT]->processConnection();
    if (inputConnections[FLANGER_CONNECTION_LFOFREQ] != NULL)
    {
        if (m_pLFO)
            m_pLFO->setFreq(inputConnections[FLANGER_CONNECTION_LFOFREQ]->processConnection());
    }

    if (!m_bBypass)
        dOutput = Process(dInput);
    else
        dOutput = dInput;
    return dOutput;
}

void Flanger::setSamplerate(const double Samplerate)
{
    BaseModule::setSamplerate(Samplerate);
    if (m_pLFO)
        m_pLFO->setSamplerate(Samplerate);
    if (m_pDelay)
        m_pDelay->setSamplerate(Samplerate);
}

void Flanger::setLFOFreq(const double Freq)
{
    if (m_pLFO)
        m_pLFO->setFreq(Freq);
}
