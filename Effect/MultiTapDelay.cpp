#include <Effect/MultiTapDelay.h>

using namespace eLibV2::Effect;

void MultiTapDelay::Init(void)
{
    m_iWriteIndex = 0;
    for (UInt8 readIndex = 0; readIndex < 4; ++readIndex)
        m_iReadIndex[readIndex] = 0;
    setDelayLength(1000);
    setMixLevel(0.5);
    setFeedbackLevel(0.0);
    m_dInternalFeedback = 0.0;
}

void MultiTapDelay::Reset(void)
{
    Init();
}

void MultiTapDelay::adjustIndices(void)
{
    for (UInt8 readIndex = 0; readIndex < 4; ++readIndex)
        m_iReadIndex[readIndex] = ModuleHelper::wrap(m_iReadIndex[readIndex], DELAY_BUFFER_SIZE - 1);
    m_iWriteIndex = ModuleHelper::wrap(m_iWriteIndex, DELAY_BUFFER_SIZE - 1);
}

void MultiTapDelay::setDelayLength(UInt16 length)
{
    UInt16 input = ModuleHelper::clamp(length, 1, DELAY_BUFFER_SIZE - 1);
    if (m_iDelayLength != input)
    {
        m_iDelayLength = input;
        m_iReadIndex[0] = m_iWriteIndex - m_iDelayLength;
        m_iReadIndex[1] = m_iWriteIndex - (m_iDelayLength / 4);
        m_iReadIndex[2] = m_iWriteIndex - (m_iDelayLength / 2);
        m_iReadIndex[3] = m_iWriteIndex - (m_iDelayLength * 3 / 4);
        adjustIndices();
    }
}

double MultiTapDelay::Process(const double Input)
{
    double dOutput = 0.0, dFeedback = 0.0;

    // read old input and write new
    dOutput = 0.5 * m_pBuffer[m_iReadIndex[0]] * m_dMixLevel + Input * (1.0 - m_dMixLevel);
    dOutput += 0.25 * m_pBuffer[m_iReadIndex[1]++] + 0.125 * m_pBuffer[m_iReadIndex[2]++] + 0.0625 * m_pBuffer[m_iReadIndex[3]++];
    dFeedback = m_dInternalFeedback;
    m_pBuffer[m_iWriteIndex++] = Input + dFeedback;

    m_dInternalFeedback = m_pBuffer[m_iReadIndex[0]++] * m_dFeedbackLevel;
    adjustIndices();

    return dOutput;
}

double MultiTapDelay::processConnection(void)
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
