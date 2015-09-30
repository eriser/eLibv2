#include <Effect/Delay.h>

using namespace eLibV2::Effect;

void Delay::Init(void)
{
    readIndex = 0;
    writeIndex = delayLength;
}

void Delay::Reset(void)
{
    Init();
}

void Delay::adjustIndices(void)
{
    readIndex = ModuleHelper::wrap(readIndex, DELAY_BUFFER_SIZE - 1);
    writeIndex = ModuleHelper::wrap(writeIndex, DELAY_BUFFER_SIZE - 1);
}

void Delay::setDelayLength(unsigned int length)
{
    unsigned int input = ModuleHelper::clamp(length, 1, DELAY_BUFFER_SIZE - 1);
    if (delayLength != input)
    {
        delayLength = input;
        readIndex = writeIndex - delayLength;
        adjustIndices();
    }
}

double Delay::Process(const double Input)
{
    double output = Input;

    // read old input and write new
    output += buffer[readIndex++] * mixLevel;
    buffer[writeIndex++] = Input;
    adjustIndices();

    return output;
}

double Delay::processConnection()
{
    double input = 0.0;

    if (inputConnections[DELAY_CONNECTION_LENGTH] != NULL)
        setDelayLength(inputConnections[DELAY_CONNECTION_LENGTH]->processConnection());
    if (inputConnections[DELAY_CONNECTION_INPUT] != NULL)
        input = inputConnections[DELAY_CONNECTION_INPUT]->processConnection();
    return Process(input);
}
