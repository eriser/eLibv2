#include <Effect/Phaser.h>

using namespace eLibV2::Effect;

double Phaser::Process(const double Input)
{
    double res = 0.0;

    if (m_pDelay)
        res = m_pDelay->processConnection();

    return res;
}

double Phaser::processConnection()
{
    double input = 0.0, res;

    if (inputConnections[PHASER_CONNECTION_LFOFREQ] != NULL)
    {
        if (m_pLFO)
            m_pLFO->setFreq(inputConnections[PHASER_CONNECTION_LFOFREQ]->processConnection());
    }
    if (inputConnections[PHASER_CONNECTION_INPUT] != NULL)
        input = inputConnections[PHASER_CONNECTION_INPUT]->processConnection();

    res = Process(input);
    return res;
}

void eLibV2::Effect::Phaser::setSamplerate(double Samplerate)
{
    BaseModule::setSamplerate(Samplerate);
    if (m_pLFO)
        m_pLFO->setSamplerate(Samplerate);
    if (m_pDelay)
        m_pDelay->setSamplerate(Samplerate);
}

void eLibV2::Effect::Phaser::setLFOFreq(double Freq)
{
    if (m_pLFO)
        m_pLFO->setFreq(Freq);
}
