/*
 * modSequencer.h
 *
 *  Created on: 06.01.2012
 *      Author: dedokter
 */

#ifndef MODSEQUENCER_H_
#define MODSEQUENCER_H_

#include <vector>
#include "modBaseModule.h"

namespace eLibV2
{
    class BaseSequencer : public BaseModule
    {
    public:
        BaseSequencer();
        ~BaseSequencer() {}

        void Init();
        void Reset();
        double Process();

        void setSync(bool Sync) {bSync = Sync;}
        void setSpeed(double Speed) {dSpeed = Speed;}
        void setBpm(double Bpm) {dBpm = Bpm;}
        void setType(long Type) {lType = Type;}
        void setStepValue(unsigned long Index, double Value);

        bool getSync(void) {return bSync;}
        double getSpeed(void) {return dSpeed;}
        double getBpm(void) {return dBpm;}
        long getStepCount(void);
        void getStepValue(unsigned long Index, double *Value);

        void addSteps(long Count);

    private:
        bool bSync;
        double dSpeed, dBpm;
        long lType, lCurrentSamplePosition, lCurrentStepIndex;
        vector<double> SequencerData;
    };
}

#endif /* MODSEQUENCER_H_ */
