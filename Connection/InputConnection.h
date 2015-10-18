#ifndef MODINPUTCONNECTOR_H_
#define MODINPUTCONNECTOR_H_

#include <Base/BaseConnection.h>
#include <Util/Logger.h>

#include <vector>

using namespace eLibV2::Util;

namespace eLibV2
{
    namespace Connection
    {
        /**
        creates a generic input connection with a variable value which can be set during runtime
        */
        class InputConnection : public Base::BaseConnection
        {
        public:
            InputConnection(std::string name = "InputConnection") :
                Base::BaseName(name)
            {}

            /**
            set input value
            @param value value to use during processing
            */
            virtual void setInput(double value)
            {
                // ModuleLogger::print(LOG_CLASS_CONNECTION, "%s::setValue: %lf", getModuleName().c_str(), value);
                mValue = value;
            }

            /**
            process connected inputs
            @return result of procession
            */
            virtual double processConnection(void) { return processInputs(); }
            virtual double processInputs()
            {
                // ModuleLogger::print(LOG_CLASS_CONNECTION, "%s::processInputs value: %lf", getModuleName().c_str(), mValue);
                return mValue;
            }

        protected:
            double mValue; ///< internal value used for processing
        };
    }
}

#endif
