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
            {}
            virtual ~OutputConnection() {}

        protected:
            double mValue;
        };
    }
}

#endif
