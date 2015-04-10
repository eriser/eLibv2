#ifndef MODBASEEFFECT_H_
#define MODBASEEFFECT_H_

#include <Base/modBaseModule.h>

namespace eLibV2
{
    class BaseEffect : public BaseModule
    {
    public:
        BaseEffect(std::string name = "BaseEffect") : BaseName(name), BaseConnection(1, 1) {}

        virtual void Reset(void) = 0;
        virtual double Process(double Input) = 0;

    protected:
        double *pBuffer;
    };
}

#endif
