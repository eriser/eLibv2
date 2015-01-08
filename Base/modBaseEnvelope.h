#ifndef MODBASEENVELOPE_H_
#define MODBASEENVELOPE_H_

#include <Base/modBaseModule.h>

namespace eLibV2
{
    class BaseEnvelope : public BaseModule
    {
	protected:
		enum
		{
			ENVELOPE_MODE_UNDEF = 0,
			ENVELOPE_MODE_LINEAR,
			ENVELOPE_MODE_LOGARITHMIC,
			ENVELOPE_MODE_EXPONENTIAL
		};

    public:
		BaseEnvelope(std::string name = "BaseEnvelope")	: BaseName(name) {}

        virtual void Init(void) = 0;
        virtual void Reset(void) = 0;
        virtual double Process(void) = 0;

        virtual void setSampleRate(double Samplerate) {dSamplerate = Samplerate;}
        virtual void setActive(bool Active) {bActive = Active;}
        virtual void setEnvelopeMode(VstInt16 EnvelopeMode) {lEnvelopeMode = EnvelopeMode;}
        virtual void setTrigger(bool Trigger) {bTrigger = Trigger;}

    protected:
        bool bActive, bTrigger, bOldTrigger;
        double dSamplerate;
        double dLastLevel;

        VstInt16 lEnvelopeMode;
    };
}

#endif
