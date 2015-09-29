#include <Effect/Delay.h>

using namespace eLibV2::Effect;

void Delay::Init(void)
{
    mixLevel = 0.5;
    readIndex = 0;
    writeIndex = DELAY_BUFFER_SIZE / 2;
}

void Delay::Reset(void)
{
    Init();
}

void Delay::adjustIndices(void)
{
    if (readIndex > DELAY_BUFFER_SIZE)
        readIndex = 0;
    if (writeIndex > DELAY_BUFFER_SIZE)
        writeIndex = 0;
}

double Delay::Process(const double Input)
{
    double output = Input;

    // write new input and read old
    buffer[writeIndex++] = Input;
    output += buffer[readIndex++] * mixLevel;
    adjustIndices();

    return output;
}

double Delay::processConnection()
{
    double input = 0.0;

    if (inputConnections[DELAY_CONNECTION_INPUT])
        input = inputConnections[DELAY_CONNECTION_INPUT]->processConnection();
    return Process(input);
}
