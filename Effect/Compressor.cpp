#include <Effect/Compressor.h>

using namespace eLibV2::Effect;

void Compressor::Init()
{
    // envelope-detector is set to default on construction
    setBypass(false);
    setThreshold(0.0);
    setRatio(1.0);
    setKneeWidth(0.0);
    setInputGain(1.0);
    setOutputGain(1.0);
}

double Compressor::Process(const double Input)
{
    double dOutput = 0.0;

    // get current detector-input, convert to dB, calc gain-factor, back to linear and scale
    if (m_pEnvelopeDetector)
    {
        double dDetectedValueIndB = ModuleHelper::value2dB(m_pEnvelopeDetector->Process(Input * m_dInputGain));

        // compressor slope
        double CS = 1.0 - (1.0 / m_dRatio);

        CS = 1.0;

        // within knee-zone?
        if (
            (m_dKneeWidth > 0.0) &&
            (dDetectedValueIndB > (m_dThreshold - m_dKneeWidth / 2.0)) &&
            (dDetectedValueIndB < (m_dThreshold + m_dKneeWidth / 2.0))
            )
        {
            double x[2], y[2];
            x[0] = m_dThreshold - m_dKneeWidth / 2.0;
            x[1] = ModuleHelper::minval(m_dThreshold + m_dKneeWidth / 2.0, 0.0);
            y[0] = 0;
            y[1] = CS;

            CS = ModuleHelper::LagrangeInterpolation(x, y, 2, dDetectedValueIndB);
        }

        double dGain = ModuleHelper::minval(CS * (m_dThreshold - dDetectedValueIndB), 0.0);
        dGain = ModuleHelper::dB2value(dGain);
        dOutput = Input * dGain * m_dOutputGain;
    }
    return dOutput;
}

double Compressor::processConnection()
{
    double dInput = 0.0, dOutput = 0.0;

    if (isInputConnected(COMPRESSOR_CONNECTION_BYPASS))
        setBypass(ModuleHelper::double2bool(inputConnections[COMPRESSOR_CONNECTION_BYPASS]->processConnection(), 0.5));
    if (isInputConnected(COMPRESSOR_CONNECTION_INPUT))
        dInput = inputConnections[COMPRESSOR_CONNECTION_INPUT]->processConnection();
    if (isInputConnected(COMPRESSOR_CONNECTION_ATTACK))
        setAttack(inputConnections[COMPRESSOR_CONNECTION_ATTACK]->processConnection());
    if (isInputConnected(COMPRESSOR_CONNECTION_RELEASE))
        setRelease(inputConnections[COMPRESSOR_CONNECTION_RELEASE]->processConnection());
    if (isInputConnected(COMPRESSOR_CONNECTION_THRESHOLD))
        setThreshold(inputConnections[COMPRESSOR_CONNECTION_THRESHOLD]->processConnection());
    if (isInputConnected(COMPRESSOR_CONNECTION_RATIO))
        setRatio(inputConnections[COMPRESSOR_CONNECTION_RATIO]->processConnection());
    if (isInputConnected(COMPRESSOR_CONNECTION_KNEEWIDTH))
        setKneeWidth(inputConnections[COMPRESSOR_CONNECTION_KNEEWIDTH]->processConnection());
    if (isInputConnected(COMPRESSOR_CONNECTION_INPUTGAIN))
        setInputGain(inputConnections[COMPRESSOR_CONNECTION_RATIO]->processConnection());
    if (isInputConnected(COMPRESSOR_CONNECTION_OUTPUTGAIN))
        setOutputGain(inputConnections[COMPRESSOR_CONNECTION_OUTPUTGAIN]->processConnection());

    if (!m_bBypass)
        dOutput = Process(dInput);
    else
        dOutput = dInput;
    return dOutput;
}
