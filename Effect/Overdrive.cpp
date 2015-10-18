#include <Effect/Overdrive.h>

using namespace eLibV2::Effect;
using namespace eLibV2::Util;

double Overdrive::Process(const double input)
{
    double ret = input * dLevel;
    ret = ModuleHelper::clamp(ret, -1.0, 1.0);

    return ret;
}

double Overdrive::processConnection(void)
{
    double dInput = 0.0, dOutput = 0.0;

    if (isInputConnected(OVERDRIVE_CONNECTION_BYPASS))
        setBypass(ModuleHelper::double2bool(inputConnections[OVERDRIVE_CONNECTION_BYPASS]->processConnection(), 0.5));
    if (isInputConnected(OVERDRIVE_CONNECTION_INPUT))
        dInput = inputConnections[OVERDRIVE_CONNECTION_INPUT]->processConnection();
    if (isInputConnected(OVERDRIVE_CONNECTION_LEVEL))
        setLevel(inputConnections[OVERDRIVE_CONNECTION_LEVEL]->processConnection());

    if (!m_bBypass)
        dOutput = Process(dInput);
    else
        dOutput = dInput;
    return dOutput;
}
