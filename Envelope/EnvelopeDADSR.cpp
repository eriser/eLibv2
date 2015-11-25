#include <Envelope/EnvelopeDADSR.h>

using namespace eLibV2::Envelope;

void EnvelopeDADSR::Init(void)
{
    setDelay(0.25);
    setDelayScale(1.0);
    setAttack(0.5);
    setAttackScale(1.0);
    setDecay(0.5);
    setDecayScale(1.0);
    setSustain(0.75);
    setRelease(0.1);
    setReleaseScale(1.0);
    setSamplerate(44100.0);
    setScale(SCALE_LINEAR);

    setActive(true);
    Reset();
}

void EnvelopeDADSR::Reset(void)
{
    m_eState = STATE_OFF;
}

bool EnvelopeDADSR::isReady(void)
{
    return false;
}

double EnvelopeDADSR::Process(void)
{
    double dOutput = 0.0, div = 0.0;
    double dSamplerate = getSamplerate();

    if (m_bActive)
    {
        switch (m_eState)
        {
            case STATE_OFF:
                tDelay = tAttack = tDecay = tRelease = 0;
                break;

            case STATE_DELAY:
                tDelayEnd = (SInt32)(dDelay * dDelayScale * dSamplerate);

                // normal processing
                if (tDelay < tDelayEnd)
                    tDelay++;
                else
                    m_eState = STATE_ATTACK;
                break;

            case STATE_ATTACK:
                tAttackEnd = (SInt32)(dAttack * dAttackScale * dSamplerate);

                // normal processing
                if (dAttack > LOWEST)
                    div = dAttack * dAttackScale * dSamplerate;
                else
                    div = LOWEST * dAttackScale * dSamplerate;
                dOutput = (double)tAttack / div;

                if (tAttack < tAttackEnd)
                    tAttack++;
                else
                    m_eState = STATE_DECAY;
                break;

            case STATE_DECAY:
                tDecayEnd = (SInt32)(dDecay * dDecayScale * dSamplerate);

                // normal processing
                if (dDecay > LOWEST)
                    div = dDecay * dDecayScale * dSamplerate;
                else
                    div = LOWEST * dDecayScale * dSamplerate;
                dOutput = (((double)tDecay * (dSustain - 1.0)) / div) + 1.0;

                if (tDecay < tDecayEnd)
                    tDecay++;
                else
                    m_eState = STATE_SUSTAIN;
                break;

            case STATE_SUSTAIN:
                dOutput = dSustain;
                break;

            case STATE_RELEASE:
                tReleaseEnd = (SInt32)(dRelease * dReleaseScale * dSamplerate);

                // normal processing
                if (dRelease > LOWEST)
                    div = dRelease * dReleaseScale * dSamplerate;
                else
                    div = LOWEST * dReleaseScale * dSamplerate;
                dOutput = (((double)tRelease * (-m_dCurrentOutput)) / div) + m_dCurrentOutput;

                if (tRelease < tReleaseEnd)
                    tRelease++;
                else
                    m_eState = STATE_OFF;
                break;
        }
    }
    else
    {
        Reset();
    }
    m_dCurrentOutput = dOutput;
    return dOutput;
}
