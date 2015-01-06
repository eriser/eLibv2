#ifndef MODDITHER_H_
#define MODDITHER_H_

#include <Base/modBaseEffect.h>
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
			CONNECTION_DITHER_INPUT,
			CONNECTION_DITHER_BITSIZE
		};

	private:
		enum
		{
			DITHER_BITSIZE_MIN = 0,
			DITHER_BITSIZE_MAX = 32
		};

    public:
		FxDither(std::string name = "FxDither")
			: BaseName(name), BaseConnector(1, 1) { Init(); }

        virtual void Init(void);
        virtual void Reset(void);
		virtual bool Test(void);
		virtual double Process(double Input);
		virtual double processConnection();

	public:
		VstInt16 getBitsize(void) { return mBitsize; }
		void setBitsize(VstInt16 Bitsize);

	public:
		void attachInput(BaseConnector *controller) { connect(CONNECTION_DITHER_INPUT, controller); }
		void attachBitsize(BaseConnector *controller) { connect(CONNECTION_DITHER_BITSIZE, controller); }

    private:
		VstInt16 mBitsize;
    };
}

#endif
