#include <Effect/modOverdrive.h>

using namespace eLibV2;
using namespace eLibV2::Effect;

double FxOverdrive::Process(double input)
{
    double ret = input * dLevel;
    ret = ModuleHelper::clamp(ret, -1.0, 1.0);

    return ret;
}

double FxOverdrive::processConnection()
{
    double input = 0.0;

    if (isConnected(CONNECTION_OVERDRIVE_LEVEL))
        setLevel(connections[CONNECTION_OVERDRIVE_LEVEL]->processConnection());
    if (isConnected(CONNECTION_OVERDRIVE_INPUT))
        input = connections[CONNECTION_OVERDRIVE_INPUT]->processConnection();
    ModuleLogger::print("%s::process %lf", getModuleName().c_str(), input);

    return Process(input);
}
