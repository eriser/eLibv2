#ifndef MODENVELOPEDADSR_H_
#define MODENVELOPEDADSR_H_

#include <Base/modBaseEnvelope.h>

using namespace eLibV2;

namespace eLibV2
{
    class EnvelopeDADSR : public BaseEnvelope
    {
    public:
        enum
        {
            ENVELOPE_DADSR_STATE_INIT = 0,
            ENVELOPE_DADSR_STATE_DELAY,
            ENVELOPE_DADSR_STATE_ATTACK,
            ENVELOPE_DADSR_STATE_DECAY,
            ENVELOPE_DADSR_STATE_SUSTAIN,
            ENVELOPE_DADSR_STATE_RELEASE
        };

    public:
        EnvelopeDADSR(std::string name = "EnvelopeDADSR")
            : BaseName(name) { Init(); }

        virtual void Init(void);
        virtual void Reset(void);
        virtual bool Test(void);
        virtual double Process(void);

        virtual bool isReady(void);

        virtual void setDelay(double Delay) {dDelay = Delay;}
        virtual void setAttack(double Attack) {dAttack = Attack;}
        virtual void setDecay(double Decay) {dDecay = Decay;}
        virtual void setSustain(double Sustain) {dSustain = Sustain;}
        virtual void setRelease(double Release) {dRelease = Release;}

        virtual void setDelayScale(double DelayScale) {dDelayScale = DelayScale;}
        virtual void setAttackScale(double AttackScale) {dAttackScale = AttackScale;}
        virtual void setDecayScale(double DecayScale) {dDecayScale = DecayScale;}
        virtual void setReleaseScale(double ReleaseScale) {dReleaseScale = ReleaseScale;}

    protected:
        VstInt16 lEnvelopeState;
        VstInt32 tDelay, tAttack, tDecay, tRelease;
        VstInt32 tDelayEnd, tAttackEnd, tDecayEnd, tReleaseEnd;

        double dDelay, dAttack, dDecay, dSustain, dRelease;
        double dDelayScale, dAttackScale, dDecayScale, dReleaseScale;
    };
}

#endif /* MODENVELOPEDADSR_H_ */
