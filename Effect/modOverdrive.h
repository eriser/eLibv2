#ifndef MODOVERDRIVE_H_
#define MODOVERDRIVE_H_

#include <Base/modBaseEffect.h>

using namespace eLibV2;

namespace eLibV2
{
    class FxOverdrive : public BaseEffect
    {
    public:
		enum
		{
			OVERDRIVE_INPUT,
			OVERDRIVE_LEVEL
		};

		FxOverdrive() : BaseName("FxOverdrive") { Init(); }

		void Init() { setLevel(150.0); }
		void Reset() {}
        double Process(double input);

        void setLevel(double Level) {dLevel = Level;}
        double getLevel(void) {return dLevel;}

		void attachInput(ControlIO *controller) { attachIO(OVERDRIVE_INPUT, controller); }
		void attachLevel(ControlIO *controller) { attachIO(OVERDRIVE_LEVEL, controller); }

		double processIOs();

	protected:
        double dLevel;
    };
}

#endif
