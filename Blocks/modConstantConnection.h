#ifndef MODCONSTANTCONNECTOR_H_
#define MODCONSTANTCONNECTOR_H_

#include <Blocks/modInputConnection.h>

#include <Util/modLogger.h>

#include <vector>

namespace eLibV2
{
    namespace Connection
    {
        class ConstantConnection : public InputConnection
        {
        public:
            ConstantConnection(double value) : BaseName("ConstantConnection"), mValue(value) {}

            virtual double processIOs() { return processInputs(); }
            double processInputs() { ModuleLogger::print("%s::processInputs value: %lf", getModuleName().c_str(), mValue); return mValue; }

        protected:
            const double mValue;
        };
    }
}

#endif
