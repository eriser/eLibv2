#ifndef MODBASELFO_H_
#define MODBASELFO_H_

#include <Generator/BaseOsc.h>

// TODO: initiailization of inputs needed
// is only one, but 4 should be inherited

// frequencies should be 100 times lower
// a -> 440.0 -> 4.4
namespace eLibV2
{
    namespace Generator
    {
        class BaseLFO : public BaseOscillator
        {
        public:
            BaseLFO(std::string name = "BaseLFO") :
                BaseName(name),
                BaseOscillator()
            {}

            virtual double Process(void);
        };
    }
}

#endif
