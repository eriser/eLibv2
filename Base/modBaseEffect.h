#ifndef MODBASEEFFECT_H_
#define MODBASEEFFECT_H_

#include <Base/modBaseModule.h>

namespace eLibV2
{
    class BaseEffect : public BaseModule
    {
    public:
		BaseEffect() : BaseName("BaseEffect") {}
		BaseEffect(std::string name) : BaseName(name) {}

        virtual void Init(void) = 0;
        virtual void Reset(void) = 0;
        virtual double Process(double Input) = 0;

    protected:
        double *pBuffer;
    };
}

#endif
