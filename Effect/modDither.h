/*
 * modDither.h
 *
 *  Created on: 25.11.2011
 *      Author: dedokter
 */

#ifndef MODDITHER_H_
#define MODDITHER_H_

#include <Base/modBaseEffect.h>
#include <Util/modTestCase.h>

namespace eLibV2
{
    enum
    {
        DITHER_BITSIZE_MIN = 0,
        DITHER_BITSIZE_MAX = 32
    };

    class FxDither : public BaseEffect, public ModuleTestCase
    {
    public:
        FxDither();
        ~FxDither() {}

        void Init(void);
        void Reset(void);
		void Test(void);
		double Process(double Input);

		virtual double processAudioInputs();

        long getBitsize(void) {return lBitsize;}
        void setBitsize(unsigned int Bitsize);

    private:
        unsigned int lBitsize;
    };
}

#endif /* MODDITHER_H_ */
