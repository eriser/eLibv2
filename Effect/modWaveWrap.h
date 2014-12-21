#ifndef MODWAVEWRAP_H_
#define MODWAVEWRAP_H_

#include <Base/modBaseEffect.h>

using namespace eLibV2;

namespace eLibV2
{
    enum { WRAP_WAVESIZE = 32768 };
	enum
	{
		WAVEWRAP_INPUT,
		WAVEWRAP_LEVEL
	};

    class FxWaveWrap : public BaseEffect
    {
    public:
		FxWaveWrap() : BaseName("FxWaveWrap") { Init(); }

        void Init(void);
        void Reset(void) {}

        double getWrapLevel(void) {return dWrapLevel;}
        void setWrapLevel(double WrapLevel) {dWrapLevel = WrapLevel;}

        double Process(double Input);
		void attachInput(ControlIO *controller) { attachIO(WAVEWRAP_INPUT, controller); }
		void attachLevel(ControlIO *controller) { attachIO(WAVEWRAP_LEVEL, controller); }

		double processIOs();

    private:
        double dWrapLevel;
        double triangle[WRAP_WAVESIZE];
    };
}

#endif
