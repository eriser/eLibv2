#include <Effect/Selector.h>

using namespace eLibV2::Effect;

void Selector::Init(void)
{
    setInputSwitch(0);
}

void Selector::Reset(void)
{
    Init();
}

double Selector::processConnection()
{
    double input = 0.0;

    if ((uiInputSwitch < CONNECTION_SELECTOR_MAXINPUT) && (isConnected(uiInputSwitch)))
        input = connections[uiInputSwitch]->processConnection();
    ModuleLogger::print(LOG_CLASS_EFFECT, "%s::process %lf", getModuleName().c_str(), input);

    return input;
}

