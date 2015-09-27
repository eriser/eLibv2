#include <Effect/SelectorBlock.h>

using namespace eLibV2::Effect;

void SelectorBlock::Reset(void)
{
    Init();
}

double SelectorBlock::processConnection()
{
    double input = 0.0;

    if ((uiInputSwitch < CONNECTION_SELECTOR_MAXINPUT) && (inputConnections[uiInputSwitch] != NULL))
        input = inputConnections[uiInputSwitch]->processConnection();
    // ModuleLogger::print(LOG_CLASS_EFFECT, "%s::process %lf", getModuleName().c_str(), input);

    return input;
}
