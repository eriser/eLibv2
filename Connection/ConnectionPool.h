#ifndef MODCONNECTIONPOOL_H_
#define MODCONNECTIONPOOL_H_

#include <Connection/InputConnection.h>
#include <Util/Logger.h>

#include <vector>

using namespace eLibV2::Util;

namespace eLibV2
{
    namespace Connection
    {
        /**
        pool for all registered input-connections
        */
        class ConnectionPool
        {
        public:
            ConnectionPool() {}

        private:
            std::map<std::string, InputConnection*> Connections;
        };
    }
}

#endif
