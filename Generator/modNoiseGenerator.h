#ifndef MODNOISEGENERATOR_H_
#define MODNOISEGENERATOR_H_

#include <Base/modBaseModule.h>
#include <Util/modHelper.h>

namespace eLibV2
{
    namespace Generator
    {
        class NoiseGenerator : public Base::BaseModule
        {
        public:
            NoiseGenerator(std::string name = "NoiseGenerator")
                : Base::BaseName(name) {}

            double Process(void);

        protected:
            double *pBuffer;
        };
    }
}

#endif
