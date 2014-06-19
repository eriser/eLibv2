/*
 * modBaseOsc.h
 *
 *  Created on: 24.11.2011
 *      Author: dedokter
 */

#ifndef MODBASEOSC_H_
#define MODBASEOSC_H_

#include "modBaseModule.h"
#include "modWavetable.h"

namespace eLibV2
{
    enum
    {
        kNumFrequencies = 128,  // 128 midi notes
    };

    static const double BASEOSC_COARSE_RANGE = 24.0;
    static const double BASEOSC_FINE_RANGE = 100.0;

    class BaseOscillator : public BaseModule
    {
    public:
        BaseOscillator();
        ~BaseOscillator();

        virtual void Init(void);
        virtual void Reset(void);
        virtual void Test(void) {}
        virtual double Process(VstInt16 Note);

        void setWaveform(VstInt32 Waveform);
        void setCoarse(double Coarse);
        void setFinetune(double Finetune);
        void setSamplerate(double Samplerate);
        void setScaler();

        VstInt32 getWaveform(void) {return lWaveform;}
        double getCoarse(void) {return dCoarse;}
        double getCoarseScale(void) {return 2 * BASEOSC_COARSE_RANGE - BASEOSC_COARSE_RANGE;}
        double getFinetune(void) {return dFinetune;}
        double getFinetuneScale(void) {return 2 * BASEOSC_FINE_RANGE - BASEOSC_FINE_RANGE;}

        VstInt32 getNumWaveforms(void);

    protected:
        VstInt32 lWaveform;
        double dCoarse;
        double dFinetune;
        double dPhase;
        double dSamplerate;
        double dScaler;
        double freqtab[kNumFrequencies];

    private:
        void SetupFreqs(void);

        BaseWavetable *pBaseWavetable;
    };
}

#endif /* MODBASEOSC_H_ */
