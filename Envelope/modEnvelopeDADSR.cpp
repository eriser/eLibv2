#include <Envelope/modEnvelopeDADSR.h>

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
                tDelayEnd = (VstInt32)(dDelay * dDelayScale * samplerate);

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
                tAttackEnd = (VstInt32)(dAttack * dAttackScale * samplerate);

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
                    div = dAttack * dAttackScale * samplerate;
                else
                    div = LOWEST * dAttackScale * samplerate;
                res = (double)tAttack / div;

                if (tAttack < tAttackEnd)
                    tAttack++;
                else
                    lEnvelopeState = ENVELOPE_DADSR_STATE_DECAY;
                break;

            case ENVELOPE_DADSR_STATE_DECAY:
                tDecayEnd = (VstInt32)(dDecay * dDecayScale * samplerate);

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
                    div = dDecay * dDecayScale * samplerate;
                else
                    div = LOWEST * dDecayScale * samplerate;
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
                tReleaseEnd = (VstInt32)(dRelease * dReleaseScale * samplerate);

                // normal processing
                if (dRelease > LOWEST)
                    div = dRelease * dReleaseScale * samplerate;
                else
                    div = LOWEST * dReleaseScale * samplerate;
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

#if 0
bool EnvelopeDADSR::Test(void)
{
    double Value;
    long oldstate = 0;

//    TestBeginMsg();
    ModuleLogger::print("using delay: %lf", dDelay);
    ModuleLogger::print("using attack: %lf", dAttack);
    ModuleLogger::print("using decay: %lf", dDecay);
    ModuleLogger::print("using sustain: %lf", dSustain);
    ModuleLogger::print("using release: %lf", dRelease);
    setTrigger(true);
    for (long EnvTime = 0; EnvTime < 200000; EnvTime++)
    {
        Value = Process();
        if (oldstate != lEnvelopeState)
        {
            ModuleLogger::print("time: %li state: %li value: %lf", EnvTime, lEnvelopeState, Value);
            oldstate = lEnvelopeState;
        }
        if (EnvTime > 60000)
            setTrigger(false);
    }
//    TestEndMsg();

    return true;
}
#endif
