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
    setEnvelopeMode(ENVELOPE_MODE_LINEAR);

    setActive(true);
    Reset();
}

void EnvelopeDADSR::Reset(void)
{
    eEnvelopeState = ENVELOPE_DADSR_STATE_INIT;
    bTrigger = bOldTrigger = false;
}

bool EnvelopeDADSR::isReady(void)
{
    return ((eEnvelopeState == ENVELOPE_DADSR_STATE_INIT) && ((bTrigger | bOldTrigger) == false));
}

double EnvelopeDADSR::Process(void)
{
    double res = 0.0, div = 0.0;

    if (bActive)
    {
        switch (eEnvelopeState)
        {
            case ENVELOPE_DADSR_STATE_INIT:
                tDelay = tAttack = tDecay = tRelease = 0;

                // trigger changed
                if (bOldTrigger != bTrigger)
                {
                    bOldTrigger = bTrigger;
                    if (bTrigger)
                    {
                        eEnvelopeState = ENVELOPE_DADSR_STATE_DELAY;
                        break;
                    }
                }
                break;

            case ENVELOPE_DADSR_STATE_DELAY:
                tDelayEnd = (VstInt32)(dDelay * dDelayScale * mSamplerate);

                // trigger changed
                if (bOldTrigger != bTrigger)
                {
                    bOldTrigger = bTrigger;
                    if (!bTrigger)
                    {
                        eEnvelopeState = ENVELOPE_DADSR_STATE_RELEASE;
                        break;
                    }
                }

                // normal processing
                if (tDelay < tDelayEnd)
                    tDelay++;
                else
                    eEnvelopeState = ENVELOPE_DADSR_STATE_ATTACK;
                break;

            case ENVELOPE_DADSR_STATE_ATTACK:
                tAttackEnd = (VstInt32)(dAttack * dAttackScale * mSamplerate);

                // trigger changed
                if (bOldTrigger != bTrigger)
                {
                    bOldTrigger = bTrigger;
                    if (!bTrigger)
                    {
                        eEnvelopeState = ENVELOPE_DADSR_STATE_RELEASE;
                        break;
                    }
                }

                // normal processing
                if (dAttack > LOWEST)
                    div = dAttack * dAttackScale * mSamplerate;
                else
                    div = LOWEST * dAttackScale * mSamplerate;
                res = (double)tAttack / div;

                if (tAttack < tAttackEnd)
                    tAttack++;
                else
                    eEnvelopeState = ENVELOPE_DADSR_STATE_DECAY;
                break;

            case ENVELOPE_DADSR_STATE_DECAY:
                tDecayEnd = (VstInt32)(dDecay * dDecayScale * mSamplerate);

                // trigger changed
                if (bOldTrigger != bTrigger)
                {
                    bOldTrigger = bTrigger;
                    if (!bTrigger)
                    {
                        eEnvelopeState = ENVELOPE_DADSR_STATE_RELEASE;
                        break;
                    }
                }

                // normal processing
                if (dDecay > LOWEST)
                    div = dDecay * dDecayScale * mSamplerate;
                else
                    div = LOWEST * dDecayScale * mSamplerate;
                res = (((double)tDecay * (dSustain - 1.0)) / div) + 1.0;

                if (tDecay < tDecayEnd)
                    tDecay++;
                else
                    eEnvelopeState = ENVELOPE_DADSR_STATE_SUSTAIN;
                break;

            case ENVELOPE_DADSR_STATE_SUSTAIN:
                // trigger changed
                if (bOldTrigger != bTrigger)
                {
                    bOldTrigger = bTrigger;
                    if (!bTrigger)
                    {
                        eEnvelopeState = ENVELOPE_DADSR_STATE_RELEASE;
                        break;
                    }
                }

                res = dSustain;
                dLastLevel = res;
                break;

            case ENVELOPE_DADSR_STATE_RELEASE:
                tReleaseEnd = (VstInt32)(dRelease * dReleaseScale * mSamplerate);

                // normal processing
                if (dRelease > LOWEST)
                    div = dRelease * dReleaseScale * mSamplerate;
                else
                    div = LOWEST * dReleaseScale * mSamplerate;
                res = (((double)tRelease * (-dLastLevel)) / div) + dLastLevel;

                if (tRelease < tReleaseEnd)
                    tRelease++;
                else
                    eEnvelopeState = ENVELOPE_DADSR_STATE_INIT;
                break;
        }
    }
    else
    {
        Reset();
    }
    dLastLevel = res;
    return res;
}

#if 0
bool EnvelopeDADSR::Test(void)
{
    double Value;
    long oldstate = 0;

//    TestBeginMsg();
    ModuleLogger::print(LOG_CLASS_EFFECT, "using delay: %lf", dDelay);
    ModuleLogger::print(LOG_CLASS_EFFECT, "using attack: %lf", dAttack);
    ModuleLogger::print(LOG_CLASS_EFFECT, "using decay: %lf", dDecay);
    ModuleLogger::print(LOG_CLASS_EFFECT, "using sustain: %lf", dSustain);
    ModuleLogger::print(LOG_CLASS_EFFECT, "using release: %lf", dRelease);
    setTrigger(true);
    for (long EnvTime = 0; EnvTime < 200000; EnvTime++)
    {
        Value = Process();
        if (oldstate != lEnvelopeState)
        {
            ModuleLogger::print(LOG_CLASS_EFFECT, "time: %li state: %li value: %lf", EnvTime, lEnvelopeState, Value);
            oldstate = lEnvelopeState;
        }
        if (EnvTime > 60000)
            setTrigger(false);
    }
//    TestEndMsg();

    return true;
}
#endif
