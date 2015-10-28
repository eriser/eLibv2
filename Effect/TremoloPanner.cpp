#include <Effect/TremoloPanner.h>

using namespace eLibV2::Effect;

void TremoloPanner::Init()
{
    setMode(TREMOLOPANNER_MODE_TREMOLO);
    setLFOFreq(1.0);
    setBypass(false);
    setModulationDepth(0.0);
    m_pOutputConnection->setInput(0.0);
}

double TremoloPanner::Process(const double Input1, const double Input2)
{
    double dOutput = Input1;

    double dLFOOutput = m_pLFO->Process(0);

    switch (m_eMode)
    {
        case TREMOLOPANNER_MODE_TREMOLO:
            dOutput = Input1 * (dLFOOutput * m_dModulationDepth + 1.0 - m_dModulationDepth);
            m_pOutputConnection->setInput(Input2 * (dLFOOutput * m_dModulationDepth + 1.0 - m_dModulationDepth));
            break;

        case TREMOLOPANNER_MODE_PANNER:
            double dPan = m_dModulationDepth * (dLFOOutput + 1.0) * PI_DIV_2 * 0.5;

            dOutput = Input1 * cos(dPan);
            m_pOutputConnection->setInput(Input2 * sin(dPan));
            break;
    }
    return dOutput;
}

double TremoloPanner::processConnection(void)
{
    double dInput1 = 0.0, dInput2 = 0.0, dOutput = 0.0;

    if (isInputConnected(TREMOLOPANNER_CONNECTION_BYPASS))
        setBypass(ModuleHelper::double2bool(inputConnections[TREMOLOPANNER_CONNECTION_BYPASS]->processConnection(), 0.5));
    if (isInputConnected(TREMOLOPANNER_CONNECTION_INPUT1))
        dInput1 = inputConnections[TREMOLOPANNER_CONNECTION_INPUT1]->processConnection();
    if (isInputConnected(TREMOLOPANNER_CONNECTION_INPUT2))
        dInput2 = inputConnections[TREMOLOPANNER_CONNECTION_INPUT2]->processConnection();
    if (isInputConnected(TREMOLOPANNER_CONNECTION_LFOFREQ))
        setLFOFreq(inputConnections[TREMOLOPANNER_CONNECTION_LFOFREQ]->processConnection());

    if (!m_bBypass)
        dOutput = Process(dInput1, dInput2);
    else
        dOutput = dInput1;
    return dOutput;
}

void eLibV2::Effect::TremoloPanner::setSamplerate(const double Samplerate)
{
    BaseModule::setSamplerate(Samplerate);
    if (m_pLFO)
        m_pLFO->setSamplerate(Samplerate);
}
