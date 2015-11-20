#ifndef MODMULTIPLIERCONNECTOR_H_
#define MODMULTIPLIERCONNECTOR_H_

#include <Connection/BaseConnection.h>

namespace eLibV2
{
    namespace Connection
    {
        /**
        creates a input connection which multiplies all input with a previously set factor
        @todo is this really complete?
        */
        class MultiplierConnection : public BaseConnection
        {
        public:
            enum
            {
                MULTIPLIER_INPUT_NUM = 4
            };

        public:
            MultiplierConnection(std::string name = "MultiplierConnection") :
                Base::BaseName(name),
                BaseConnection(MULTIPLIER_INPUT_NUM),
                numInputs(0)
            {
            }
            virtual ~MultiplierConnection() {}

            virtual double processConnection(void)
            {
                double input = 1.0;

                for (UInt8 inputIndex = 0; inputIndex < MULTIPLIER_INPUT_NUM; ++inputIndex)
                {
                    if (isInputConnected(inputIndex))
                        input *= inputConnections[inputIndex]->processConnection();
                }

                // ModuleLogger::print(LOG_CLASS_CONNECTION, "%s::processIOs value: %lf", getModuleName().c_str(), input);
                return input;
            }

            virtual void attachInput(BaseConnection *connection)
            {
                if (numInputs < MULTIPLIER_INPUT_NUM)
                    inputConnections[numInputs++] = connection;
            }

        protected:
            UInt8 numInputs;
        };
    }
}

#endif
