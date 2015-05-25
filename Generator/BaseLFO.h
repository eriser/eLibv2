#ifndef MODBASELFO_H_
#define MODBASELFO_H_

#include <Generator/BaseOsc.h>

namespace eLibV2
{
    namespace Generator
    {
        class BaseLFO : public BaseOscillator
        {
        public:
            BaseLFO(std::string name = "BaseLFO")
                : BaseName(name) {}

            virtual double Process(void);
        };
    }
}

#endif
