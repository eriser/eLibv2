#include <Effect/DynamicsProcessor.h>

using namespace eLibV2::Effect;

void DynamicsProcessor::Init()
{
    // envelope-detector is set to default on construction
    setBypass(false);
    setThreshold(0.0);
    setRatio(1.0);
    setKneeWidth(0.0);
    setInputGain(0.0);
    setOutputGain(0.0);
}

double DynamicsProcessor::Process(const double Input)
{
    double dOutput = 0.0;

    // get current detector-input, convert to dB, calc gain-factor, back to linear and scale
    if (m_pEnvelopeDetector)
    {
        double dInputGain = ModuleHelper::dB2value(m_dInputGainIndB);
        double dDetectedValueIndB = ModuleHelper::value2dB(m_pEnvelopeDetector->Process(Input * dInputGain));
        double dSlope = 0.0;

        // get slope depending on mode
        switch (m_eMode)
        {
            case DYNAMICS_MODE_COMPRESSOR:
                dSlope = 1.0 - (1.0 / m_dRatio);
                break;

            case DYNAMICS_MODE_LIMITER:
                dSlope = 1.0;
                break;

            case DYNAMICS_MODE_EXPANDER:
                dSlope = 1.0 / m_dRatio - 1.0;
                break;

            case DYNAMICS_MODE_GATE:
                dSlope = -1.0;
                break;
        }

        // within knee-zone?
        if (
            (m_dKneeWidth > 0.0) &&
            (dDetectedValueIndB > (m_dThreshold - m_dKneeWidth / 2.0)) &&
            (dDetectedValueIndB < (m_dThreshold + m_dKneeWidth / 2.0))
            )
        {
            double x[2], y[2];
            x[0] = m_dThreshold - m_dKneeWidth / 2.0;
            x[1] = min(0.0, m_dThreshold + m_dKneeWidth / 2.0);
            y[0] = 0;
            y[1] = dSlope;

            dSlope = ModuleHelper::LagrangeInterpolation(x, y, 2, dDetectedValueIndB);
        }

        double dGain = min(0.0, dSlope * (m_dThreshold - dDetectedValueIndB));
        dGain = ModuleHelper::dB2value(dGain);

        double dOutputGain = ModuleHelper::dB2value(m_dOutputGainIndB);
        dOutput = Input * dGain * dOutputGain;
    }
    return dOutput;
}

double DynamicsProcessor::processConnection()
{
    double dInput = 0.0, dOutput = 0.0;

    if (isInputConnected(DYNAMICS_CONNECTION_BYPASS))
        setBypass(ModuleHelper::double2bool(inputConnections[DYNAMICS_CONNECTION_BYPASS]->processConnection(), 0.5));
    if (isInputConnected(DYNAMICS_CONNECTION_INPUT))
        dInput = inputConnections[DYNAMICS_CONNECTION_INPUT]->processConnection();
    if (isInputConnected(DYNAMICS_CONNECTION_ATTACK))
        setAttack(inputConnections[DYNAMICS_CONNECTION_ATTACK]->processConnection());
    if (isInputConnected(DYNAMICS_CONNECTION_RELEASE))
        setRelease(inputConnections[DYNAMICS_CONNECTION_RELEASE]->processConnection());
    if (isInputConnected(DYNAMICS_CONNECTION_THRESHOLD))
        setThreshold(inputConnections[DYNAMICS_CONNECTION_THRESHOLD]->processConnection());
    if (isInputConnected(DYNAMICS_CONNECTION_RATIO))
        setRatio(inputConnections[DYNAMICS_CONNECTION_RATIO]->processConnection());
    if (isInputConnected(DYNAMICS_CONNECTION_KNEEWIDTH))
        setKneeWidth(inputConnections[DYNAMICS_CONNECTION_KNEEWIDTH]->processConnection());
    if (isInputConnected(DYNAMICS_CONNECTION_INPUTGAIN))
        setInputGain(inputConnections[DYNAMICS_CONNECTION_RATIO]->processConnection());
    if (isInputConnected(DYNAMICS_CONNECTION_OUTPUTGAIN))
        setOutputGain(inputConnections[DYNAMICS_CONNECTION_OUTPUTGAIN]->processConnection());

    if (!m_bBypass)
        dOutput = Process(dInput);
    else
        dOutput = dInput;
    return dOutput;
}
