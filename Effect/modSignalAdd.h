#ifndef MODSIGNALADD_H_
#define MODSIGNALADD_H_

#include <Base/modBaseEffect.h>

using namespace eLibV2;

namespace eLibV2
{
    class FxSignalAdd : public BaseEffect
    {
    public:
		FxSignalAdd() : BaseName("FxSignalAdd") {}

		void Init(void) {}
		void Reset(void) {}
		bool Test();

        double Process(double Input) {return Input;}
        double Process(double Input1, double Input2);
    };
}

#endif
