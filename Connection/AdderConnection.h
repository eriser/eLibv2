#ifndef MODADDERCONNECTOR_H_
#define MODADDERCONNECTOR_H_

#include <Connection/BaseConnection.h>

namespace eLibV2
{
    namespace Connection
    {
        /**
        creates a input connection which processes every input with a factor and an offset
        */
        class AdderConnection : public BaseConnection
        {
        public:
            enum
            {
                ADDER_INPUT_NUM = 8
            };

        public:
            AdderConnection(std::string name = "AdderConnection") :
                Base::BaseName(name),
                BaseConnection(ADDER_INPUT_NUM)
            {
                memset(inputConnections, 0, sizeof(inputConnections));
                numInputs = 0;
            }
            virtual ~AdderConnection() {};

            virtual double processConnection(void)
            {
                double input = 0.0;

                for (UInt8 inputIndex = 0; inputIndex < ADDER_INPUT_NUM; ++inputIndex)
                {
                    if (isInputConnected(inputIndex))
                        input += inputConnections[inputIndex]->processConnection();
                }

                // ModuleLogger::print(LOG_CLASS_CONNECTION, "%s::processIOs value: %lf %lf %lf -> %lf", getModuleName().c_str(), input, mFactor, mOffset, input * mFactor + mOffset);
                return input;
            }

            virtual void attachInput(BaseConnection *connection)
            {
                if (numInputs < ADDER_INPUT_NUM)
                    inputConnections[numInputs++] = connection;
            }

        protected:
            UInt8 numInputs;
        };
    }
}

#endif
