#ifndef MODCONSTANTCONNECTOR_H_
#define MODCONSTANTCONNECTOR_H_

#include <Connection/InputConnection.h>
#include <Util/Logger.h>

#include <vector>

namespace eLibV2
{
    namespace Connection
    {
        /**
        creates a constant connection which always return a single constant value specified at construction
        */
        class ConstantConnection : public InputConnection
        {
        public:
            ConstantConnection(double value) : BaseName("ConstantConnection"), mValue(value) {}

            /**
            process connected inputs
            @return result of procession
            */
            virtual double processConnection() { return processInputs(); }

            double processInputs()
            {
                // ModuleLogger::print(LOG_CLASS_CONNECTION, "%s::processInputs value: %lf", getModuleName().c_str(), mValue);
                return mValue;
            }

        protected:
            const double mValue; ///< internal value used for processing
        };
    }
}

#endif
