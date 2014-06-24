/*
 * modWaveWrap.h
 *
 *  Created on: 26.11.2011
 *      Author: dedokter
 */

#ifndef MODWAVEWRAP_H_
#define MODWAVEWRAP_H_

#include "Base/modBaseEffect.h"

#define sgn(x) x < 0.0 ? -1.0 : 1.0

namespace eLibV2
{
    enum
    {
        WRAP_WAVESIZE = 32768
    };

    class FxWaveWrap : public BaseEffect
    {
    public:
        FxWaveWrap();
        ~FxWaveWrap() {}

        void Init(void);
        void Reset(void) {}
        void Test(void) {}

        double getWrapLevel(void) {return dWrapLevel;}
        void setWrapLevel(double WrapLevel) {dWrapLevel = WrapLevel;}

        double Process(double Input);

    private:
        double dWrapLevel;
        double triangle[WRAP_WAVESIZE];
    };
}

#endif /* MODWAVEWRAP_H_ */
