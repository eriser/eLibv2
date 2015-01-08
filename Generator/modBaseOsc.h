#ifndef MODBASEOSC_H_
#define MODBASEOSC_H_

#include <Base/modBaseGenerator.h>
#include <Generator/modWavetable.h>

#include <Util/modFrequencyTable.h>
#include <Util/modLogger.h>

using namespace eLibV2;

namespace eLibV2
{
    static const double BASEOSC_COARSE_RANGE = 24.0;
    static const double BASEOSC_FINE_RANGE = 100.0;

    class BaseOscillator : public BaseGenerator
    {
	protected:
		enum
		{
			CONNECTION_OSC_INPUT_NOTE,
			CONNECTION_OSC_INPUT_WAVEFORM,
			CONNECTION_OSC_INPUT_COARSE,
			CONNECTION_OSC_INPUT_FINETUNE
		};

    public:
		BaseOscillator(std::string name = "BaseOscillator")	: BaseName(name) { Init(); }
		~BaseOscillator();

	public:
        virtual void Init(void);
        virtual void Reset(void);
		virtual bool Test(void) { return true; }
		virtual double Process(VstInt16 Note);
		virtual double processConnection();

	public:
		/* set and get methods */
		void setWaveform(VstInt32 Waveform);
        void setCoarse(double Coarse);
        void setFinetune(double Finetune);
        void setSamplerate(double Samplerate);
		void adjustScaler();

        VstInt32 getWaveform(void) {return lWaveform;}
        double getCoarse(void) {return dCoarse;}
        double getFinetune(void) {return dFinetune;}
        VstInt32 getNumWaveforms(void);

	public:
		/* attach methods */
		void attachNote(BaseConnection *controller) { connect(CONNECTION_OSC_INPUT_NOTE, controller); }
		void attachWaveform(BaseConnection *controller) { connect(CONNECTION_OSC_INPUT_WAVEFORM, controller); }
		void attachCoarse(BaseConnection *controller) { connect(CONNECTION_OSC_INPUT_COARSE, controller); }
		void attachFinetune(BaseConnection *controller) { connect(CONNECTION_OSC_INPUT_FINETUNE, controller); }

    protected:
        VstInt32 lWaveform;
        double dCoarse;
        double dFinetune;
        double dPhase;
        double dSamplerate;
		double dScaler;
		BaseWavetable *pBaseWavetable;

    };
}

#endif

