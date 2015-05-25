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
            OutputConnection(std::string name = "InputConnection") : Base::BaseName(name) {}

        protected:
            double mValue;
        };
    }
}

#endif
