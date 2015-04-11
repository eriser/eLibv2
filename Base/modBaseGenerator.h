#ifndef MODBASEGENERATOR_H_
#define MODBASEGENERATOR_H_

#include <Base/modBaseModule.h>

#include <string>

namespace eLibV2
{
    namespace Base
    {
        static const VstInt16 GENERATOR_NUM_NOTES = 128; ///< number of notes used in generators

        /**
        this module provides methods for generating output signals in its subclasses
        */
        class BaseGenerator : public BaseModule
        {
        public:
            BaseGenerator(std::string name = "BaseGenerator") : BaseName(name) {}

            /**
            reset internal module state to initialization
            */
            virtual void Reset(void) = 0;

            /**
            generate output signal using given note
            @param Note input note
            @return the output signal
            */
            virtual double Process(VstInt16 Note) = 0;
        };
    }
}

#endif
