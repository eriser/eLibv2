#include <Effect/Overdrive.h>

using namespace eLibV2::Effect;
using namespace eLibV2::Util;

double FxOverdrive::Process(const double input)
{
    double ret = input * dLevel;
    ret = ModuleHelper::clamp(ret, -1.0, 1.0);

    return ret;
}

double FxOverdrive::processConnection()
{
    double input = 0.0;

    if (inputConnections[OVERDRIVE_CONNECTION_INPUT] != NULL)
        input = inputConnections[OVERDRIVE_CONNECTION_INPUT]->processConnection();
    if (inputConnections[OVERDRIVE_CONNECTION_LEVEL] != NULL)
        setLevel(inputConnections[OVERDRIVE_CONNECTION_LEVEL]->processConnection());
    ModuleLogger::print(LOG_CLASS_EFFECT, "%s::process %lf", getModuleName().c_str(), input);

    return Process(input);
}
