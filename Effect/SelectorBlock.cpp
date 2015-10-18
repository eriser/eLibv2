#include <Effect/SelectorBlock.h>

using namespace eLibV2::Effect;

void SelectorBlock::Reset(void)
{
    Init();
}

double SelectorBlock::processConnection(void)
{
    double input = 0.0;

    if ((uiInputSwitch < CONNECTION_SELECTOR_MAXINPUT) && (isInputConnected(uiInputSwitch)))
        input = inputConnections[uiInputSwitch]->processConnection();
    return input;
}
