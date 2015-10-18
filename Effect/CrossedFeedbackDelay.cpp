#include <Effect/CrossedFeedbackDelay.h>

using namespace eLibV2::Effect;

void CrossedFeedbackDelay::Init()
{
    setDelayMode(DELAY_MODE_NORMAL);
}

void CrossedFeedbackDelay::setSamplerate(const double Samplerate)
{
    BaseModule::setSamplerate(Samplerate);
    if (m_pDelayLeft)
        m_pDelayLeft->setSamplerate(Samplerate);
    if (m_pDelayRight)
        m_pDelayRight->setSamplerate(Samplerate);
}

void CrossedFeedbackDelay::setDelayMode(const DelayMode delayMode)
{
    m_eDelayMode = delayMode;

    switch (m_eDelayMode)
    {
        case DELAY_MODE_NORMAL:
            m_pDelayLeft->setFeedbackMode(Delay::FEEDBACK_MODE_INTERNAL);
            m_pDelayRight->setFeedbackMode(Delay::FEEDBACK_MODE_INTERNAL);
            break;

        case DELAY_MODE_CROSSED:
            m_pDelayLeft->setFeedbackMode(Delay::FEEDBACK_MODE_EXTERNAL);
            m_pDelayRight->setFeedbackMode(Delay::FEEDBACK_MODE_EXTERNAL);
            break;
    }
}

void CrossedFeedbackDelay::Process(const double input1, const double input2, double * output1, double * output2)
{
    if (!m_bBypass)
    {
        if (output1 && output2)
        {
            *output1 = *output2 = 0.0;

            if (m_eDelayMode == DELAY_MODE_CROSSED)
            {
                m_pDelayLeft->setFeedback(m_pDelayRight->getFeedback());
                m_pDelayRight->setFeedback(m_pDelayLeft->getFeedback());
            }

            *output1 = m_pDelayLeft->Process(input1);
            *output2 = m_pDelayRight->Process(input2);
        }
    }
}
