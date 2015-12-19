#include <Envelope/AR.h>

using namespace eLibV2::Envelope;

void AR::Init(void)
{
    setMode(MODE_ANALOG);
    setAttack(1000.0);
    setRelease(1000.0);
    setSamplerate(44100.0);
    setActive(true);

    m_dCurrentOutput = 0.0;
    m_dIncShutdown = 0.0;
    m_dShutdownTime_mSec = 10.0;

    m_eState = STATE_OFF;

    m_bResetToZero = true;
    m_bLegatoMode = false;
}

void AR::Reset(void)
{
    m_eState = STATE_OFF;

    if (m_bResetToZero)
        m_dCurrentOutput = 0.0;
}

void AR::setMode(const EnvelopeMode Mode)
{
    BaseEnvelope::setMode(Mode);

    switch (Mode)
    {
        case MODE_UNDEF:
            break;

        case MODE_ANALOG:
            m_dAttackTCO = exp(-1.5);
            m_dReleaseTCO = exp(-4.95);
            break;

        case MODE_DIGITAL:
            m_dAttackTCO = 0.99999;
            m_dReleaseTCO = exp(-11.05);
            break;
    }
}

void AR::start(void)
{
    if (m_bLegatoMode && (m_eState != STATE_OFF) && (m_eState != STATE_RELEASE))
        return;

    Reset();
    m_eState = STATE_ATTACK;
}

void AR::stop(void)
{
    if (m_dCurrentOutput > 0.0)
        m_eState = STATE_RELEASE;
    else
        m_eState = STATE_OFF;
}

void AR::shutdown(void)
{
    if (!m_bLegatoMode)
    {
        m_dIncShutdown = -(1000.0 * m_dCurrentOutput) / (m_dShutdownTime_mSec * m_dSamplerate);
        m_eState = STATE_SHUTDOWN;
    }
}

double AR::Process(void)
{
    if (m_bActive)
    {
        switch (m_eState)
        {
            case STATE_OFF:
                if (m_bResetToZero)
                    m_dCurrentOutput = 0.0;
                break;

            case STATE_ATTACK:
                m_dCurrentOutput = m_dAttackOffset + m_dCurrentOutput * m_dAttackCoeff;

                if (
                    (m_dCurrentOutput >= 1.0) ||
                    (m_dAttackTime_mSec <= 0.0)
                    )
                {
                    m_dCurrentOutput = 1.0;
                    m_eState = STATE_RELEASE;
                }
                break;

            case STATE_RELEASE:
                m_dCurrentOutput = m_dReleaseOffset + m_dCurrentOutput * m_dReleaseCoeff;

                if (
                    (m_dCurrentOutput <= 0.0) ||
                    (m_dReleaseTime_mSec <= 0.0)
                    )
                {
                    m_dCurrentOutput = 0.0;
                    m_eState = STATE_OFF;
                }
                break;

            case STATE_SHUTDOWN:
                if (m_bResetToZero)
                {
                    m_dCurrentOutput += m_dIncShutdown;
                    if (m_dCurrentOutput <= 0.0)
                    {
                        m_dCurrentOutput = 0.0;
                        m_eState = STATE_OFF;
                    }
                }
                else
                {
                    m_eState = STATE_OFF;
                }
                break;
                
            case STATE_DELAY:
            case STATE_HOLD:
            case STATE_DECAY:
            case STATE_SUSTAIN:
                // these states are not supported by class
                break;
        }
    }
    else
    {
        // not active means always output 1.0
        m_dCurrentOutput = 1.0;
    }
    return m_dCurrentOutput;
}
