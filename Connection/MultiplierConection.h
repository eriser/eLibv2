#ifndef MODMULTIPLIERCONNECTOR_H_
#define MODMULTIPLIERCONNECTOR_H_

#include <Base/BaseConnection.h>

namespace eLibV2
{
    namespace Connection
    {
        /**
        creates a input connection which multiplies all input with a previously set factor
        @todo is this really complete?
        */
        class MultiplierConnection : public Base::BaseConnection
        {
        public:
            enum
            {
                MULTIPLIER_INPUT
            };

        public:
            MultiplierConnection(std::string name = "MultiplierConnection")
                : Base::BaseName(name), numInputs(0) {}

            virtual double processConnection()
            {
                double input = 1.0;

                for (connectionIterator it = connections.begin(); it != connections.end(); it++)
                {
                    double value = it->second->processConnection();
                    input *= value;
                }

                ModuleLogger::print(LOG_CLASS_CONNECTION, "%s::processIOs value: %lf", getModuleName().c_str(), input);
                return input;
            }

            virtual void attachInput(BaseConnection *controller) { connect(MULTIPLIER_INPUT + numInputs, controller); numInputs++; }

        protected:
            int numInputs;
        };
    }
}

#endif
