#ifndef MODCONNECTIONPOOL_H_
#define MODCONNECTIONPOOL_H_

#include <Base/BaseConnection.h>
#include <Util/Logger.h>

#include <vector>

namespace eLibV2
{
    namespace Connection
    {
        /**
        pool for all registered connections, which are deleted at the end
        */
        class ConnectionPool
        {
        public:
            ConnectionPool() {}
            ~ConnectionPool()
            {
                if (m_pConnections.size())
                {
                    for (std::vector<Base::BaseConnection*>::iterator it = m_pConnections.begin(); it != m_pConnections.end(); ++it)
                        delete (*it);
                }
            }

            void registerConnection(Base::BaseConnection* Connection) { m_pConnections.push_back(Connection); }

        private:
            std::vector<Base::BaseConnection*> m_pConnections;
        };
    }
}

#endif
