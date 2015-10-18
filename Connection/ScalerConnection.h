#ifndef MODSCALERCONNECTOR_H_
#define MODSCALERCONNECTOR_H_

#include <Base/BaseConnection.h>

namespace eLibV2
{
    namespace Connection
    {
        /**
        creates a input connection which processes input with a factor and an offset
        */
        class ScalerConnection : public Base::BaseConnection
        {
        public:
            enum
            {
                SCALER_CONNECTION_FACTOR = 0,
                SCALER_CONNECTION_OFFSET,
                SCALER_CONNECTION_INPUT,
                SCALER_CONNECTION_NUM
            };

        public:
            ScalerConnection(std::string name = "ScalerConnection", double factor = 1.0, double offset = 0.0) :
                Base::BaseName(name),
                BaseConnection(SCALER_CONNECTION_NUM)
            {
                mFactor = factor;
                mOffset = offset;
            }
            virtual ~ScalerConnection() {}

            virtual double processConnection(void)
            {
                double input = 0.0;

                // get factor and offset
                if (isInputConnected(SCALER_CONNECTION_FACTOR))
                    mFactor = inputConnections[SCALER_CONNECTION_FACTOR]->processConnection();
                if (isInputConnected(SCALER_CONNECTION_OFFSET))
                    mOffset = inputConnections[SCALER_CONNECTION_OFFSET]->processConnection();
                if (isInputConnected(SCALER_CONNECTION_INPUT))
                    input = inputConnections[SCALER_CONNECTION_INPUT]->processConnection();

//                ModuleLogger::print(LOG_CLASS_CONNECTION, "%s::processIOs value: %lf %lf %lf -> %lf", getModuleName().c_str(), input, mFactor, mOffset, input * mFactor + mOffset);
                return input * mFactor + mOffset;
            }

            virtual void attachFactor(BaseConnection *controller) { inputConnections[SCALER_CONNECTION_FACTOR] = controller; }
            virtual void attachOffset(BaseConnection *controller) { inputConnections[SCALER_CONNECTION_OFFSET] = controller; }
            virtual void attachInput(BaseConnection *controller) { inputConnections[SCALER_CONNECTION_INPUT] = controller; }

        protected:
            double mFactor, mOffset;
        };
    }
}

#endif
