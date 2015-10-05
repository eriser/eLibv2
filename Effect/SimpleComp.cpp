#include <Effect/SimpleComp.h>

using namespace eLibV2::Effect;

void SimpleComp::Init()
{
    setThreshold(0.5);
    setAttack(0.01);
    setRelease(0.01);
    setRatio(0.5);
    setOutput(0.707);

    m_dEnvDecay = 0.0;

    m_dTransferA = 0.0;
    m_dTransferB = 1.0;

    Reset();
}

void SimpleComp::Reset()
{
    m_dEnv = 0.0;
    m_dGain = 1.0;
}

void SimpleComp::setThreshold(double Threshold)
{
    m_dThreshold = Threshold;
    m_dTransferB = m_dOutput * pow(m_dThreshold, -m_dTransferA);
}

void SimpleComp::setRatio(double Ratio)
{
    m_dRatio = Ratio;
    m_dTransferA = Ratio - 1.0;
    m_dTransferB = m_dOutput * pow(m_dThreshold, -m_dTransferA);
}

void SimpleComp::setAttack(double Attack)
{
    m_dAttack = exp(-1.0 / Attack);
}

void SimpleComp::setRelease(double Release)
{
    m_dRelease = exp(-1.f / Release);
    m_dEnvDecay = exp(-4.f / Release); /* = exp(-1/(0.25*value)) */
}

void SimpleComp::setOutput(double Output)
{
    m_dOutput = Output;
    m_dTransferB = m_dOutput * pow(m_dThreshold, -m_dTransferA);
}

double SimpleComp::Process(const double Input)
{
    double det, transfer_gain, res = 0.0;

    det = fabs(Input);
    det += 10e-30f; /* add tiny DC offset (-600dB) to prevent denormals */
    m_dEnv = (det >= m_dEnv) ? det : det + m_dEnvDecay * (m_dEnv - det);
    transfer_gain = (m_dEnv > m_dThreshold) ? pow(m_dEnv, m_dTransferA) * m_dTransferB : m_dOutput;
    m_dGain = (transfer_gain < m_dGain) ? transfer_gain + m_dAttack * (m_dGain - transfer_gain) : transfer_gain + m_dRelease * (m_dGain - transfer_gain);
    res = Input * m_dGain;

    return res;
}

double SimpleComp::processConnection()
{
    double input = 0.0;

    //if (inputConnections[SIMPLECOMP_CONNECTION_BYPASS] != NULL)
    //    m_dAttack = inputConnections[SIMPLECOMP_CONNECTION_BYPASS]->processConnection();
    if (inputConnections[SIMPLECOMP_CONNECTION_INPUT] != NULL)
        input = inputConnections[SIMPLECOMP_CONNECTION_INPUT]->processConnection();
    if (inputConnections[SIMPLECOMP_CONNECTION_ATTACK] != NULL)
        m_dAttack = inputConnections[SIMPLECOMP_CONNECTION_ATTACK]->processConnection();
    if (inputConnections[SIMPLECOMP_CONNECTION_RELEASE] != NULL)
        m_dRelease = inputConnections[SIMPLECOMP_CONNECTION_RELEASE]->processConnection();
    if (inputConnections[SIMPLECOMP_CONNECTION_THRESHOLD] != NULL)
        m_dThreshold = inputConnections[SIMPLECOMP_CONNECTION_THRESHOLD]->processConnection();
    if (inputConnections[SIMPLECOMP_CONNECTION_RATIO] != NULL)
        m_dRatio = inputConnections[SIMPLECOMP_CONNECTION_RATIO]->processConnection();
    if (inputConnections[SIMPLECOMP_CONNECTION_GAIN] != NULL)
        m_dGain = inputConnections[SIMPLECOMP_CONNECTION_GAIN]->processConnection();

    return Process(input);
}
