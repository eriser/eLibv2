#ifndef MODBASEGENERATOR_H_
#define MODBASEGENERATOR_H_

#include <Base/BaseModule.h>

#include <string>

namespace eLibV2
{
    namespace Generator
    {
        /**
        this module provides methods for generating output signals in its subclasses
        */
        class BaseGenerator : public Base::BaseModule
        {
        public:
            BaseGenerator(std::string name = "BaseGenerator") : Base::BaseName(name) {}

            /**
            reset internal module state to initialization
            */
            virtual void Reset(void) = 0;

            /**
            generate output signal using given note
            @param Note input note
            @return the output signal
            */
            virtual double Process(UInt8 Note) = 0;
        };
    }
}

#endif
