#ifndef MODDITHER_H_
#define MODDITHER_H_

#include <Base/modBaseEffect.h>
#include <Util/modTestCase.h>
#include <Util/modLogger.h>

#include <math.h>

using namespace eLibV2;

namespace eLibV2
{
    class FxDither : public BaseEffect
    {
	public:
		enum
		{
			DITHER_INPUT,
			DITHER_BITSIZE
		};

	private:
		enum
		{
			DITHER_BITSIZE_MIN = 0,
			DITHER_BITSIZE_MAX = 32
		};

    public:
		FxDither() : BaseName("FxDither") { Init(); }

        void Init(void);
        void Reset(void);
		bool Test(void);
		double Process(double Input);

		virtual double processIOs();

		VstInt16 getBitsize(void) { return mBitsize; }
		void setBitsize(VstInt16 Bitsize);

		void attachInput(ControlIO *controller) { attachIO(DITHER_INPUT, controller); }
		void attachBitsize(ControlIO *controller) { attachIO(DITHER_BITSIZE, controller); }

    private:
		VstInt16 mBitsize;
    };
}

#endif
