#ifndef MODBASELFO_H_
#define MODBASELFO_H_

#include <Generator/modBaseOsc.h>

using namespace eLibV2;

namespace eLibV2
{
    class BaseLFO : public BaseOscillator
    {
    public:
		BaseLFO(std::string name = "BaseLFO")
			: BaseName(name) {}

        virtual double Process(void);
		virtual bool Test(void) { return true; }
    };
}

#endif
