#ifndef MODOUTPUTCONNECTOR_H_
#define MODOUTPUTCONNECTOR_H_

#include <Base/BaseConnection.h>
#include <Util/Logger.h>

#include <vector>

namespace eLibV2
{
    namespace Connection
    {
        class OutputConnection : public Base::BaseConnection
        {
        public:
            OutputConnection(std::string name = "OutputConnection") :
                Base::BaseName(name)
            {
            }
            virtual ~OutputConnection() {}

            virtual double processConnection()
            {
                double input = 0.0;

                if (isInputConnected(0))
                    input = inputConnections[0]->processConnection();
                // ModuleLogger::print(LOG_CLASS_EFFECT, "%s::process %lf", getModuleName().c_str(), input);

                return input;
            }

        public:
            void attachInput(BaseConnection *controller) { inputConnections[0] = controller; }

        protected:
            double mValue;
        };
    }
}

#endif
