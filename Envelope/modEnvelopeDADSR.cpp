#include <Envelope/modEnvelopeDADSR.h>

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
    lEnvelopeState = ENVELOPE_DADSR_STATE_INIT;
    bTrigger = bOldTrigger = false;
}

bool EnvelopeDADSR::isReady(void)
{
    return ((lEnvelopeState == ENVELOPE_DADSR_STATE_INIT) && ((bTrigger | bOldTrigger) == false));
}

double EnvelopeDADSR::Process(void)
{
    double res = 0.0, div = 0.0;

    if (bActive)
    {
        switch (lEnvelopeState)
        {
            case ENVELOPE_DADSR_STATE_INIT:
                tDelay = tAttack = tDecay = tRelease = 0;

                // trigger changed
                if (bOldTrigger != bTrigger)
                {
                    bOldTrigger = bTrigger;
                    if (bTrigger)
                    {
                        lEnvelopeState++;
                        break;
                    }
                }
                break;

            case ENVELOPE_DADSR_STATE_DELAY:
                tDelayEnd = (VstInt32)(dDelay * dDelayScale * dSamplerate);

                // trigger changed
                if (bOldTrigger != bTrigger)
                {
                    bOldTrigger = bTrigger;
                    if (!bTrigger)
                    {
                        lEnvelopeState = ENVELOPE_DADSR_STATE_RELEASE;
                        break;
                    }
                }

                // normal processing
                if (tDelay < tDelayEnd)
                    tDelay++;
                else
                    lEnvelopeState = ENVELOPE_DADSR_STATE_ATTACK;
                break;

            case ENVELOPE_DADSR_STATE_ATTACK:
                tAttackEnd = (VstInt32)(dAttack * dAttackScale * dSamplerate);

                // trigger changed
                if (bOldTrigger != bTrigger)
                {
                    bOldTrigger = bTrigger;
                    if (!bTrigger)
                    {
                        lEnvelopeState = ENVELOPE_DADSR_STATE_RELEASE;
                        break;
                    }
                }

                // normal processing
                if (dAttack > LOWEST)
                    div = dAttack * dAttackScale * dSamplerate;
                else
                    div = LOWEST * dAttackScale * dSamplerate;
                res = (double)tAttack / div;

                if (tAttack < tAttackEnd)
                    tAttack++;
                else
                    lEnvelopeState = ENVELOPE_DADSR_STATE_DECAY;
                break;

            case ENVELOPE_DADSR_STATE_DECAY:
                tDecayEnd = (VstInt32)(dDecay * dDecayScale * dSamplerate);

                // trigger changed
                if (bOldTrigger != bTrigger)
                {
                    bOldTrigger = bTrigger;
                    if (!bTrigger)
                    {
                        lEnvelopeState = ENVELOPE_DADSR_STATE_RELEASE;
                        break;
                    }
                }

                // normal processing
                if (dDecay > LOWEST)
                    div = dDecay * dDecayScale * dSamplerate;
                else
                    div = LOWEST * dDecayScale * dSamplerate;
                res = (((double)tDecay * (dSustain - 1.0)) / div) + 1.0;

                if (tDecay < tDecayEnd)
                    tDecay++;
                else
                    lEnvelopeState = ENVELOPE_DADSR_STATE_SUSTAIN;
                break;

            case ENVELOPE_DADSR_STATE_SUSTAIN:
                // trigger changed
                if (bOldTrigger != bTrigger)
                {
                    bOldTrigger = bTrigger;
                    if (!bTrigger)
                    {
                        lEnvelopeState = ENVELOPE_DADSR_STATE_RELEASE;
                        break;
                    }
                }

                res = dSustain;
                dLastLevel = res;
                break;

            case ENVELOPE_DADSR_STATE_RELEASE:
                tReleaseEnd = (VstInt32)(dRelease * dReleaseScale * dSamplerate);

                // normal processing
                if (dRelease > LOWEST)
                    div = dRelease * dReleaseScale * dSamplerate;
                else
                    div = LOWEST * dReleaseScale * dSamplerate;
                res = (((double)tRelease * (-dLastLevel)) / div) + dLastLevel;

                if (tRelease < tReleaseEnd)
                    tRelease++;
                else
                    lEnvelopeState = ENVELOPE_DADSR_STATE_INIT;
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

void EnvelopeDADSR::runTests(void)
{
    double Value;
    long oldstate = 0;

    TestBeginMsg();
    dbgOutputF("using delay: %lf", dDelay);
    dbgOutputF("using attack: %lf", dAttack);
    dbgOutputF("using decay: %lf", dDecay);
    dbgOutputF("using sustain: %lf", dSustain);
    dbgOutputF("using release: %lf", dRelease);
    setTrigger(true);
    for (long EnvTime = 0; EnvTime < 200000; EnvTime++)
    {
        Value = Process();
        if (oldstate != lEnvelopeState)
        {
            dbgOutputF("time: %li state: %li value: %lf", EnvTime, lEnvelopeState, Value);
            oldstate = lEnvelopeState;
        }
        if (EnvTime > 60000)
            setTrigger(false);
    }
    TestEndMsg();
}
