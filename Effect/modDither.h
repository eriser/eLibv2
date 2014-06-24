/*
 * modDither.h
 *
 *  Created on: 25.11.2011
 *      Author: dedokter
 */

#ifndef MODDITHER_H_
#define MODDITHER_H_

#include "Base/modBaseEffect.h"

namespace eLibV2
{
    enum
    {
        DITHER_BITSIZE_MIN = 0,
        DITHER_BITSIZE_MAX = 24
    };

    class FxDither : public BaseEffect
    {
    public:
        FxDither();
        ~FxDither() {}

        void Init(void);
        void Reset(void);
        void Test(void);
        double Process(double Input);

        long getBitsize(void) {return lBitsize;}
        void setBitsize(long Bitsize);

    private:
        long lBitsize;
    };
}

#endif /* MODDITHER_H_ */
