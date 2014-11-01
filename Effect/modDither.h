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
#include <Util/modLogger.h>

#include <math.h>

namespace eLibV2
{
    class FxDither : public BaseEffect, public ModuleTestCase
    {
	public:
		enum
		{
			DITHER_OUTPUT,
			DITHER_INPUT_BITSIZE
		};

	private:
		enum
		{
			DITHER_BITSIZE_MIN = 0,
			DITHER_BITSIZE_MAX = 32
		};

    public:
        FxDither();
        ~FxDither() {}

        void Init(void);
        void Reset(void);
		void Test(void);
		double Process(double Input);

		virtual double processIOs();

        long getBitsize(void) {return lBitsize;}
        void setBitsize(unsigned int Bitsize);

    private:
        unsigned int lBitsize;
    };
}

#endif /* MODDITHER_H_ */
