/*
 * modEnvFollower.h
 *
 *  Created on: 25.11.2011
 *      Author: dedokter
 */

#ifndef MODENVFOLLOWER_H_
#define MODENVFOLLOWER_H_

#include "Base/modBaseEffect.h"

namespace eLibV2
{
	class FxEnvFollower : public BaseEffect
	{
	public:
        FxEnvFollower();
        ~FxEnvFollower() {}

		void Init(void);
		void Reset(void);
        void Test(void) {}
		double Process(double Input);

        double getAttack(void) {return dAttack;}
        double getRelease(void) {return dRelease;}

        void setAttack(double Attack) {dAttack = Attack;}
        void setRelease(double Release) {dRelease = Release;}

    private:
        double dAttack;
        double dRelease;
        double dLastInput;
        double dSampleRate;
        long tAttack;
        long tRelease;
	};
}

#endif /* MODENVFOLLOWER_H_ */
