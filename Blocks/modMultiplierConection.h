#ifndef MODMULTIPLIERCONNECTOR_H_
#define MODMULTIPLIERCONNECTOR_H_

#include <Base/modBaseConnection.h>

namespace eLibV2
{
    namespace Connection
    {
        class MultiplierConnection : public BaseConnection
        {
        public:
            enum
            {
                MULTIPLIER_INPUT
            };

        public:
            MultiplierConnection(std::string name = "MultiplierConnection")
                : BaseName(name), numInputs(0) {}

            virtual void Init() {}
            virtual double processConnection()
            {
                double input = 1.0;

                for (connectionIterator it = connections.begin(); it != connections.end(); it++)
                {
                    double value = it->second->processConnection();
                    input *= value;
                }

                ModuleLogger::print("%s::processIOs value: %lf", getModuleName().c_str(), input);
                return input;
            }

            virtual void attachInput(BaseConnection *controller) { connect(MULTIPLIER_INPUT + numInputs, controller); numInputs++; }

        protected:
            int numInputs;
        };
    }
}

#endif
