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
    double dInput = 0.0, dOutput = 0.0;

    if (isInputConnected(DELAY_CONNECTION_BYPASS))
        setBypass(ModuleHelper::double2bool(inputConnections[DELAY_CONNECTION_BYPASS]->processConnection(), 0.5));
    if (isInputConnected(DELAY_CONNECTION_INPUT))
        dInput = inputConnections[DELAY_CONNECTION_INPUT]->processConnection();
    if (isInputConnected(DELAY_CONNECTION_LENGTH))
        setDelayLength(inputConnections[DELAY_CONNECTION_LENGTH]->processConnection());

    if (!m_bBypass)
        dOutput = Process(dInput);
    else
        dOutput = dInput;
    return dOutput;
}
