#ifndef MODSEQUENCER_H_
#define MODSEQUENCER_H_

#include <Base/modBaseModule.h>

#include <vector>

using namespace eLibV2;

namespace eLibV2
{
    class BaseSequencer : public BaseModule
    {
    public:
		BaseSequencer() : BaseName("BaseSequencer") { Init(); }

        void Init();
		void Reset() {}
        double Process();

        void setSync(bool Sync) { bSync = Sync; }
        void setSpeed(double Speed) { dSpeed = Speed; }
        void setBpm(double Bpm) { dBpm = Bpm; }
        void setType(long Type) { lType = Type; }
        void setStepValue(unsigned long Index, double Value);

        bool getSync(void) { return bSync; }
        double getSpeed(void) { return dSpeed; }
        double getBpm(void) { return dBpm; }
		long getType(void) { return lType; }
        long getStepCount(void);
		double getStepValue(unsigned long Index);

        void addSteps(long Count);

    private:
        bool bSync;
        double dSpeed, dBpm;
        long lType, lCurrentSamplePosition, lCurrentStepIndex;
		std::vector<double> SequencerData;
    };
}

#endif