#ifndef MODSEQUENCER_H_
#define MODSEQUENCER_H_

#include <Base/BaseModule.h>
#include <Util/Helper.h>

#include <vector>

namespace eLibV2
{
    namespace Generator
    {
        class BaseSequencer : public Base::BaseModule
        {
        public:
            BaseSequencer(std::string name = "BaseSequencer")
                : Base::BaseName(name) {
                Init();
            }

            void Init(void);
            void Reset(void) {}
            double Process(const double Input);

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
}

#endif