#include <Effect/WaveShaper.h>

using namespace eLibV2::Effect;

void WaveShaper::Init()
{
    setPositiveExponent(1.0);
    setNegativeExponent(1.0);
    setNumStages(1);
    setInvertStages(false);
}

double WaveShaper::Process(const double Input)
{
    double dOutput = Input;

    for (unsigned char stageIndex = 0; stageIndex < m_iNumStages; ++stageIndex)
    {
        if (dOutput >= 0.0)
            dOutput = (1.0 / atan(m_dPositiveExponent)) * atan(m_dPositiveExponent * dOutput);
        else
            dOutput = (1.0 / atan(m_dNegativeExponent)) * atan(m_dNegativeExponent * dOutput);

        if ((m_bInvertStages) && ((stageIndex % 2) == 0))
            dOutput *= -1.0;
    }
    return dOutput;
}

double WaveShaper::processConnection(void)
{
    double dInput = 0.0, dOutput = 0.0;

    if (isInputConnected(WAVESHAPER_CONNECTION_BYPASS))
        setBypass(ModuleHelper::double2bool(inputConnections[WAVESHAPER_CONNECTION_BYPASS]->processConnection(), 0.5));
    if (isInputConnected(WAVESHAPER_CONNECTION_INPUT))
        dInput = inputConnections[WAVESHAPER_CONNECTION_INPUT]->processConnection();

    if (!m_bBypass)
        dOutput = Process(dInput);
    else
        dOutput = dInput;
    return dOutput;
}
