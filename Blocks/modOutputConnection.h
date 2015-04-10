#ifndef MODOUTPUTCONNECTOR_H_
#define MODOUTPUTCONNECTOR_H_

#include <Base/modBaseConnection.h>

#include <Util/modLogger.h>

#include <vector>

namespace eLibV2
{
    namespace Connection
    {
        class OutputConnection : public BaseConnection
        {
        public:
            OutputConnection(std::string name = "InputConnection") : BaseName(name) {}

        protected:
            double mValue;
        };
    }
}

#endif
