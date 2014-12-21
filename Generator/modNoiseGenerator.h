#ifndef MODNOISEGENERATOR_H_
#define MODNOISEGENERATOR_H_

#include <Base/modBaseModule.h>

using namespace eLibV2;

namespace eLibV2
{
    class NoiseGenerator : public BaseModule
    {
    public:
		NoiseGenerator() : BaseName("NoiseGenerator") {}

        double Process(void);

    protected:
        double *pBuffer;
    };
}

#endif
