#include <Effect/Delay.h>

using namespace eLibV2::Effect;

void Delay::Init(void)
{
    m_iWriteIndex = 0;
    m_iReadIndex = 0;
    setDelayLength(1000);
    setMixLevel(0.5);
    setFeedbackMode(FEEDBACK_MODE_INTERNAL);
    setFeedbackLevel(0.0);
    m_dInternalFeedback = 0.0;
    m_dExternalFeedback = 0.0;
}

void Delay::Reset(void)
{
    Init();
}

void Delay::adjustIndices(void)
{
    m_iReadIndex = ModuleHelper::wrap(m_iReadIndex, DELAY_BUFFER_SIZE - 1);
    m_iWriteIndex = ModuleHelper::wrap(m_iWriteIndex, DELAY_BUFFER_SIZE - 1);
}

void Delay::setDelayLength(unsigned int length)
{
    unsigned int input = ModuleHelper::clamp(length, 1, DELAY_BUFFER_SIZE - 1);
    if (m_iDelayLength != input)
    {
        m_iDelayLength = input;
        m_iReadIndex = m_iWriteIndex - m_iDelayLength;
        adjustIndices();
    }
}

double Delay::Process(const double Input)
{
    double dOutput = 0.0, dFeedback = 0.0;

    // read old input and write new
    dOutput = m_pBuffer[m_iReadIndex] * m_dMixLevel + Input * (1.0 - m_dMixLevel);

    if (m_eFeedbackMode == FEEDBACK_MODE_INTERNAL)
        dFeedback = m_dInternalFeedback;
    else if (m_eFeedbackMode == FEEDBACK_MODE_EXTERNAL)
        dFeedback = m_dExternalFeedback;
    m_pBuffer[m_iWriteIndex++] = Input + dFeedback;

    m_dInternalFeedback = m_pBuffer[m_iReadIndex++] * m_dFeedbackLevel;
    adjustIndices();

    return dOutput;
}

double Delay::processConnection(void)
{
    double dInput = 0.0, dOutput = 0.0;

    if (isInputConnected(DELAY_CONNECTION_BYPASS))
        setBypass(ModuleHelper::double2bool(inputConnections[DELAY_CONNECTION_BYPASS]->processConnection(), 0.5));
    if (isInputConnected(DELAY_CONNECTION_INPUT))
        dInput = inputConnections[DELAY_CONNECTION_INPUT]->processConnection();
    if (isInputConnected(DELAY_CONNECTION_FEEDBACK))
        m_dExternalFeedback = inputConnections[DELAY_CONNECTION_FEEDBACK]->processConnection();
    if (isInputConnected(DELAY_CONNECTION_LENGTH))
        setDelayLength(inputConnections[DELAY_CONNECTION_LENGTH]->processConnection());

    if (!m_bBypass)
        dOutput = Process(dInput);
    else
        dOutput = dInput;
    return dOutput;
}
