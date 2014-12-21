#ifndef MODBASEOSC_H_
#define MODBASEOSC_H_

#include <Base/modBaseGenerator.h>
#include <Util/modLogger.h>

using namespace eLibV2;

namespace eLibV2
{
    static const double BASEOSC_COARSE_RANGE = 24.0;
    static const double BASEOSC_FINE_RANGE = 100.0;

    class BaseOscillator : public BaseGenerator
    {
	public:
		enum
		{
			OSC_INPUT_NOTE,
			OSC_INPUT_WAVEFORM,
			OSC_INPUT_COARSE,
			OSC_INPUT_FINETUNE
		};

	private:

    public:
		BaseOscillator() : BaseName("BaseOscillator") { Init(); }
		~BaseOscillator();

        virtual void Init(void);
        virtual void Reset(void);
		virtual double Process(VstInt16 Note);
		virtual double processIOs();

        void setWaveform(VstInt32 Waveform);
        void setCoarse(double Coarse);
        void setFinetune(double Finetune);
        void setSamplerate(double Samplerate);
		void adjustScaler();

        VstInt32 getWaveform(void) {return lWaveform;}
        double getCoarse(void) {return dCoarse;}
        double getFinetune(void) {return dFinetune;}

        VstInt32 getNumWaveforms(void);

		void attachNote(ControlIO *controller) { attachIO(OSC_INPUT_NOTE, controller); }
		void attachWaveform(ControlIO *controller) { attachIO(OSC_INPUT_WAVEFORM, controller); }
		void attachCoarse(ControlIO *controller) { attachIO(OSC_INPUT_COARSE, controller); }
		void attachFinetune(ControlIO *controller) { attachIO(OSC_INPUT_FINETUNE, controller); }

    protected:
        VstInt32 lWaveform;
        double dCoarse;
        double dFinetune;
        double dPhase;
        double dSamplerate;
		double dScaler;
    };
}

#endif

