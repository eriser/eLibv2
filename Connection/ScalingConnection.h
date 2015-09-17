#ifndef MODSCALINGCONNECTOR_H_
#define MODSCALINGCONNECTOR_H_

#include <Base/BaseConnection.h>

namespace eLibV2
{
    namespace Connection
    {
        /**
        creates a input connection which processes every input with a factor and an offset
        */
        class ScalingConnection : public Base::BaseConnection
        {
        public:
            enum
            {
                SCALING_FACTOR,
                SCALING_OFFSET,
                SCALING_INPUT
            };

        public:
            ScalingConnection(std::string name = "ScalingConnection", double factor = 1.0, double offset = 0.0)
                : Base::BaseName(name), numInputs(0), mFactor(factor), mOffset(offset) {}

            virtual double processConnection()
            {
                double input = 0.0;

                // get factor and offset
                if (isConnected(SCALING_FACTOR))
                    mFactor = connections[SCALING_FACTOR]->processConnection();
                if (isConnected(SCALING_OFFSET))
                    mOffset = connections[SCALING_FACTOR]->processConnection();

                // sum up all inputs
                for (unsigned int inputIndex = 0; inputIndex < numInputs; inputIndex++)
                {
                    if (isConnected(inputIndex))
                        input += connections[inputIndex]->processConnection();
                }

//                ModuleLogger::print(LOG_CLASS_CONNECTION, "%s::processIOs value: %lf %lf %lf -> %lf", getModuleName().c_str(), input, mFactor, mOffset, input * mFactor + mOffset);
                return input * mFactor + mOffset;
            }

            virtual void attachInput(BaseConnection *controller) { connect(SCALING_INPUT + numInputs, controller); numInputs++; }
            virtual void attachFactor(BaseConnection *controller) { connect(SCALING_FACTOR, controller); }
            virtual void attachOffset(BaseConnection *controller) { connect(SCALING_OFFSET, controller); }

        protected:
            int numInputs;
            double mFactor, mOffset;
        };
    }
}

#endif
