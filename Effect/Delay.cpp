#include <Effect/Delay.h>

// TODO: mac osx has also class delay
void eLibV2::Effect::Delay::Init(void)
{
    m_iWriteIndex = 0;
    m_iReadIndex = 0;
    setSyncTempo(false);
    setDelayLength(1000);
    setMixLevel(0.5);
    setFeedbackMode(FEEDBACK_MODE_INTERNAL);
    setFeedbackLevel(0.0);
    m_dInternalFeedback = 0.0;
    m_dExternalFeedback = 0.0;
}

void eLibV2::Effect::Delay::Reset(void)
{
    Init();
}

void eLibV2::Effect::Delay::adjustIndices(void)
{
    m_iReadIndex = ModuleHelper::wrap(m_iReadIndex, DELAY_BUFFER_SIZE - 1);
    m_iWriteIndex = ModuleHelper::wrap(m_iWriteIndex, DELAY_BUFFER_SIZE - 1);
}

void eLibV2::Effect::Delay::setDelayLength(const double Length)
{
    m_dDelayLength = Length;
    if (m_bSyncTempo)
    {
        // prevent out of bounds
        unsigned char syncIndex = ModuleHelper::clamp((unsigned char)((double)((Length)* 8.5)), 0, NUM_SYNC_FACTORS - 1);
        m_iDelayLengthInSamples = ModuleHelper::quarter2samples(m_dSyncFactors[syncIndex], getTempo(), getSamplerate());
    }
    else
        // length is in milliseconds
        m_iDelayLengthInSamples = Length;
//        m_iDelayLengthInSamples = (int)(((double)Length / 1000.0) * getSamplerate());

    m_iDelayLengthInSamples = ModuleHelper::clamp(m_iDelayLengthInSamples, 1, DELAY_BUFFER_SIZE - 1);
    m_iReadIndex = m_iWriteIndex - m_iDelayLengthInSamples;
    adjustIndices();
}

double eLibV2::Effect::Delay::Process(const double Input)
{
    double dOutput = 0.0, dFeedback = 0.0;

    // read old input and write new
    dOutput = m_pBuffer[m_iReadIndex] * m_dMixLevel + Input * (1.0 - m_dMixLevel);

    if (m_eFeedbackMode == FEEDBACK_MODE_INTERNAL)
        dFeedback = m_dInternalFeedback;
    else if (m_eFeedbackMode == FEEDBACK_MODE_EXTERNAL)
        dFeedback = m_dExternalFeedback;
    m_pBuffer[m_iWriteIndex++] = Input + dFeedback * m_dFeedbackLevel;

    m_dInternalFeedback = m_pBuffer[m_iReadIndex++];
    adjustIndices();

    return dOutput;
}

double eLibV2::Effect::Delay::processConnection(void)
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
